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
#include "ARMException.h"
#include "ARMCore.h"

extern Address vecTable[], handlerTable;

ARMException::ARMException(Address base)
    : m_vecTable(base)
{
    MemoryBlock::copy((void *)m_vecTable, vecTable, ARM_EX_VECTAB_SIZE);
    vbar_set(base);
}

ARMException::Result ARMException::install(
    ARMException::ExceptionType vector,
    ARMException::Handler handler)
{
    ((Address *) &handlerTable)[vector] = (Address) handler;
    return Success;
}
