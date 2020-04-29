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
#include <Macros.h>
#include "Allocator.h"

Allocator * Allocator::m_default = ZERO;

Allocator::Allocator()
    : m_parent(ZERO)
    , m_alignment(MEMALIGN)
    , m_base(ZERO)
{
}

Allocator::~Allocator()
{
}

void Allocator::setParent(Allocator *parent)
{
    m_parent = parent;
}

Allocator * Allocator::getDefault()
{
    return m_default;
}

void Allocator::setDefault(Allocator *alloc)
{
    m_default = alloc;
}

Allocator::Result Allocator::setAlignment(Size size)
{
    if (size % MEMALIGN)
        return InvalidAlignment;

    m_alignment = size;
    return Success;
}

Allocator::Result Allocator::setBase(Address addr)
{
    if (addr % PAGESIZE)
        return InvalidAddress;

    m_base = addr;
    return Success;
}

Address Allocator::aligned(Address addr, Size boundary) const
{
    Address corrected = addr;

    if (addr % boundary)
        corrected += boundary - (addr % boundary);

    return corrected;
}
