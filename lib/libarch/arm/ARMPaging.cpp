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
#include <SplitAllocator.h>
#include <MemoryBlock.h>
#include <Log.h>
#include "ARMCore.h"
#include "ARMControl.h"
#include "ARMPaging.h"
#include "ARMFirstTable.h"

ARMPaging::ARMPaging(MemoryMap *map, SplitAllocator *alloc)
    : MemoryContext(map, alloc)
{
    Allocator::Arguments alloc_args;
    alloc_args.address = 0;
    alloc_args.size = sizeof(ARMFirstTable);
    alloc_args.alignment = sizeof(ARMFirstTable);

    // Allocate page directory from low physical memory.
    if (alloc->allocateLow(alloc_args) == Allocator::Success)
    {
        m_firstTableAddr = alloc_args.address;
        m_firstTable = (ARMFirstTable *) alloc->toVirtual(m_firstTableAddr);

        // Initialize the page directory
        MemoryBlock::set(m_firstTable, 0, sizeof(ARMFirstTable));

        // Map the kernel. The kernel has permanently mapped 1GB of
        // physical memory (i.e. the "low memory" in SplitAllocator). The low
        // memory starts at its physical base address offset (varies per core).
        m_firstTable->mapLarge( m_map->range(MemoryMap::KernelData), m_alloc );

        // Unmap I/O zone
        for (Size i = 0; i < IO_SIZE; i += MegaByte(1))
            m_firstTable->unmap(IO_BASE + i, m_alloc);

        // Map the I/O zone as Device / Uncached memory.
        Memory::Range io;
        io.phys = IO_BASE;
        io.virt = IO_BASE;
        io.size = IO_SIZE;
        io.access = Memory::Readable | Memory::Writable | Memory::Device;
        m_firstTable->mapLarge(io, m_alloc);
    }
}

ARMPaging::~ARMPaging()
{
    for (Size i = 0; i < sizeof(ARMFirstTable); i += PAGESIZE)
        m_alloc->release(m_firstTableAddr + i);
}

#ifdef ARMV6
MemoryContext::Result ARMPaging::enableMMU()
{
    ARMControl ctrl;

    // Program first level table. Enable L2 cache for page walking.
    ctrl.write(ARMControl::TranslationTable0, ((u32) m_firstTableAddr | 1));
    ctrl.write(ARMControl::TranslationTable1,    0);
    ctrl.write(ARMControl::TranslationTableCtrl, 0);

    // Set Control flags
    ctrl.set(ARMControl::DomainClient);
    ctrl.set(ARMControl::DisablePageColoring);
    ctrl.set(ARMControl::AccessPermissions);
    ctrl.set(ARMControl::ExtendedPaging);
    ctrl.unset(ARMControl::BranchPrediction);

    // Flush TLB's and caches
    tlb_flush_all();
    m_cache.cleanInvalidate(Cache::Unified);

    // Disable caches.
    ctrl.unset(ARMControl::InstructionCache);
    ctrl.unset(ARMControl::DataCache);

    // Enable the MMU. This re-enables instruction and data cache too.
    ctrl.set(ARMControl::MMUEnabled);
    NOTICE("MMUEnabled = " << (ctrl.read(ARMControl::SystemControl) & ARMControl::MMUEnabled));
    tlb_flush_all();

    // Reactivate both caches and branch prediction
    ctrl.set(ARMControl::InstructionCache);
    ctrl.set(ARMControl::DataCache);
    ctrl.set(ARMControl::BranchPrediction);

    return Success;
}
#elif defined(ARMV7)

MemoryContext::Result ARMPaging::enableMMU()
{
    ARMControl ctrl;

    // Flush TLB's
    tlb_flush_all();
    dsb();
    isb();

    // Enable branch prediction
    ctrl.set(ARMControl::BranchPrediction);

    // Program first level table
    ctrl.write(ARMControl::TranslationTable0, (((u32) m_firstTableAddr) |
        (1 << 3) | // outer write-back, write-allocate
        (1 << 6)   // inner write-back, write-allocate
    ));
    ctrl.write(ARMControl::TranslationTable1,    0);
    ctrl.write(ARMControl::TranslationTableCtrl, 0);
    dsb();
    isb();

    // Set as client for all domains
    ctrl.write(ARMControl::DomainControl, 0x55555555);

    // Enable the MMU.
    u32 nControl = ctrl.read(ARMControl::SystemControl);

    // Raise all caching, MMU and branch prediction flags.
    nControl |= (1 << 11) | (1 << 2) | (1 << 12) | (1 << 0) | (1 << 5);

    // Write back to set.
    ctrl.write(ARMControl::SystemControl, nControl);
    isb();

    NOTICE("MMUEnabled = " << (ctrl.read(ARMControl::SystemControl) & ARMControl::MMUEnabled));

    // Need to enable alignment faults separately of the MMU,
    // otherwise QEMU will hard reset the CPU
    ctrl.set(ARMControl::AlignmentFaults);

    // Flush all
    tlb_flush_all();
    dsb();
    isb();
    return Success;
}
#endif /* ARMV7 */

MemoryContext::Result ARMPaging::activate()
{
    ARMControl ctrl;

    // Do we need to (re)enable the MMU?
    if (!(ctrl.read(ARMControl::SystemControl) & ARMControl::MMUEnabled))
    {
        enableMMU();
    }
    // MMU already enabled, we only need to change first level table and flush caches.
    else
    {
#ifdef ARMV6
        mcr(p15, 0, 0, c7, c5,  0);    // flush entire instruction cache
        mcr(p15, 0, 0, c7, c10, 0);    // flush entire data cache
        mcr(p15, 0, 0, c7, c7,  0);    // flush entire cache
        mcr(p15, 0, 5, c7, c10, 0);    // data memory barrier
        mcr(p15, 0, 4, c7, c10, 0);    // memory sync barrier
#else
        m_cache.cleanInvalidate(Cache::Unified);
#endif /* ARMV6 */

        // Switch first page table and re-enable L1 caching
        ctrl.write(ARMControl::TranslationTable0, (((u32) m_firstTableAddr) |
            (1 << 3) | /* outer write-back, write-allocate */
            (1 << 6)   /* inner write-back, write-allocate */
        ));

        // Flush TLB caches
        tlb_flush_all();

        // Synchronize execution stream
        isb();
    }
    // Done. Update currently active context pointer
    m_current = this;
    return Success;
}

MemoryContext::Result ARMPaging::map(Address virt, Address phys, Memory::Access acc)
{
    // Modify page tables
    Result r = m_firstTable->map(virt, phys, acc, m_alloc);

    // Flush the TLB to refresh the mapping
    if (m_current == this)
        tlb_invalidate(virt);

    // Synchronize execution stream.
    isb();
    return r;
}

MemoryContext::Result ARMPaging::unmap(Address virt)
{
    // Clean the given data page in cache
    if (m_current == this)
        m_cache.cleanInvalidateAddress(Cache::Data, virt);

    // Modify page tables
    Result r = m_firstTable->unmap(virt, m_alloc);

    // Flush TLB to refresh the mapping
    if (m_current == this)
        tlb_invalidate(virt);

    // Synchronize execution stream
    isb();
    return r;
}

MemoryContext::Result ARMPaging::lookup(Address virt, Address *phys) const
{
    return m_firstTable->translate(virt, phys, m_alloc);
}

MemoryContext::Result ARMPaging::access(Address virt, Memory::Access *access) const
{
    return m_firstTable->access(virt, access, m_alloc);
}

MemoryContext::Result ARMPaging::releaseRegion(MemoryMap::Region region, bool tablesOnly)
{
    return m_firstTable->releaseRange(m_map->range(region), m_alloc, tablesOnly);
}

MemoryContext::Result ARMPaging::releaseRange(Memory::Range *range, bool tablesOnly)
{
    return m_firstTable->releaseRange(*range, m_alloc, tablesOnly);
}
