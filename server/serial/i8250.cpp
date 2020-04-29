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
#include <FreeNOS/Config.h>
#include <Macros.h>
#include <Types.h>
#include <string.h>
#include "i8250.h"

i8250::i8250(u16 b, u16 q)
    : Device(CharacterDeviceFile), base(b), irq(q)
{
    m_identifier << "serial0";
}

Error i8250::initialize()
{
    // 8bit Words, no parity
    WriteByte(base + LINECONTROL, 3);

    // Enable interrupts
    WriteByte(base + IRQCONTROL, 1);

    // No FIFO
    WriteByte(base + FIFOCONTROL, 0);

    // Data Ready, Request to Send
    WriteByte(base + MODEMCONTROL, 3);

    // Set baudrate
    WriteByte(base + LINECONTROL, ReadByte(base + LINECONTROL) | DLAB);
    WriteByte(base + DIVISORLOW,  (11500 / BAUDRATE) & 0xff);
    WriteByte(base + DIVISORHIGH, (11500 / BAUDRATE) >> 8);
    WriteByte(base + LINECONTROL, ReadByte(base + LINECONTROL) & ~(DLAB));

    INFO("i8250 initialized");

    // Done
    return ESUCCESS;
}

Error i8250::interrupt(Size vector)
{
    ProcessCtl(SELF, EnableIRQ, irq);
    return ESUCCESS;
}

Error i8250::read(IOBuffer & buffer, Size size, Size offset)
{
    Size bytes = 0;
    u8 byte;

    // Read as much bytes as possible
    while (ReadByte(base + LINESTATUS) & RXREADY && bytes < size)
    {
        byte = ReadByte(base);
        buffer.bufferedWrite(&byte, 1);
        bytes++;
    }
    return bytes ? (Error) bytes : EAGAIN;
}

Error i8250::write(IOBuffer & buffer, Size size, Size offset)
{
    Size bytes = 0;

    // Write as much bytes as possible
    while (ReadByte(base + LINESTATUS) & TXREADY && bytes < size)
    {
        WriteByte(base, buffer[bytes++]);
    }
    return bytes ? (Error) bytes : EAGAIN;
}
