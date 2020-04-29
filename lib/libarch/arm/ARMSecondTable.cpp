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
#include "ARMCore.h"
#include "ARMConstant.h"
#include "ARMSecondTable.h"

/*
 *  nG: bits 11
 *   S: bits 10
 * APX: bits 9
 * TEX: bits 6, 7, 8
 *  AP: bits 4, 5
 *   C: bits 3
 *   B: bits 2
 *  XN: bits 0
 *
 * @see ARM Architecture Reference Manual, page 709.
 */
#define PAGE2_NONE      (0)
#define PAGE2_PRESENT   (1 << 1)

/**
 * @group Second Level Memory Types
 *
 * Inner cache: local L1 cache for one core.
 * Outer cache: shared L2 cache for a subset of multiple cores.
 * Write-Back cache: sync cache data later, let host continue immediately (faster).
 * Write-Through cache: sync cache and RAM simulatously (slower).
 * Allocate-On-Write: Each write claims a cache entry.
 * No-Allocate-On-Write: Write do no claim a cache entry, only write the RAM directly.
 *
 * @see https://en.wikipedia.org/wiki/Cache_(computing)
 * @{
 */

/** Disable all caching */
#define PAGE2_UNCACHED            (PAGE2_TEX)

/** Outer and Inner Write-Back. Allocate on write. */
#define PAGE2_CACHE_WRITEBACK     (PAGE2_TEX | PAGE2_CACHE | PAGE2_BUFFER)

/** Outer and Inner Write-Through. No allocate on write. */
#define PAGE2_CACHE_WRITETHROUGH  (PAGE2_CACHE)

/** Memory Mapped Device (Private) */
#define PAGE2_DEVICE_PRIV         ((1 << 7))

/** Memory Mapped Device (Shared) */
#define PAGE2_DEVICE_SHARED       (PAGE2_BUFFER)

#define PAGE2_TEX       (1 << 6)
#define PAGE2_CACHE     (1 << 3)
#define PAGE2_BUFFER    (1 << 2)
#define PAGE2_SHARED    (1 << 10)

/**
 * Access permission flags
 * @{
 */

/** No-execute bit flag */
#define PAGE2_NOEXEC    (1 << 0)

/* Read-only flag */
#define PAGE2_APX       (1 << 9)

/* User access permissions flag */
#define PAGE2_AP_USER   (1 << 5)

/* System access permissions flag */
#define PAGE2_AP_SYS    (1 << 4)

/**
 * @}
 */

/**
 * Entry inside the page table of a given virtual address.
 *
 * @param vaddr Virtual Address.
 *
 * @return Index of the corresponding page table entry.
 */
#define TABENTRY(vaddr) \
    (((vaddr) >> PAGESHIFT) & 0xff)

MemoryContext::Result ARMSecondTable::map(Address virt,
                                          Address phys,
                                          Memory::Access access)
{
    Arch::Cache cache;

    // Check if the address is already mapped
    if (m_pages[ TABENTRY(virt) ] & PAGE2_PRESENT)
        return MemoryContext::AlreadyExists;

    // Insert mapping
    m_pages[ TABENTRY(virt) ] = phys | PAGE2_PRESENT | flags(access);
    cache.cleanData(&m_pages[TABENTRY(virt)]);
    return MemoryContext::Success;
}

MemoryContext::Result ARMSecondTable::unmap(Address virt)
{
    Arch::Cache cache;

    m_pages[ TABENTRY(virt) ] = PAGE2_NONE;
    cache.cleanData(&m_pages[TABENTRY(virt)]);
    return MemoryContext::Success;
}

MemoryContext::Result ARMSecondTable::translate(Address virt, Address *phys) const
{
    if (!(m_pages[ TABENTRY(virt) ] & PAGE2_PRESENT))
        return MemoryContext::InvalidAddress;

    *phys = (m_pages[ TABENTRY(virt) ] & PAGEMASK) + (virt & ~PAGEMASK) ;
    return MemoryContext::Success;
}

MemoryContext::Result ARMSecondTable::access(Address virt, Memory::Access *access) const
{
    u32 entry = m_pages[ TABENTRY(virt) ];

    if (!(entry & PAGE2_PRESENT))
        return MemoryContext::InvalidAddress;

    // Permissions
    *access = Memory::Readable;

    if (entry & PAGE2_AP_USER)
        *access |= Memory::User;

    if (!(entry & PAGE2_APX))
        *access |= Memory::Writable;

    // Caching
    if (entry & PAGE2_DEVICE_SHARED)
        *access |= Memory::Device;
    else if (entry & PAGE2_UNCACHED)
        *access |= Memory::Uncached;
    else
        *access |= Memory::InnerCached | Memory::OuterCached;

    return MemoryContext::Success;
}

u32 ARMSecondTable::flags(Memory::Access access) const
{
    u32 f = PAGE2_AP_SYS;

    // Permissions
    if (!(access & Memory::Executable)) f |= PAGE2_NOEXEC;
    if ((access & Memory::User))        f |= PAGE2_AP_USER;
    if (!(access & Memory::Writable))   f |= PAGE2_APX;

    // Caching
    if (access & Memory::Device)        f |= PAGE2_DEVICE_SHARED;
    else if (access & Memory::Uncached) f |= PAGE2_UNCACHED;
    else                                f |= PAGE2_CACHE_WRITEBACK;

    return f;
}
