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
#include "SplitAllocator.h"

SplitAllocator::SplitAllocator(Memory::Range low, Memory::Range high)
    : Allocator()
    , m_low(low)
    , m_high(high)
{
    Memory::Range mem = low;
    mem.size += high.size;

    m_alloc = new BitAllocator(mem, PAGESIZE);
}

SplitAllocator::~SplitAllocator()
{
    delete m_alloc;
}

Size SplitAllocator::size() const
{
    return m_alloc->size();
}

Size SplitAllocator::available() const
{
    return m_alloc->available();
}

Allocator::Result SplitAllocator::allocate(Allocator::Arguments & args)
{
    Allocator::Result r;

    if ((r = allocateLow(args)) == Success)
        return r;
    else
        return allocateHigh(args);
}

Allocator::Result SplitAllocator::allocate(Address addr)
{
    return m_alloc->allocate(addr);
}

Allocator::Result SplitAllocator::allocateLow(Allocator::Arguments & args)
{
    return m_alloc->allocate(args, 0);
}

Allocator::Result SplitAllocator::allocateHigh(Allocator::Arguments & args)
{
    return m_alloc->allocate(args, m_high.phys - m_alloc->base());
}

Allocator::Result SplitAllocator::release(Address addr)
{
    return m_alloc->release(addr);
}

#ifdef ARM
void * SplitAllocator::toVirtual(Address phys) const
{
    return (void *)phys;
}

void * SplitAllocator::toPhysical(Address virt) const
{
    return (void *)virt;
}
#else
void * SplitAllocator::toVirtual(Address phys) const
{
    return (void *) (phys - m_low.phys);
}

void * SplitAllocator::toPhysical(Address virt) const
{
    return (void *) (virt + m_low.phys);
}
#endif /* ARM */
