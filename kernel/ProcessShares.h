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

#ifndef __KERNEL_PROCESSSHARES_H
#define __KERNEL_PROCESSSHARES_H

#include <Types.h>
#include <Macros.h>
#include <List.h>
#include <MemoryMap.h>
#include <Index.h>
#include <MemoryContext.h>

class MemoryChannel;
class Process;

/**
 * @addtogroup kernel
 * @{
 */

/**
 * Manages memory shares for a Process.
 */
class ProcessShares
{
  public:

    typedef struct MemoryShare
    {
        /** Remote process id for this share */
        ProcessID pid;

        /** CoreId for the other process */
        Size coreId;

        /** Share tag id is defined by the application */
        Size tagId;

        /** Physical memory address range. */
        Memory::Range range;

        /** True if the share is attached (used by both processes) */
        bool attached;

        bool operator == (const struct MemoryShare & sh) const
        {
            return true;
        }
        bool operator != (const struct MemoryShare & sh) const
        {
            return false;
        }
    }
    MemoryShare;

    enum Result
    {
        Success,
        InvalidArgument,
        MemoryMapError,
        OutOfMemory,
        AlreadyExists,
        NotFound
    };

    /**
     * Constructor.
     */
    ProcessShares(ProcessID pid);

    /**
     * Destructor function.
     */
    virtual ~ProcessShares();

    /**
     * Get process.
     */
    const ProcessID getProcessID() const;

    /**
     * Get MemoryContext object.
     *
     * @return MemoryContext object pointer.
     */
    MemoryContext * getMemoryContext();

    /**
     * Set MemoryContext.
     *
     * @param context MemoryContext object
     *
     * @return Result code.
     */
    Result setMemoryContext(MemoryContext *context);

    /**
     *
     */
    Result createShare(ProcessShares & instance,
                       MemoryShare *share);

    /**
     * Create memory share.
     *
     * @param pid ProcessID for the share.
     * @param coreId CoreID for the share.
     * @param tagId TagID for the share.
     * @param virt Virtual address of the share.
     * @param size Size of the share.
     *
     * @return Result code.
     */
    Result createShare(ProcessID pid,
                       Size coreId,
                       Size tagId,
                       Address virt,
                       Size size);

    /**
     * Read memory share by Process, Core and Tag IDs.
     *
     * @param share MemoryShare buffer (input/output).
     *
     * @return Result code.
     */
    Result readShare(MemoryShare *share);

    /**
     * Remove all shares for the given ProcessID
     *
     * @param pid ProcessID to remove all shares for
     *
     * @return Result code
     */
    Result removeShares(ProcessID pid);

  private:

    /**
     * Release one memory share
     *
     * @param share MemoryShare object pointer
     * @param idx Index position of the object
     *
     * @return Result code
     */
    Result releaseShare(MemoryShare *share, Size idx);

  private:

    /** ProcessID associated to these shares */
    ProcessID m_pid;

    /** MemoryContext instance */
    MemoryContext *m_memory;

    /** Memory channel for sending kernel events to the associated Process */
    MemoryChannel *m_kernelChannel;

    /** Contains all memory shares */
    Index<MemoryShare> m_shares;
};

/**
 * @}
 */

#endif /* __KERNEL_PROCESSSHARES_H */
