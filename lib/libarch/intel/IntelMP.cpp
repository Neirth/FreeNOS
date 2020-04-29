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
#include <Log.h>
#include "IntelConstant.h"
#include "IntelMP.h"
#include "IntelBoot.h"

IntelMP::IntelMP(IntelAPIC & apic)
    : CoreManager()
    , m_apic(apic)
{
    SystemInformation info;

    m_bios.map(MPAreaAddr, MPAreaSize);
    m_lastMemory.map(info.memorySize - MegaByte(1), MegaByte(1));

    m_apic.getIO().map(IntelAPIC::IOBase, PAGESIZE);
}

IntelMP::MPConfig * IntelMP::scanMemory(Address addr)
{
    MPFloat *mpf;

    // Look for the Multiprocessor configuration
    for (uint i = 0; i < MPAreaSize - sizeof(Address); i += sizeof(Address))
    {
        mpf = (MPFloat *)(addr + i);

        if (mpf->signature == MPFloatSignature)
            return (MPConfig *) (mpf->configAddr - MPAreaAddr + addr);
    }
    return ZERO;
}

IntelMP::Result IntelMP::discover()
{
    MPConfig *mpc = 0;
    MPEntry *entry;

    // Clear previous discoveries
    m_cores.clear();

    // Try to find MPTable in the BIOS memory.
    mpc = scanMemory(m_bios.getBase());

    // Retry in the last 1MB of physical memory if not found.
    if (!mpc)
    {
        mpc = scanMemory(m_lastMemory.getBase());
        if (!mpc)
        {
            ERROR("MP header not found");
            return NotFound;
        }
    }

    // Found config
    DEBUG("MP header found at " << (void *) mpc);
    DEBUG("Local APIC at " << (void *) mpc->apicAddr);
    entry = (MPEntry *)(mpc + 1);

    // Search for multiprocessor entries
    for (uint i = 0; i < mpc->count; i++)
        entry = parseEntry(entry);

    return Success;
}

IntelMP::Result IntelMP::boot(CoreInfo *info)
{
    DEBUG("booting core" << info->coreId << " at " <<
            (void *) info->memory.phys << " with kernel: " << info->kernelCommand);

    // Copy 16-bit realmode startup code
    VMCopy(SELF, API::Write, (Address) bootEntry16, MPEntryAddr, PAGESIZE);

    // Copy the CoreInfo structure
    VMCopy(SELF, API::Write, (Address) info, MPInfoAddr, sizeof(*info));

    // Send inter-processor-interrupt to wakeup the processor
    if (m_apic.sendStartupIPI(info->coreId, MPEntryAddr) != IntController::Success)
        return IOError;

    // Wait until the core raises the 'booted' flag in CoreInfo
    while (1)
    {
        CoreInfo check;

        VMCopy(SELF, API::Read, (Address) &check, MPInfoAddr, sizeof(check));

        if (check.booted)
            break;
    }
    return Success;
}

IntelMP::MPEntry * IntelMP::parseEntry(IntelMP::MPEntry *entry)
{
    if (entry->type == MPEntryProc)
    {
        m_cores.append(entry->apicId);
        return entry + 1;
    }
    else
        return (MPEntry *) (((Address)(entry)) + 8);
}
