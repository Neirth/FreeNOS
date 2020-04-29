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

#include "IntelPIT.h"

IntelPIT::IntelPIT()
    : Timer()
{
    m_int = InterruptNumber;
}

uint IntelPIT::getCounter()
{
    uint count = 0;

    setControl(LatchedRead | Channel0);
    count  = m_io.inb(Channel0Data);
    count |= m_io.inb(Channel0Data) << 8;

    return count;
}

IntelPIT::Result IntelPIT::setFrequency(uint hertz)
{
    uint divisor;

    // Frequency must be non-zero and cannot exceed the oscillator
    if (hertz == 0 || hertz > OscillatorFreq)
        return InvalidFrequency;

    // Frequency must be within bounds of the 16-bit counter
    divisor = OscillatorFreq / hertz;
    if (divisor <= 2 || divisor > 0xffff)
        return InvalidFrequency;

    // Let the i8254 timer run continuously
    setControl(RateGenerator | Channel0 | AccessLowHigh);
    m_io.outb(Channel0Data, divisor & 0xff);
    m_io.outb(Channel0Data, (divisor >> 8) & 0xff);
    m_frequency = hertz;
    return Success;
}

IntelPIT::Result IntelPIT::waitTrigger()
{
    uint previous, current;

    // Wait until the 16-bit counter restarts
    // at its initial counter value.
    current = getCounter();
    do
    {
        previous = current;
        current  = getCounter();
    }
    while (previous >= current);
 
    // Now at the trigger moment.
    return Success;
}

IntelPIT::Result IntelPIT::setControl(IntelPIT::ControlFlags flags)
{
    m_io.outb(Control, flags);
    return Success;
}
