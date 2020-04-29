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

#include <SplitAllocator.h>
#include <MemoryBlock.h>
#include "IntelConstant.h"
#include "IntelPageDirectory.h"

#define PAGE_NONE       0
#define PAGE_PRESENT    1
#define PAGE_READ       0
#define PAGE_EXEC       0
#define PAGE_WRITE      2
#define PAGE_USER       4
#define PAGE_SECTION    (1 << 7)

/**
 * Entry inside the page directory of a given virtual address.
 *
 * @param vaddr Virtual Address.
 *
 * @return Index of the corresponding page directory entry.
 */
#define DIRENTRY(vaddr) \
    ((vaddr) >> DIRSHIFT)

IntelPageTable * IntelPageDirectory::getPageTable(Address virt, SplitAllocator *alloc) const
{
    u32 entry = m_tables[ DIRENTRY(virt) ];

    // Check if the page table is present.
    if (!(entry & PAGE_PRESENT))
        return ZERO;
    else
        return (IntelPageTable *) alloc->toVirtual(entry & PAGEMASK);
}

MemoryContext::Result IntelPageDirectory::copy(IntelPageDirectory *directory,
                                               Address from,
                                               Address to)
{
    while (from < to)
    {
        m_tables[ DIRENTRY(from) ] = directory->m_tables[ DIRENTRY(from) ];
        from += MegaByte(4);
    }
    return MemoryContext::Success;
}

MemoryContext::Result IntelPageDirectory::map(Address virt,
                                              Address phys,
                                              Memory::Access access,
                                              SplitAllocator *alloc)
{
    IntelPageTable *table = getPageTable(virt, alloc);
    Allocator::Arguments alloc_args;

    // Check if the page table is present.
    if (!table)
    {
        alloc_args.address = 0;
        alloc_args.size = sizeof(IntelPageTable);
        alloc_args.alignment = PAGESIZE;

        // Allocate a new page table
        if (alloc->allocateLow(alloc_args) != Allocator::Success)
            return MemoryContext::OutOfMemory;

        MemoryBlock::set(alloc->toVirtual(alloc_args.address), 0, sizeof(IntelPageTable));

        // Assign to the page directory
        m_tables[ DIRENTRY(virt) ] = alloc_args.address | PAGE_PRESENT | PAGE_WRITE | flags(access);
        table = getPageTable(virt, alloc);
    }
    return table->map(virt, phys, access);
}

MemoryContext::Result IntelPageDirectory::unmap(Address virt, SplitAllocator *alloc)
{
    IntelPageTable *table = getPageTable(virt, alloc);
    if (!table)
        return MemoryContext::InvalidAddress;
    else
        return table->unmap(virt);
}

MemoryContext::Result IntelPageDirectory::translate(Address virt,
                                                    Address *phys,
                                                    SplitAllocator *alloc) const
{
    IntelPageTable *table = getPageTable(virt, alloc);
    if (!table)
    {
        if (m_tables[DIRENTRY(virt)] & PAGE_SECTION)
        {
            *phys = (m_tables[DIRENTRY(virt)] & PAGEMASK) + ((virt % MegaByte(4)) & PAGEMASK);
            return MemoryContext::Success;
        }
        return MemoryContext::InvalidAddress;
    }
    else
        return table->translate(virt, phys);
}

MemoryContext::Result IntelPageDirectory::access(Address virt,
                                                 Memory::Access *access,
                                                 SplitAllocator *alloc) const
{
    IntelPageTable *table = getPageTable(virt, alloc);
    if (!table)
        return MemoryContext::InvalidAddress;
    else
        return table->access(virt, access);
}

u32 IntelPageDirectory::flags(Memory::Access access) const
{
    u32 f = 0;

    if (access & Memory::Writable) f |= PAGE_WRITE;
    if (access & Memory::User)     f |= PAGE_USER;

    return f;
}

MemoryContext::Result IntelPageDirectory::releaseRange(Memory::Range range,
                                                       SplitAllocator *alloc,
                                                       bool tablesOnly)
{
    Address phys;

    // Walk the page directory within the specified range
    for (Size i = 0; i < range.size; i += MegaByte(4))
    {
        IntelPageTable *table = getPageTable(range.virt + i, alloc);
        if (table)
        {
            // Release mapped pages
            if (!tablesOnly)
            {
                for (Size j = 0; j < MegaByte(4); j += PAGESIZE)
                {
                    if (table->translate(range.virt + i + j, &phys) == MemoryContext::Success)
                    {
                        alloc->release(phys);
                    }
                }
            }
            // Release page table
            alloc->release(m_tables[ DIRENTRY(range.virt + i) ] & PAGEMASK);
        }
    }
    return MemoryContext::Success;
}
