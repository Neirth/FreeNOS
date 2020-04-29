/*
 * Copyright (C) 2009 Niek Linnenbank
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
#include <FreeNOS/Kernel.h>
#include <FreeNOS/Config.h>
#include <FreeNOS/Process.h>
#include <FreeNOS/ProcessEvent.h>
#include <Log.h>
#include "ProcessCtl.h"

API::Result ProcessCtlHandler(ProcessID procID,
                              ProcessOperation action,
                              Address addr,
                              Address output)
{
    Process *proc = ZERO;
    ProcessInfo *info = (ProcessInfo *) addr;
    ProcessManager *procs = Kernel::instance->getProcessManager();
    Timer *timer;
    Arch::MemoryMap map;

    DEBUG("#" << procs->current()->getID() << " " << action << " -> " << procID << " (" << addr << ")");

    // Does the target process exist?
    if(action != GetPID && action != Spawn)
    {
        if (procID == SELF)
            proc = procs->current();
        else if (!(proc = procs->get(procID)))
            return API::NotFound;
    }
    // Handle request
    switch (action)
    {
    case Spawn:
        proc = procs->create(addr, map);
        if (!proc)
        {
            ERROR("failed to create process");
            return API::IOError;
        }
        return proc->getID();

    case KillPID:
        procs->remove(proc, addr); // Addr contains the exit status
        procs->schedule();
        break;

    case GetPID:
        return procs->current()->getID();

    case GetParent:
        return procs->current()->getParent();

    case Schedule:
        procs->schedule();
        break;

    case Resume:
        // increment wakeup counter and set process ready
        if (procs->wakeup(proc) != ProcessManager::Success)
        {
            ERROR("failed to wakeup process ID " << proc->getID());
            return API::IOError;
        }
        break;

    case WatchIRQ:
        if (procs->registerInterruptNotify(proc, IRQ(addr)) != ProcessManager::Success)
        {
            ERROR("failed to register IRQ #" << IRQ(addr) << " to process ID " << proc->getID());
            return API::IOError;
        }
        break;

    case EnableIRQ:
        Kernel::instance->enableIRQ(addr, true);
        break;

    case DisableIRQ:
        Kernel::instance->enableIRQ(addr, false);
        break;

    case InfoPID:
        info->id    = proc->getID();
        info->state = proc->getState();
        info->parent = proc->getParent();
        break;

    case WaitPID:
        if (procs->wait(proc) != ProcessManager::Success)
        {
            ERROR("failed to wait for Process ID " << proc->getID());
            return API::IOError;
        }
        procs->schedule();

        // contains the exit status of the other process.
        // Note that only the Intel code has kernel stacks.
        // For ARM, the kernel continues executing here even after
        // the schedule() is done. For ARM, the actual wait result is
        // injected directly in the saved CPU registers.
        return procs->current()->getWaitResult();

    case InfoTimer:
        if (!(timer = Kernel::instance->getTimer()))
            return API::NotFound;

        timer->getCurrent((Timer::Info *) addr);
        break;

    case WaitTimer:
        // Process is only allowed to continue execution after the sleep timer expires
        if (procs->sleep((const Timer::Info *)addr, true) != ProcessManager::Success)
        {
            ERROR("sleep failed on process ID " << procs->current()->getID());
            return API::IOError;
        }
        procs->schedule();
        break;

    case EnterSleep:
        // Only sleeps the process if no pending wakeups
        if (procs->sleep((const Timer::Info *)addr) == ProcessManager::Success)
            procs->schedule();
        break;
    }
    return API::Success;
}

Log & operator << (Log &log, ProcessOperation op)
{
    switch (op)
    {
        case Spawn:     log.append("Spawn"); break;
        case KillPID:   log.append("KillPID"); break;
        case GetPID:    log.append("GetPID"); break;
        case GetParent: log.append("GetParent"); break;
        case WatchIRQ:  log.append("WatchIRQ"); break;
        case EnableIRQ: log.append("EnableIRQ"); break;
        case DisableIRQ:log.append("DisableIRQ"); break;
        case InfoPID:   log.append("InfoPID"); break;
        case WaitPID:   log.append("WaitPID"); break;
        case InfoTimer: log.append("InfoTimer"); break;
        case EnterSleep: log.append("EnterSleep"); break;
        case Schedule:  log.append("Schedule"); break;
        case Resume:    log.append("Resume"); break;
        default:        log.append("???"); break;
    }
    return log;
}
