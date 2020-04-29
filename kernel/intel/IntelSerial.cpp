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
#include "IntelSerial.h"

IntelSerial::IntelSerial(u16 base)
{
    m_io.setBase(base);

    // 8bit Words, no parity
    m_io.outb(LINECONTROL, 3);

    // Enable interrupts
    m_io.outb(IRQCONTROL, 1);

    // No FIFO
    m_io.outb(FIFOCONTROL, 0);

    // Data Ready, Request to Send
    m_io.outb(MODEMCONTROL, 3);

    // Set baudrate
    m_io.outb(LINECONTROL, m_io.inb(LINECONTROL) | DLAB);
    m_io.outb(DIVISORLOW,  (11500 / BAUDRATE) & 0xff);
    m_io.outb(DIVISORHIGH, (11500 / BAUDRATE) >> 8);
    m_io.outb(LINECONTROL, m_io.inb(LINECONTROL) & ~DLAB);
}

void IntelSerial::write(const char *str)
{
    // Write as much bytes as possible
    while (m_io.inb(LINESTATUS) & TXREADY && *str)
    {
        m_io.outb(TRANSMIT, *str++);
    }
}
