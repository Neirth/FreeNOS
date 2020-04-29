/*
 * Copyright (C) 2019 Niek Linnenbank
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
#include "NS16550.h"

NS16550::NS16550(u32 irq)
    : Device(CharacterDeviceFile)
{
    m_irq = irq;
    m_identifier << "serial0";
}

Error NS16550::initialize()
{
    if (!isKernel)
    {
        // Remap IO base to ensure we have user-level access to the registers.
        if (m_io.map(UART_BASE, PAGESIZE*2,
                     Memory::User | Memory::Readable | Memory::Writable | Memory::Device)
            != IO::Success)
        {
            return EINVAL;
        }

        // Disable receiving interrupts
        ProcessCtl(SELF, DisableIRQ, m_irq);
    }
    else
    {
        m_io.setBase(UART_BASE);
    }

    // Set integer & fractional part of baud rate.
    setDivisorLatch(true);
    m_io.write(DivisorLatchLow, 13);
    m_io.write(DivisorLatchHigh, 0x00);
    setDivisorLatch(false);

    // Use 8 bit data transmission, 1 stop bit, no parity
    m_io.write(LineControl, LineControl8Bits);

    if (isKernel)
    {
        // Mask all interrupts.
        // TODO: overlap: m_io.write(InterruptIdentity, 0);
        m_io.write(InterruptEnable, 0);
    }
    else
    {
        // Enable Rx/Tx interrupts and FIFOs
        m_io.write(FifoControl, FifoControlTrigger1 | FifoControlEnable);
        // TODO: they overlap? Perhaps DLAB must be used here too?
        // m_io.write(InterruptIdentity, InterruptIdentityFifoEnable);
        m_io.write(InterruptEnable, ReceiveDataInterrupt);
        ProcessCtl(SELF, EnableIRQ, m_irq);
    }
    return 0;
}

Error NS16550::interrupt(u32 vector)
{
    // Mask interrupt until FIFOs are empty
    m_io.write(InterruptEnable, 0);
    return ESUCCESS;
}

Error NS16550::read(IOBuffer & buffer, Size size, Size offset)
{
    Size bytes = 0;

    // Read as much bytes as possible
    while ((m_io.read(LineStatus) & LineStatusDataReady) && bytes < size)
    {
        u8 byte = m_io.read(ReceiveBuffer);
        buffer.bufferedWrite(&byte, 1);
        bytes++;
    }

    // Re-enable interrupts
    if (!isKernel)
    {
        if (!(m_io.read(InterruptEnable) & ReceiveDataInterrupt))
        {
            m_io.write(InterruptEnable, ReceiveDataInterrupt);
            ProcessCtl(SELF, EnableIRQ, m_irq);
        }
    }
    return buffer.getCount() ? (Error) buffer.getCount() : EAGAIN;
}

Error NS16550::write(IOBuffer & buffer, Size size, Size offset)
{
    Size bytes = 0;

    // Write as much bytes as possible
    while (bytes < size)
    {
        // Wait until TX fifo is empty
        // TODO: optimize later
        while (!(m_io.read(LineStatus) & LineStatusTxEmpty))
        {
            ;
        }

        m_io.write(TransmitHolding, buffer[bytes++]);
    }
    return bytes ? (Error) bytes : EAGAIN;
}

void NS16550::delay(s32 count) const
{
    asm volatile("1: subs %0, %0, #1; bne 1b"
         : "=r"(count) : "0"(count));
}

void NS16550::setDivisorLatch(bool enabled)
{
    // Must wait until the busy flag is cleared
    //while (m_io.read(UartStatus) & UartStatusBusy)
    //{
    //    ;
    //}

    // Set the divisor latch register
    u32 lc = m_io.read(LineControl);

    if (enabled)
        lc |= LineControlDivisorLatch;
    else
        lc &= ~LineControlDivisorLatch;

    m_io.write(LineControl, lc);
}
