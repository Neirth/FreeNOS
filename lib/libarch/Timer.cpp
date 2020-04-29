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
#include "Timer.h"

Timer::Timer()
{
    m_frequency = 0;
    m_int       = 0;
    MemoryBlock::set(&m_info, 0, sizeof(m_info));
}

Size Timer::getInterrupt() const
{
    return m_int;
}

Size Timer::getFrequency() const
{
    return m_frequency;
}

Timer::Result Timer::setFrequency(Size hertz)
{
    m_frequency = hertz;
    return Success;
}

Timer::Result Timer::getCurrent(Info *info)
{
    info->frequency = m_frequency;
    info->ticks     = m_info.ticks;
    return Success;
}

Timer::Result Timer::initialize()
{
    return Success;
}

Timer::Result Timer::start()
{
    return Success;
}

Timer::Result Timer::stop()
{
    return Success;
}

Timer::Result Timer::tick()
{
    m_info.ticks++;
    return Success;
}

Timer::Result Timer::wait(u32 microseconds) const
{
    return Success;
}

bool Timer::isExpired(const Timer::Info & info) const
{
    if (!info.frequency)
        return false;

    return m_info.ticks > info.ticks;
}
