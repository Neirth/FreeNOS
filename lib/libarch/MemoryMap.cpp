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

#include "MemoryMap.h"

MemoryMap::MemoryMap()
{
}

MemoryMap::MemoryMap(const MemoryMap &map)
{
    setRange(KernelData,    map.m_regions[KernelData]);
    setRange(KernelPrivate, map.m_regions[KernelPrivate]);
    setRange(UserData,      map.m_regions[UserData]);
    setRange(UserHeap,      map.m_regions[UserHeap]);
    setRange(UserStack,     map.m_regions[UserStack]);
    setRange(UserPrivate,   map.m_regions[UserPrivate]);
    setRange(UserShare,     map.m_regions[UserShare]);
    setRange(UserArgs,      map.m_regions[UserArgs]);
}

Memory::Range MemoryMap::range(MemoryMap::Region region) const
{
    return m_regions[region];
}

void MemoryMap::setRange(Region region, Memory::Range range)
{
    m_regions[region] = range;
}
