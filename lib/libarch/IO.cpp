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
#include "MemoryContext.h"
#include "Core.h"
#include "IO.h"

IO::IO()
{
    m_base = 0;
}

uint IO::getBase() const
{
    return m_base;
}

void IO::setBase(uint base)
{
    m_base = base;
}

IO::Result IO::map(Address phys, Size size, Memory::Access access)
{
    m_range.virt   = 0;
    m_range.phys   = phys;
    m_range.access = access;
    m_range.size   = size;

    if (!isKernel)
    {
        if (VMCtl(SELF, Map, &m_range) != API::Success)
            return MapFailure;
    }
    else
    {
        m_range.access &= ~Memory::User;

        MemoryContext *ctx = MemoryContext::getCurrent();
        if (!ctx)
            return MapFailure;

        if (ctx->findFree(size, MemoryMap::KernelPrivate, &m_range.virt) != MemoryContext::Success)
            return OutOfMemory;

        if (ctx->map(m_range.virt, phys, m_range.access) != MemoryContext::Success)
            return MapFailure;
    }
    m_base = m_range.virt;
    return Success;
}

IO::Result IO::unmap()
{
    if (!isKernel)
    {
        if (VMCtl(SELF, UnMap, &m_range) != API::Success)
            return MapFailure;
    }
    else
    {
        MemoryContext *ctx = MemoryContext::getCurrent();
        if (!ctx)
            return MapFailure;

        if (ctx->unmapRange(&m_range) != MemoryContext::Success)
            return MapFailure;
    }
    return Success;
}
