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
#include "PageAllocator.h"

/** Minimum size required to allocate. */
#define PAGEALLOC_MINIMUM (PAGESIZE * 2)

PageAllocator::PageAllocator(Address base, Size size)
    : Allocator()
    , m_base(base)
    , m_size(size)
    , m_allocated(PAGESIZE)
{
}

Address PageAllocator::base() const
{
    return m_base;
}

Size PageAllocator::size() const
{
    return m_size;
}

Size PageAllocator::available() const
{
    return m_size - m_allocated;
}

Allocator::Result PageAllocator::allocate(Allocator::Arguments & args)
{
    Memory::Range range;

    // Set return address
    args.address = m_base + m_allocated;

    Size bytes  = args.size > PAGEALLOC_MINIMUM ?
                  args.size : PAGEALLOC_MINIMUM;

    // Align to pagesize
    bytes = aligned(bytes, PAGESIZE);

    // Fill in the message
    range.size   = bytes;
    range.access = Memory::User | Memory::Readable | Memory::Writable;
    range.virt   = m_base + m_allocated;
    range.phys   = ZERO;
    VMCtl(SELF, Map, &range);

    // Clear the pages
    MemoryBlock::set((void *) range.virt, 0, range.size);

    // Update count
    m_allocated += range.size;

    // Success
    args.size = range.size;
    return Success;
}

Allocator::Result PageAllocator::release(Address addr)
{
    return InvalidAddress;
}
