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

#ifndef __LIBARCH_INTEL_PAGING_H
#define __LIBARCH_INTEL_PAGING_H

#include <Types.h>
#include "MemoryContext.h"
#include "MemoryMap.h"
#include "IntelPageDirectory.h"

/** Forward declaration */
class SplitAllocator;

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_intel
 * @{
 */

/**
 * Intel virtual memory implementation.
 */
class IntelPaging : public MemoryContext
{
  public:

    /**
     * Constructor.
     *
     * Allocates new page tables for the paged memory context.
     *
     * @param map Virtual memory map.
     * @param alloc Allocator for physical memory page allocations.
     */
    IntelPaging(MemoryMap *map, SplitAllocator *alloc);

    /**
     * Constructor.
     *
     * Assign the given page directory to this paged memory context.
     * This constructor does not allocate new page tables.
     *
     * @param map Virtual memory map.
     * @param pageDirectory Physical address of the page directory to use.
     * @param alloc Allocator for physical memory page allocations.
     */
    IntelPaging(MemoryMap *map, Address pageDirectory, SplitAllocator *alloc);

    /**
     * Destructor.
     */
    virtual ~IntelPaging();

    /**
     * Activate the MemoryContext.
     *
     * This function applies this MemoryContext to the hardware MMU.
     *
     * @return Result code.
     */
    virtual Result activate();

    /**
     * Map a physical page to a virtual address.
     *
     * @param virt Virtual address.
     * @param phys Physical address.
     * @param access Memory access flags.
     *
     * @return Result code
     */
    virtual Result map(Address virt, Address phys, Memory::Access access);

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
    virtual Result unmap(Address virt);

    /**
     * Translate virtual address to physical address.
     *
     * @param virt Virtual address to lookup on input, physical address on output.
     *
     * @return Result code
     */
    virtual Result lookup(Address virt, Address *phys) const;

    /**
     * Get Access flags for a virtual address.
     *
     * @param virt Virtual address to get Access flags for.
     * @param access MemoryAccess object pointer.
     *
     * @return Result code.
     */
    virtual Result access(Address addr, Memory::Access *access) const;

    /**
     * Release region of memory.
     *
     * @param region Memory region input
     * @param tablesOnly Set to true to only release page tables and not mapped pages.
     *
     * @return Result code.
     */
    virtual Result releaseRegion(MemoryMap::Region region, bool tablesOnly);

    /**
     * Release range of memory.
     *
     * @param range Memory range input
     * @param tablesOnly Set to true to only release page tables and not mapped pages.
     *
     * @return Result code.
     */
    virtual Result releaseRange(Memory::Range *range, bool tablesOnly);

  private:

    /** Pointer to page directory in kernel's virtual memory. */
    IntelPageDirectory *m_pageDirectory;

    /** Physical address of the page directory */
    Address m_pageDirectoryAddr;

    /** Set to true if page directory was allocated by this class */
    bool m_pageDirectoryAllocated;
};

namespace Arch
{
    typedef IntelPaging Memory;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_INTEL_PAGING_H */
