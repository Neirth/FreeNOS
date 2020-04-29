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

#include "BitAllocator.h"

BitAllocator::BitAllocator(Memory::Range range, Size chunkSize)
    : Allocator()
    , m_array(range.size / chunkSize)
    , m_base(range.phys)
    , m_chunkSize(chunkSize)
{
}

Size BitAllocator::chunkSize() const
{
    return m_chunkSize;
}

Size BitAllocator::size() const
{
    return m_array.size() * m_chunkSize;
}

Size BitAllocator::available() const
{
    return m_array.count(false) * m_chunkSize;
}

Address BitAllocator::base() const
{
    return m_base;
}

BitArray * BitAllocator::getBitArray()
{
    return &m_array;
}

Allocator::Result BitAllocator::allocate(Allocator::Arguments & args)
{
    return allocate(args, 0);
}

Allocator::Result BitAllocator::allocate(Allocator::Arguments & args,
                                         Address allocStart)
{
    Size num = (args.size) / m_chunkSize;
    BitArray::Result result;
    Size bit;

    if ((args.size) % m_chunkSize)
        num++;

    if (!args.alignment)
        args.alignment = 1;
    else if (args.alignment % m_chunkSize)
        return InvalidAlignment;
    else
        args.alignment /= m_chunkSize;

    result = m_array.setNext(&bit, num, allocStart / m_chunkSize, args.alignment);
    if (result != BitArray::Success)
        return OutOfMemory;

    args.address = m_base + (bit * m_chunkSize);
    return Success;
}

Allocator::Result BitAllocator::allocate(Address addr)
{
    if (addr < m_base || isAllocated(addr))
        return InvalidAddress;

    m_array.set((addr - m_base) / m_chunkSize);
    return Success;
}

bool BitAllocator::isAllocated(Address addr) const
{
    if (addr < m_base)
        return false;
    else
        return m_array.isSet((addr - m_base) / m_chunkSize);
}

Allocator::Result BitAllocator::release(Address addr)
{
    if (addr < m_base)
        return InvalidAddress;

    m_array.unset((addr - m_base) / m_chunkSize);
    return Success;
}
