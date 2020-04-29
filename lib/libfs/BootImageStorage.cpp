/*
 * Copyright (C) 2009 Niek Linnenbank
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
#include <string.h>
#include "BootImageStorage.h"

BootImageStorage::BootImageStorage(const char *name)
{
    m_name   = name;
    m_data   = ZERO;
    m_size   = 0;
}

bool BootImageStorage::load()
{
    SystemInformation info;
    BootImage *image;
    BootSymbol *symbol;
    BootSegment *segment;
    Memory::Range range;
    u8 *base;

    // Request boot image memory
    range.size   = info.bootImageSize;
    range.access = Memory::User |
                   Memory::Readable;
    range.virt   = ZERO;
    range.phys   = info.bootImageAddress;
    VMCtl(SELF, Map, &range);

    // Update our state
    image = (BootImage *) range.virt;
    base  = (u8 *) image;

    // Search for the given BootSymbol
    for (uint i = 0; i < image->symbolTableCount; i++)
    {
        symbol = (BootSymbol *) (base + image->symbolTableOffset);
        symbol += i;

        if (strcmp(symbol->name, m_name) == 0)
        {
            m_size   = symbol->segmentsTotalSize;
            segment = (BootSegment *) (base + image->segmentsTableOffset +
                                      (symbol->segmentsOffset * sizeof(BootSegment)));
            m_data   = base + segment->offset;
            // Success
            return true;
        }
    }
    // BootSymbol not found
    return false;
}

Error BootImageStorage::read(u64 offset, void *buffer, Size size)
{
    memcpy(buffer, m_data + offset, size);
    return size;
}

u64 BootImageStorage::capacity() const
{
    return m_size;
}
