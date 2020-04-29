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
#include "IntelACPI.h"

IntelACPI::IntelACPI()
{
    m_bios.map(RSDBase, RSDSize);
}

IntelACPI::Result IntelACPI::initialize()
{
    RootSystemDescriptor1 *rsd1 = ZERO;
    RootSystemDescriptor2 *rsd2 = ZERO;
    Address addr = m_bios.getBase();

    // Look for the Multiprocessor configuration
    for (uint i = 0; i < RSDSize - sizeof(Address); i += sizeof(Address))
    {
        rsd1 = (RootSystemDescriptor1 *)(addr + i);

        if (rsd1->signature[0] == RootSystemSignature1 &&
            rsd1->signature[1] == RootSystemSignature2)
        {
            // Found ACPI
            DEBUG("found ACPI RootSys at " << (void *) rsd1);
            DEBUG("ACPI v" << (rsd1->revision + 1) << ".0");
            break;
        }
    }

    // Check if the ACPI tables are found
    if (!rsd1)
        return NotFound;

    if (rsd1->revision == 0)
    {
        m_rootIO.map(rsd1->rsdtAddress, PAGESIZE);
        NOTICE("RootSystemTable found");
    }
    else
    {
        rsd2 = (RootSystemDescriptor2 *) rsd1;
        m_rootIO.map((u32) rsd2->xsdtAddress, PAGESIZE);
        NOTICE("ExtendedSystemTable found");
    }
    return Success;
}

IntelACPI::Result IntelACPI::scanAPIC(MultipleAPICTable *madt)
{
    MultipleAPICTableEntry *entry = &madt->entry[0];
    MultipleAPICTableProc *proc;
    Size j = 0, madt_length = madt->header.length - sizeof(MultipleAPICTable);

    // Search for APIC entries
    while (j < madt_length)
    {
        entry = (MultipleAPICTableEntry *) (((u8 *)(&madt->entry[0])) + j);
        switch (entry->type)
        {
            case 0:
                proc = (MultipleAPICTableProc *) entry;
                DEBUG("APIC for core" << proc->apicId);
                m_cores.append(proc->apicId);
                break;

            case 1:
                DEBUG("I/O APIC");
                break;

            case 2:
                DEBUG("IRQ source override");
                break;
        }
        j += entry->length;
    }
    return Success;
}

IntelACPI::Result IntelACPI::discover()
{
    SystemDescriptorHeader *hdr = (SystemDescriptorHeader *) m_rootIO.getBase();
    m_cores.clear();

    // Detect the Root/ExtendedSystemTable
    if (hdr->signature == RootSystemTableSignature)
    {
        RootSystemTable *rst = (RootSystemTable *) hdr;
        Size num = (rst->header.length - sizeof(RootSystemTable)) / sizeof(u32);

        DEBUG("found " << num << " SDT entries");
        for (uint i = 0; i < num; i++)
        {
            IntelIO io;

            io.map(rst->entry[i], PAGESIZE);
            hdr = (SystemDescriptorHeader *) io.getBase();

            DEBUG("entry " << i << " : " << (void *) hdr->signature);

            if (hdr->signature == MultipleAPICTableSignature)
                scanAPIC((MultipleAPICTable *) hdr);

            io.unmap();
        }
    }
    else if (hdr->signature == ExtendedSystemTableSignature)
    {
        ExtendedSystemTable *xst = (ExtendedSystemTable *) hdr;
        Size num = (xst->header.length - sizeof(ExtendedSystemTable)) / sizeof(u64);

        DEBUG("found " << num << " SDT entries");
        for (uint i = 0; i < num; i++)
        {
            IntelIO io;

            io.map(xst->entry[i], PAGESIZE);
            hdr = (SystemDescriptorHeader *) io.getBase();

            DEBUG("entry " << i << " : " << (void *) hdr->signature);

            if (hdr->signature == MultipleAPICTableSignature)
                scanAPIC((MultipleAPICTable *) hdr);

            io.unmap();
        }
    }
    return Success;
}

IntelACPI::Result IntelACPI::boot(CoreInfo *info)
{
    return IOError;
}
