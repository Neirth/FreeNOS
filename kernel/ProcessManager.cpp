/*
 * Copyright (C) 2015 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <FreeNOS/System.h>
#include <Log.h>
#include <ListIterator.h>
#include "ProcessEvent.h"
#include "ProcessManager.h"

ProcessManager::ProcessManager()
    : m_procs(MAX_PROCS)
    , m_interruptNotifyList(256)
{
    DEBUG("m_procs = " << MAX_PROCS);

    m_current   = ZERO;
    m_idle      = ZERO;
    m_interruptNotifyList.fill(ZERO);
    MemoryBlock::set(&m_nextSleepTimer, 0, sizeof(m_nextSleepTimer));
}

ProcessManager::~ProcessManager()
{
}

Process * ProcessManager::create(Address entry,
                                 const MemoryMap &map,
                                 bool readyToRun,
                                 bool privileged)
{
    Process *proc = new Arch::Process(m_procs.count(), entry, privileged, map);

    // Insert to the process table
    if (proc && proc->initialize() == Process::Success)
    {
        m_procs.insert(proc);

        if (readyToRun)
        {
            proc->wakeup(true);
            m_scheduler.enqueue(proc);
        }

        if (m_current != 0)
            proc->setParent(m_current->getID());

        return proc;
    }
    return ZERO;
}

Process * ProcessManager::get(ProcessID id)
{
    Process **p = (Process **) m_procs.get(id);
    return p ? *p : ZERO;
}

void ProcessManager::remove(Process *proc, uint exitStatus)
{
    if (proc == m_idle)
        m_idle = ZERO;

    if (proc == m_current)
        m_current = ZERO;

    // Wakeup any Processes which are waiting for this Process
    Size size = m_procs.size();

    for (Size i = 0; i < size; i++)
    {
        if (m_procs[i] != ZERO &&
            m_procs[i]->getState() == Process::Waiting &&
            m_procs[i]->getWait() == proc->getID())
        {
            m_procs[i]->setWaitResult(exitStatus);
            m_procs[i]->wakeup(true);
            m_scheduler.enqueue(m_procs[i]);
        }
    }

    // Unregister any interrupt events for this process
    unregisterInterruptNotify(proc);

    // Remove process from administration and schedule
    m_procs[proc->getID()] = ZERO;
    m_scheduler.dequeue(proc, true);

    // Free the process memory
    delete proc;
}

ProcessManager::Result ProcessManager::schedule()
{
    Timer *timer = Kernel::instance->getTimer();

    // Let the scheduler select a new process
    Process *proc = m_scheduler.select();

    // If no process ready, let us idle
    if (!proc)
        proc = m_idle;

    if (!proc)
    {
        FATAL("no process found to run!");
    }

    // Wakeup processes if the next sleeptimer expired
    if (timer->isExpired(m_nextSleepTimer))
    {
        MemoryBlock::set(&m_nextSleepTimer, 0, sizeof(m_nextSleepTimer));

        // Loop all procs, wakeup() those which have their sleep timer expired
        for (Size i = 0; i < MAX_PROCS; i++)
        {
            Process *p = m_procs.at(i);
            if (p && p->getState() == Process::Sleeping)
            {
                const Timer::Info & procTimer = p->getSleepTimer();

                if (timer->isExpired(procTimer))
                {
                    wakeup(p);
                }
                else if (procTimer.ticks < m_nextSleepTimer.ticks || !m_nextSleepTimer.ticks)
                {
                    MemoryBlock::copy(&m_nextSleepTimer, &procTimer, sizeof(m_nextSleepTimer));
                }
            }
        }
    }

    // Only execute if its a different process
    if (proc != m_current)
    {
        Process *previous = m_current;
        m_current = proc;
        proc->execute(previous);
    }

    return Success;
}

Process * ProcessManager::current()
{
    return m_current;
}

void ProcessManager::setIdle(Process *proc)
{
    m_idle = proc;
    m_scheduler.dequeue(proc, true);
}

Vector<Process *> * ProcessManager::getProcessTable()
{
    return &m_procs;
}

ProcessManager::Result ProcessManager::wait(Process *proc)
{
    if (m_current->wait(proc->getID()) != Process::Success)
    {
        ERROR("process ID " << m_current->getID() << " failed to wait");
        return IOError;
    }

    if (m_scheduler.dequeue(m_current) != Scheduler::Success)
    {
        ERROR("process ID " << m_current->getID() << " not removed from Scheduler");
        return IOError;
    }

    return Success;
}

ProcessManager::Result ProcessManager::sleep(const Timer::Info *timer, bool ignoreWakeups)
{
    Process::Result result;

    if (!m_current)
    {
        ERROR("no current process found");
        return InvalidArgument;
    }

    result = m_current->sleep(timer, ignoreWakeups);
    switch (result)
    {
        case Process::WakeupPending:
            return WakeupPending;

        case Process::Success:
            if (m_scheduler.dequeue(m_current) != Scheduler::Success)
            {
                ERROR("process ID " << m_current->getID() << " not removed from Scheduler");
                return IOError;
            }

            if (timer && (timer->ticks < m_nextSleepTimer.ticks || !m_nextSleepTimer.ticks))
            {
                MemoryBlock::copy(&m_nextSleepTimer, timer, sizeof(m_nextSleepTimer));
            }
            break;

        default:
            ERROR("failed to sleep process ID " << m_current->getID() <<
                  ": result: " << (uint) result);
            return IOError;
    }

    return Success;
}

ProcessManager::Result ProcessManager::wakeup(Process *proc)
{
    Process::Result result;
    Process::State state = proc->getState();

    if ((result = proc->wakeup()) != Process::Success)
    {
        ERROR("failed to wakeup process ID " << proc->getID() <<
              ": result: " << (uint) result);
        return IOError;
    }

    if (state != Process::Ready)
    {
        if (m_scheduler.enqueue(proc) != Scheduler::Success)
        {
            ERROR("process ID " << proc->getID() << " not added to Scheduler");
            return IOError;
        }
    }

    return Success;
}

ProcessManager::Result ProcessManager::raiseEvent(Process *proc, struct ProcessEvent *event)
{
    Process::Result result;
    Process::State state = proc->getState();

    if ((result = proc->raiseEvent(event)) != Process::Success)
    {
        ERROR("failed to raise event in process ID " << proc->getID() <<
              ": result: " << (uint) result);
        return IOError;
    }

    if (state != Process::Ready)
    {
        if (m_scheduler.enqueue(proc) != Scheduler::Success)
        {
            ERROR("process ID " << proc->getID() << " not added to Scheduler");
            return IOError;
        }
    }

    return Success;
}

ProcessManager::Result ProcessManager::registerInterruptNotify(Process *proc, u32 vec)
{
    // Create List if necessary
    if (!m_interruptNotifyList[vec])
    {
        m_interruptNotifyList.insert(vec, new List<Process *>());
    }

    // Check for duplicates
    if (m_interruptNotifyList[vec]->contains(proc))
        return AlreadyExists;

    // Append the Process
    m_interruptNotifyList[vec]->append(proc);
    return Success;
}

ProcessManager::Result ProcessManager::unregisterInterruptNotify(Process *proc)
{
    // Remove the Process from all notify lists
    for (Size i = 0; i < m_interruptNotifyList.size(); i++)
    {
        List<Process *> *lst = m_interruptNotifyList[i];
        if (lst)
        {
            lst->remove(proc);
        }
    }

    return Success;
}

ProcessManager::Result ProcessManager::interruptNotify(u32 vector)
{
    List<Process *> *lst = m_interruptNotifyList[vector];
    if (lst)
    {
        ProcessEvent event;
        event.type   = InterruptEvent;
        event.number = vector;

        for (ListIterator<Process *> i(lst); i.hasCurrent(); i++)
        {
            if (raiseEvent(i.current(), &event) != Success)
            {
                ERROR("failed to raise InterruptEvent for IRQ #" << vector <<
                      " on Process ID " << i.current()->getID());
                return IOError;
            }
        }
    }

    return Success;
}
