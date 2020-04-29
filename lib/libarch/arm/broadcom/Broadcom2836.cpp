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

#include "Broadcom2836.h"

Broadcom2836::Broadcom2836(Size coreId)
    : m_coreId(coreId)
{
    m_io.setBase(IOBase);
}

Broadcom2836::Result Broadcom2836::initialize()
{
    return Success;
}

Broadcom2836::Result Broadcom2836::setCoreTimerIrq(Broadcom2836::Timer timer,
                                                   bool enable)
{
    u32 reg = CoreTimerRegister + (m_coreId * sizeof(u32));

    switch (timer)
    {
        case PhysicalTimer1:
            if (enable)
                m_io.set(reg, (1 << 0));
            else
                m_io.unset(reg, (1 << 0));

            return Success;

        default:
            break;
    }
    return NotFound;
}

bool Broadcom2836::getCoreTimerIrqStatus(Broadcom2836::Timer timer) const
{
    if (timer == PhysicalTimer1)
        return (m_io.read(CoreIrqRegister + (m_coreId * sizeof(u32))) & (1 << 0)) > 0;
    else
        return false;
}
