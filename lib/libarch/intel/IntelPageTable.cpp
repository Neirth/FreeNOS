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

#include "IntelConstant.h"
#include "IntelPageTable.h"

#define PAGE_NONE       0
#define PAGE_PRESENT    1
#define PAGE_READ       0
#define PAGE_EXEC       0
#define PAGE_WRITE      2
#define PAGE_USER       4

/**
 * Entry inside the page table of a given virtual address.
 *
 * @param vaddr Virtual Address.
 *
 * @return Index of the corresponding page table entry.
 */
#define TABENTRY(vaddr) \
    (((vaddr) >> PAGESHIFT) & 0x3ff)

MemoryContext::Result IntelPageTable::map(Address virt,
                                          Address phys,
                                          Memory::Access access)
{
    // Check if the address is already mapped
    if (m_pages[ TABENTRY(virt) ] & PAGE_PRESENT)
        return MemoryContext::AlreadyExists;

    // Insert mapping
    m_pages[ TABENTRY(virt) ] = phys | PAGE_PRESENT | flags(access);
    return MemoryContext::Success;
}

MemoryContext::Result IntelPageTable::unmap(Address virt)
{
    m_pages[ TABENTRY(virt) ] = PAGE_NONE;
    return MemoryContext::Success;
}

MemoryContext::Result IntelPageTable::translate(Address virt, Address *phys) const
{
    if (!(m_pages[ TABENTRY(virt) ] & PAGE_PRESENT))
        return MemoryContext::InvalidAddress;

    *phys = (m_pages[ TABENTRY(virt) ] & PAGEMASK) + (virt & ~PAGEMASK);
    return MemoryContext::Success;
}

MemoryContext::Result IntelPageTable::access(Address virt, Memory::Access *access) const
{
    u32 entry = m_pages[ TABENTRY(virt) ];

    if (!(entry & PAGE_PRESENT))
        return MemoryContext::InvalidAddress;

    *access = Memory::Readable;

    if (entry & PAGE_WRITE) *access |= Memory::Writable;
    if (entry & PAGE_USER)  *access |= Memory::User;

    return MemoryContext::Success;
}

u32 IntelPageTable::flags(Memory::Access access) const
{
    u32 f = 0;

    if (access & Memory::Writable) f |= PAGE_WRITE;
    if (access & Memory::User)     f |= PAGE_USER;

    return f;
}
