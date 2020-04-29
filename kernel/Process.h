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

#ifndef __KERNEL_PROCESS_H
#define __KERNEL_PROCESS_H

#include <Types.h>
#include <Macros.h>
#include <List.h>
#include <MemoryMap.h>
#include <Timer.h>
#include <Index.h>
#include "ProcessShares.h"

/** @see IPCMessage.h. */
struct Message;
class MemoryContext;
class MemoryChannel;
struct ProcessEvent;
class ProcessManager;
class Scheduler;

/**
 * @addtogroup kernel
 * @{
 */

/**
 * Represents a process which may run on the host.
 */
class Process
{
  friend class ProcessManager;
  friend class Scheduler;

  public:

    /**
     * Result codes
     */
    enum Result
    {
        Success,
        InvalidArgument,
        MemoryMapError,
        OutOfMemory,
        WakeupPending
    };

    /**
     * Represents the execution state of the Process
     */
    enum State
    {
        Ready,
        Sleeping,
        Waiting
    };

  public:

    /**
     * Constructor function.
     *
     * @param id Process Identifier
     * @param entry Initial program counter value.
     * @param privileged If true, the process has unlimited access to hardware.
     */
    Process(ProcessID id, Address entry, bool privileged, const MemoryMap &map);

    /**
     * Destructor function.
     */
    virtual ~Process();

    /**
     * Retrieve our ID number.
     *
     * @return Process Identification number.
     */
    ProcessID getID() const;

    /**
     * Retrieve our parent ID.
     *
     * @return Process ID of our parent.
     */
    ProcessID getParent() const;

    /**
     * Get Wait ID.
     */
    ProcessID getWait() const;

    /**
     * Get wait result
     */
    uint getWaitResult() const;

    /**
     * Get process shares.
     *
     * @return Reference to memory shares.
     */
    ProcessShares & getShares();

    /**
     * Retrieves the current state.
     *
     * @return Current status of the Process.
     */
    State getState() const;

    /**
     * Get MMU memory context.
     *
     * @return MemoryContext pointer.
     */
    MemoryContext * getMemoryContext();


    /**
     * Get privilege.
     *
     * @return Privilege of the Process.
     */
    bool isPrivileged() const;

    /**
     * Compare two processes.
     *
     * @param p Process to compare with.
     *
     * @return True if equal, false otherwise.
     */
    bool operator == (Process *proc);

  protected:

    /**
     * Initialize the Process.
     *
     * Allocates various (architecture specific) resources,
     * creates MMU context and stacks.
     *
     * @return Result code
     */
    virtual Result initialize();

    /**
     * Allow the Process to run on the CPU.
     *
     * @param previous The previous Process which ran on the CPU. ZERO if none.
     */
    virtual void execute(Process *previous) = 0;

    /**
     * Prevent process from sleeping.
     *
     * @param ignorePendingSleep True to ignore any (just) pending sleep of this Process
     *
     * @return Result code
     */
    Result wakeup(bool ignorePendingSleep = false);

    /**
     * Stops the process for executing until woken up
     *
     * @param timer Timer on which the process must be woken up (if expired), or ZERO for no limit
     * @param ignoreWakeups True to enter Sleep state regardless of pending wakeups
     *
     * @return Result code
     */
    Result sleep(const Timer::Info *timer, bool ignoreWakeups);

    /**
     * Let Process wait for other Process to terminate.
     *
     * @param id Process ID to wait for
     *
     * @return Result code
     */
    Result wait(ProcessID id);

    /**
     * Raise kernel event
     *
     * @return Result code
     */
    Result raiseEvent(struct ProcessEvent *event);

    /**
     * Get sleep timer.
     *
     * @return Sleep timer value.
     */
    const Timer::Info & getSleepTimer() const;

    /**
     * Set parent process ID.
     */
    void setParent(ProcessID id);

    /**
     * Set wait result
     *
     * @param result Exit code of the other process
     */
    virtual void setWaitResult(uint result);

  protected:

    /** Process Identifier */
    const ProcessID m_id;

    /** Parent process */
    ProcessID m_parent;

    /** Current process status. */
    State m_state;

    /** Waits for exit of this Process. */
    ProcessID m_waitId;

    /** Wait exit result of the other Process. */
    uint m_waitResult;

    /** Privilege level */
    bool m_privileged;

    /** Entry point of the program */
    Address m_entry;

    /** Virtual memory layout */
    MemoryMap m_map;

    /** MMU memory context */
    MemoryContext *m_memoryContext;

    /** Number of wakeups received */
    Size m_wakeups;

    /**
     * Sleep timer value.
     * If non-zero, set the process in the Ready state
     * when the System timer is greater than this value.
     */
    Timer::Info m_sleepTimer;

    /** Contains virtual memory shares between this process and others. */
    ProcessShares m_shares;

    /** Channel for sending kernel events to the Process */
    MemoryChannel *m_kernelChannel;
};

/**
 * @}
 */

#endif /* __KERNEL_PROCESS_H */
