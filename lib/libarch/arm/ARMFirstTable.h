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

#ifndef __LIBARCH_ARM_FIRSTTABLE_H
#define __LIBARCH_ARM_FIRSTTABLE_H

#include <FreeNOS/System.h>
#include <Types.h>
#include <MemoryContext.h>
#include "ARMSecondTable.h"

/** Forward declaration */
class SplitAllocator;

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_arm
 * @{
 */

/**
 * ARM first level page table
 */
class ARMFirstTable
{
  public:

    /**
     * Map a virtual address to a physical address.
     *
     * @param virt Virtual address.
     * @param phys Physical address.
     * @param access Memory access flags.
     * @param alloc Physical memory allocator for extra page tables.
     *
     * @return Result code
     */
    MemoryContext::Result map(Address virt,
                              Address phys,
                              Memory::Access access,
                              SplitAllocator *alloc);

    /**
     * Map a contigous range of virtual memory to physical memory.
     *
     * This function can map at the granularity of 1 megabyte memory chunks.
     *
     * @param range Virtual to physical memory range.
     * @param alloc Physical memory allocator for extra page tables.
     *
     * @return Result code
     */
    MemoryContext::Result mapLarge(Memory::Range range,
                                   SplitAllocator *alloc);

    /**
     * Remove virtual address mapping.
     *
     * @param virt Virtual address.
     * @param alloc Physical memory allocator
     *
     * @return Result code
     */
    MemoryContext::Result unmap(Address virt,
                                SplitAllocator *alloc);

    /**
     * Translate virtual address to physical address.
     *
     * @param virt Virtual address to lookup as input
     * @param phys Physical address corresponding to the virtual address
     * @param alloc Physical memory allocator
     *
     * @return Result code
     */
    MemoryContext::Result translate(Address virt,
                                    Address *phys,
                                    SplitAllocator *alloc) const;

    /**
     * Get Access flags for a virtual address.
     *
     * @param virt Virtual address to get Access flags for.
     * @param access MemoryAccess object pointer.
     * @param alloc Physical memory allocator
     *
     * @return Result code.
     */
    MemoryContext::Result access(Address virt,
                                 Memory::Access *access,
                                 SplitAllocator *alloc) const;

    /**
     * Release range of memory.
     *
     * @param range Memory range input
     * @param alloc Memory allocator to release memory from
     * @param tablesOnly Set to true to only release page tables and not mapped pages.
     *
     * @return Result code
     */
    MemoryContext::Result releaseRange(Memory::Range range,
                                       SplitAllocator *alloc,
                                       bool tablesOnly);

  private:

    /**
     * Retrieve second level page table
     *
     * @param virt Virtual address to fetch page table for
     * @param alloc Physical memory allocator
     *
     * @return Second level page table
     */
    ARMSecondTable * getSecondTable(Address virt,
                                    SplitAllocator *alloc) const;

    /**
     * Convert Memory::Access to first level page table flags.
     *
     * @param access Memory access flags to convert
     *
     * @return Page table access flags
     */
    u32 flags(Memory::Access access) const;

  private:

    /** Array of page table entries. */
    u32 m_tables[4096];
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_ARM_FIRSTTABLE_H */
