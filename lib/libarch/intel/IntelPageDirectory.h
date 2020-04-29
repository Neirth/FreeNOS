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

#ifndef __LIBARCH_INTEL_PAGEDIRECTORY_H
#define __LIBARCH_INTEL_PAGEDIRECTORY_H

#include <Types.h>
#include <MemoryContext.h>
#include "IntelPageTable.h"

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
 * Intel page directory implementation.
 */
class IntelPageDirectory
{
  public:

    /**
     * Copy mappings from another directory.
     *
     * @param directory Source page directory to copy from
     * @param from Virtual address to start copy mapping from
     * @param to End virtual address of mappings to copy
     *
     * @return Result code
     */
    MemoryContext::Result copy(IntelPageDirectory *directory,
                               Address from,
                               Address to);

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
     * Remove virtual address mapping.
     *
     * @param virt Virtual address.
     *
     * @return Result code
     */
    MemoryContext::Result unmap(Address virt,
                                SplitAllocator *alloc);

    /**
     * Translate virtual address to physical address.
     *
     * @param virt Virtual address to lookup on input, physical address on output.
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
     * @return Result code.
     */
    MemoryContext::Result releaseRange(Memory::Range range,
                                       SplitAllocator *alloc,
                                       bool tablesOnly);

  private:

    /**
     * Retrieve second level page table
     *
     * @param virt Input virtual address to find second level page table for
     * @param alloc Memory allocator for high/low translation
     *
     * @return Pointer to second level page table
     */
    IntelPageTable * getPageTable(Address virt, SplitAllocator *alloc) const;

    /**
     * Convert Memory::Access to page directory flags.
     *
     * @param acess Input memory access flags
     *
     * @return Page directory flags
     */
    u32 flags(Memory::Access access) const;

  private:

    /** Array of page directory entries. */
    u32 m_tables[1024];
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_INTEL_PAGEDIRECTORY_H */
