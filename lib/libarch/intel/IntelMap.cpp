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

#include <MemoryBlock.h>
#include "IntelMap.h"

IntelMap::IntelMap()
{
    MemoryBlock::set(m_regions, 0, sizeof(m_regions));

    m_regions[KernelData].virt    = 0;
    m_regions[KernelData].size    = GigaByte(1) - MegaByte(128);

    m_regions[KernelPrivate].virt = GigaByte(1) - MegaByte(128);
    m_regions[KernelPrivate].size = MegaByte(128);

    m_regions[UserData].virt      = 0x80000000;
    m_regions[UserData].size      = MegaByte(256);

    m_regions[UserHeap].virt      = 0xb0000000;
    m_regions[UserHeap].size      = MegaByte(256);

    m_regions[UserStack].virt     = 0xc0000000;
    m_regions[UserStack].size     = KiloByte(16);

    m_regions[UserPrivate].virt   = 0xa0000000;
    m_regions[UserPrivate].size   = MegaByte(256);

    m_regions[UserShare].virt     = 0xd0000000;
    m_regions[UserShare].size     = MegaByte(256);

    m_regions[UserArgs].virt      = 0xe0000000;
    m_regions[UserArgs].size      = KiloByte(128);
}
