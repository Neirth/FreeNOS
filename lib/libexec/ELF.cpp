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
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <MemoryBlock.h>
#include "ELF.h"

ELF::ELF(const u8 *image, Size size)
    : ExecutableFormat(image, size)
{
}

ELF::~ELF()
{
}

ELF::Result ELF::detect(const u8 *image, Size size, ExecutableFormat **fmt)
{
    ELFHeader *header = (ELFHeader *) image;

    // Verify ELF magic bytes
    if (header->ident[ELF_INDEX_MAGIC0] == ELF_MAGIC0 &&
        header->ident[ELF_INDEX_MAGIC1] == ELF_MAGIC1 &&
        header->ident[ELF_INDEX_MAGIC2] == ELF_MAGIC2 &&
        header->ident[ELF_INDEX_MAGIC3] == ELF_MAGIC3)
    {
        // Only accept current version, 32-bit ELF executable programs
        if (header->ident[ELF_INDEX_CLASS] == ELF_CLASS_32 &&
            header->version == ELF_VERSION_CURRENT &&
            header->type    == ELF_TYPE_EXEC)
        {
            (*fmt) = new ELF(image, size);
            return Success;
        }
    }
    return InvalidFormat;
}

ELF::Result ELF::regions(ELF::Region *regions, Size *count) const
{
    ELFSegment *segments;
    ELFHeader *header = (ELFHeader *) m_image;
    Size max = *count, num = header->programHeaderEntryCount, c = 0;

    // Must be of the same sizes
    if (!(header->programHeaderEntrySize == sizeof(ELFSegment) &&
          header->programHeaderEntryCount < 16))
    {
        return InvalidFormat;
    }

    // Point to the program segments
    segments = (ELFSegment *) (m_image + header->programHeaderOffset);
    (*count) = 0;

    // Fill in the memory regions
    for (Size i = 0; c < max && i < num; i++)
    {
        // We are only interested in loadable segments
        if (segments[i].type != ELF_SEGMENT_LOAD)
            continue;

        regions[c].virt   = segments[i].virtualAddress;
        regions[c].size   = segments[i].memorySize;
        regions[c].access = Memory::User | Memory::Readable | Memory::Writable;
        regions[c].data   = new u8[segments[i].memorySize];

        // Read segment contents from file
        MemoryBlock::copy(regions[c].data, m_image + segments[i].offset,
                          segments[i].fileSize);

        // Nulify remaining space
        if (segments[i].memorySize > segments[i].fileSize)
        {
            memset(regions[c].data + segments[i].fileSize, 0,
                   segments[i].memorySize - segments[i].fileSize);
        }
        c++;
    }

    // All done
    (*count) = c;
    return Success;
}

ELF::Result ELF::entry(Address *entry) const
{
    ELFHeader *header = (ELFHeader *) m_image;
    *entry = header->entry;
    return Success;
}
