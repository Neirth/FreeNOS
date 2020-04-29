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

#ifndef __LIBARCH_MEMORYCONTEXT_H
#define __LIBARCH_MEMORYCONTEXT_H

#include <Types.h>
#include <Macros.h>
#include <BitOperations.h>
#include "Memory.h"
#include "MemoryMap.h"

/** Forward declaration */
class SplitAllocator;

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 */

/**
 * Virtual memory abstract interface.
 */
class MemoryContext
{
  public:

    /**
     * Result codes.
     */
    typedef enum Result
    {
        Success,
        InvalidAddress,
        InvalidSize,
        AlreadyExists,
        OutOfMemory
    }
    Result;

    /**
     * Constructor.
     *
     * @param alloc Allocator used to allocate page tables.
     */
    MemoryContext(MemoryMap *map, SplitAllocator *alloc);

    /**
     * Destructor
     */
    virtual ~MemoryContext();

    /**
     * Get currently active MemoryContext.
     *
     * @return MemoryContext object pointer or NULL.
     */
    static MemoryContext * getCurrent();

    /**
     * Activate the MemoryContext.
     *
     * This function applies this MemoryContext to the hardware MMU.
     *
     * @return Result code.
     */
    virtual Result activate() = 0;

    /**
     * Map a physical page to a virtual address.
     *
     * @param virt Virtual address.
     * @param phys Physical address.
     * @param access Page entry protection flags.
     *
     * @return Result code.
     */
    virtual Result map(Address virt, Address phys, Memory::Access access) = 0;

    /**
     * Unmap a virtual address.
     *
     * This function removes a virtual to physical memory
     * mapping without deallocating any physical memory.
     *
     * @param virt Virtual address to unmap.
     *
     * @return Result code
     */
    virtual Result unmap(Address virt) = 0;

    /**
     * Translate virtual address to physical address.
     *
     * @param virt Virtual address to lookup on input, physical address on output.
     *
     * @return Result code
     */
    virtual Result lookup(Address virt, Address *phys) const = 0;

    /**
     * Get Access flags for a virtual address.
     *
     * @param virt Virtual address to get Access flags for.
     * @param access MemoryAccess object pointer.
     *
     * @return Result code.
     */
    virtual Result access(Address addr, Memory::Access *access) const = 0;

    /**
     * Map a range of physical pages to virtual addresses.
     *
     * @param range Range object describing the range of physical pages.
     *
     * @return Result code.
     */
    virtual Result mapRange(Memory::Range *range);

    /**
     * Unmaps a range of virtual memory.
     *
     * @param range Range object describing the range of virtual addresses.
     *
     * @return Result code
     */
    virtual Result unmapRange(Memory::Range *range);

    /**
     * Release a memory page mapping.
     *
     * @param virt Virtual address of the page to release.
     *
     * @return Result code
     */
    virtual Result release(Address virt);

    /**
     * Release a range of physical memory by its virtual memory pages.
     *
     * @param range Range object describing the range of physical pages to release.
     *
     * @return Result code
     */
    virtual Result releaseRange(Memory::Range *range, bool tablesOnly = false) = 0;

    /**
     * Release memory region.
     *
     * Deallocate all associated physical memory
     * which resides in the given memory region.
     *
     * @param region Memory region to release
     *
     * @return Result code
     */
    virtual Result releaseRegion(MemoryMap::Region region, bool tablesOnly = false) = 0;

    /**
     * Find unused memory.
     *
     * This function finds a contigeous block of a given size
     * of virtual memory which is unused and then returns
     * the virtual address of the first page in the block.
     *
     * @param region Memory region to search in.
     * @param size Number of bytes requested to be free.
     * @param virt Virtual memory address on output.
     *
     * @return Result code
     */
    virtual Result findFree(Size size, MemoryMap::Region region, Address *virt) const;

  protected:

    /** Physical memory allocator */
    SplitAllocator *m_alloc;

    /** Virtual memory layout */
    MemoryMap *m_map;

    /** The currently active MemoryContext */
    static MemoryContext *m_current;
};

/**
 * @}
 * @}
 */

#endif /* __LIBARCH_MEMORYCONTEXT_H */
