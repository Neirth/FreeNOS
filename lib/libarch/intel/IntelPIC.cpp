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

#include "IntelPIC.h"

IntelPIC::IntelPIC()
    : IntController()
{
    m_base = InterruptBase;
    m_master.setBase(MasterBase);
    m_slave.setBase(SlaveBase);
}

IntelPIC::Result IntelPIC::initialize()
{
    // ICW1: Initialize PIC's (Edge triggered, Cascade)
    m_master.outb(Command, Init1 | CascadeMode | LevelTriggered);
    m_slave.outb(Command, Init1 | CascadeMode | LevelTriggered);

    // ICW2: Remap IRQ's to interrupts 32-47
    m_master.outb(Data, InterruptBase);
    m_slave.outb(Data, InterruptBase + 8);

    // ICW3: PIC2 is connected to PIC1 via IRQ2
    m_master.outb(Data, 0x4);
    m_slave.outb(Data, 0x2);

    // ICW4: 8086 mode, fully nested, not buffered, no implicit EOI
    m_master.outb(Data, Mode8086);
    m_slave.outb(Data, Mode8086);

    // OCW1: Disable all IRQ's
    m_master.outb(Data, 0xff);
    m_slave.outb(Data, 0xff);

    // Make sure to enable PIC2's interrupt in the Master (IRQ2)
    enable(2);
    return Success;
}

IntelPIC::Result IntelPIC::enable(uint irq)
{
    if (irq > 15)
        return InvalidIRQ;

    if (irq < 8)
        m_master.outb(Data, m_master.inb(Data) & ~(1 << irq));
    else
        m_slave.outb(Data, m_slave.inb(Data) & ~(1 << (irq - 8)));

    return Success;
}

IntelPIC::Result IntelPIC::disable(uint irq)
{
    if (irq > 15)
        return InvalidIRQ;

    if (irq < 8)
        m_master.outb(Data, m_master.inb(Data) | (1 << irq));
    else
        m_slave.outb(Data, m_slave.inb(Data) | (1 << (irq - 8)));

    return Success;
}

IntelPIC::Result IntelPIC::clear(uint irq)
{
    if (irq > 15)
        return InvalidIRQ;

    // End-Of-Interrupt to slave needed?
    if (irq >= 8)
        m_slave.outb(Command, EndOfInterrupt);

    // Always signal End-Of-Interrupt to the master
    m_master.outb(Command, EndOfInterrupt);
    return Success;
}
