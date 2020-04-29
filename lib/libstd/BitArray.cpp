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

#include "BitArray.h"
#include "MemoryBlock.h"

BitArray::BitArray(Size size, u8 *array)
{
    m_array = array ? array : new u8[BITS_TO_BYTES(size)];
    m_allocated = array == ZERO;
    m_size  = size;
    m_set   = 0;
    clear();
}

BitArray::~BitArray()
{
    if (m_allocated)
        delete[] m_array;
}

Size BitArray::size() const
{
    return m_size;
}

Size BitArray::count(bool on) const
{
    return on ? m_set : m_size - m_set;
}

void BitArray::set(Size bit, bool value)
{
    assertRead(m_array);
    assertWrite(m_array);

    // Check if the bit is inside the array
    if (bit >= m_size)
        return;

    // Check current value
    bool current = m_array[bit / 8] & (1 << (bit % 8));

    // Update the bit only if needed (and update administration)
    if (current != value)
    {
        if (value)
        {
            m_array[bit / 8] |= 1 << (bit % 8);
            m_set++;
        }
        else
        {
            m_array[bit / 8] &= ~(1 << (bit % 8));
            m_set--;
        }
    }
}

void BitArray::unset(Size bit)
{
    set(bit, false);
}

bool BitArray::isSet(Size bit) const
{
    assert(bit < m_size);
    assertRead(m_array);

    return m_array[bit / 8] & (1 << (bit % 8));
}

void BitArray::setRange(Size from, Size to)
{
    for (Size i = from; i <= to; i++)
        set(i, true);
}

BitArray::Result BitArray::setNext(Size *bit, Size count, Size start, Size boundary)
{
    Size from = 0, found = 0;

    // Loop BitArray for unset bits
    for (Size i = start; i < m_size; i++)
    {
        if (!isSet(i))
        {
            // Remember this bit
            if (!found)
            {
                if (i % boundary)
                    continue;
                from  = i;
                found = 1;
            }
            else
                found++;

            // Are there enough contigious bits?
            if (found >= count)
            {
                setRange(from, i);
                *bit = from;
                return Success;
            }
        }
        else
        {
            from = found = 0;
        }
    }
    // No unset bits left!
    return OutOfMemory;
}

u8 * BitArray::array() const
{
    return m_array;
}

void BitArray::setArray(u8 *map, Size size)
{
    // Set bits count
    if (size)
        m_size = size;

    // Cleanup old array, if needed
    if (m_array && m_allocated)
        delete[] m_array;

    // Reassign to the new map
    m_array = map;
    m_allocated = false;
    m_set   = 0;

    // Recalculate set bits
    for (Size i = 0; i < m_size; i++)
        if (isSet(i))
            m_set++;
}

void BitArray::clear()
{
    // Zero it
    MemoryBlock::set(m_array, 0, BITS_TO_BYTES(m_size));

    // Reset set count
    m_set = 0;
}

bool BitArray::operator[](Size bit) const
{
    return isSet(bit);
}

bool BitArray::operator[](int bit) const
{
    return isSet(bit);
}
