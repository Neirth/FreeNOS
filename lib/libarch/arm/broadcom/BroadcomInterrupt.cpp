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

#include <Log.h>
#include "BroadcomInterrupt.h"

#define INTERRUPT_BASE_ADDR        0xB000
#define INTERRUPT_BASICPEND       (INTERRUPT_BASE_ADDR+0x200)
#define INTERRUPT_IRQPEND1        (INTERRUPT_BASE_ADDR+0x204)
#define INTERRUPT_IRQPEND2        (INTERRUPT_BASE_ADDR+0x208)
#define INTERRUPT_FIQCONTROL      (INTERRUPT_BASE_ADDR+0x20C)
#define INTERRUPT_ENABLEIRQ1      (INTERRUPT_BASE_ADDR+0x210)
#define INTERRUPT_ENABLEIRQ2      (INTERRUPT_BASE_ADDR+0x214)
#define INTERRUPT_ENABLEBASICIRQ  (INTERRUPT_BASE_ADDR+0x218)
#define INTERRUPT_DISABLEIRQ1     (INTERRUPT_BASE_ADDR+0x21C)
#define INTERRUPT_DISABLEIRQ2     (INTERRUPT_BASE_ADDR+0x220)
#define INTERRUPT_DISABLEBASICIRQ (INTERRUPT_BASE_ADDR+0x224)
   #define IRQSYSTIMERC1 1
   #define IRQSYSTIMERC3 3
   #define IRQAUX        29
   #define IRQUART       57

BroadcomInterrupt::BroadcomInterrupt() : IntController()
{
    // disable all IRQ sources first, just to be "safe"
    m_io.write(INTERRUPT_DISABLEIRQ1, 0xFFFFFFFF);
    m_io.write(INTERRUPT_DISABLEIRQ2, 0xFFFFFFFF);
}

BroadcomInterrupt::Result BroadcomInterrupt::enable(uint vector)
{
    // enable the respective interrupt
    if(vector < 32)
    {
        // only 1 bits are recognized when writing to the (en/dis)able regs.
        // using |= here could be problematic since it would likely be
        // implemented as multiple instructions: at least a read, an or,
        // and a write. if we interrupted _after_ the read instruction or
        // the or instruction, and disabled certain bits in the IRQ
        // routine, the |= would write back the old state of the enable
        // bits. This would effectively be re-enabling interrupts that we
        // wanted disabled.
        m_io.write(INTERRUPT_ENABLEIRQ1, (1<<vector));
    }
    else
    {
        m_io.write(INTERRUPT_ENABLEIRQ2, (1<<(vector-32)));
    }
    return Success;
}

BroadcomInterrupt::Result BroadcomInterrupt::disable(uint vector)
{
    // disable IRQs for this device before NULL-ing the vector. otherwise,
    // we might interrupt with a NULL_VECT in the handler's address.
    // because the interrupt wasn't ACKed because we never went to the
    // handler routine, the device will continue to assert its IRQ line,
    // which will put us in a never-ending IRQ loop.
    if(vector < 32)
    {
        m_io.write(INTERRUPT_DISABLEIRQ1, (1<<vector));
    }
    else
    {
        m_io.write(INTERRUPT_DISABLEIRQ2, (1<<(vector-32)));
    }
    return Success;
}

BroadcomInterrupt::Result BroadcomInterrupt::clear(uint vector)
{
    return Success;
}

BroadcomInterrupt::Result BroadcomInterrupt::nextPending(uint & irq)
{
    for (Size i = 0; i < 64; i++)
    {
        if (isTriggered(i))
        {
            irq = i;
            return Success;
        }
    }

    return NotFound;
}

bool BroadcomInterrupt::isTriggered(u32 vector)
{
    u32 basic = m_io.read(INTERRUPT_BASICPEND);

    switch (vector)
    {
        case 9: return (basic & (1 << 11));
    }
    u32 pend1 = m_io.read(INTERRUPT_IRQPEND1);
    u32 pend2 = m_io.read(INTERRUPT_IRQPEND2);

    if (vector < 32)
        return (pend1 & (1 << vector));
    else
        return (pend2 & (1 << (vector-32)));
}
