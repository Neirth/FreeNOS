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
#include "BroadcomSPI.h"

BroadcomSPI::BroadcomSPI()
{
}

BroadcomSPI::Result BroadcomSPI::initialize()
{
    // Map I2C registers
    if (m_io.map(IO_BASE + SPI0, PAGESIZE,
                 Memory::User|Memory::Readable|Memory::Writable|Memory::Device) != IO::Success)
        return IOError;

    // Initialize GPIO controller
    if (m_gpio.initialize() != BroadcomGPIO::Success)
        return IOError;

    // Set GPIO pins to ALT0 function for CLK, MOSI, MISO, CS pins
    m_gpio.setAltFunction(8,  BroadcomGPIO::Function0); // CS0
    m_gpio.setAltFunction(9,  BroadcomGPIO::Function0); // MISO
    m_gpio.setAltFunction(10, BroadcomGPIO::Function0); // MOSI
    m_gpio.setAltFunction(11, BroadcomGPIO::Function0); // CLK

    // Reset registers and clear FIFO
    m_io.write(Control, 0);
    m_io.write(Control, ClearFIFO);
    return Success;
}

BroadcomSPI::Result BroadcomSPI::setClockDivider(Size divider)
{
    m_io.write(ClockDivider, divider);
    return Success;
}

BroadcomSPI::Result BroadcomSPI::transfer(u8 *sendBuf, u8 *recvBuf, Size size)
{
    uint rx = 0, tx = 0;
    DEBUG("transfer of " << size << " bytes");

    // Set modes
    m_io.write(Control, 0);

    // Clear FIFOs
    m_io.write(Control, ClearFIFO);

    // Start transfer
    m_io.set(Control, TransferActive);

    // Keep transfering until we reach the length needed
    while (rx < size || tx < size)
    {
        // Append to tx FIFO
        while (m_io.read(Control) & TxHasSpace && tx < size)
        {
            DEBUG("tx[" << tx << "] = " << sendBuf[tx]);
            m_io.write(FIFO, sendBuf[tx++]);
        }

        // Pull from rx FIFO
        while (m_io.read(Control) & RxHasData && rx < size)
        {
            recvBuf[rx] = m_io.read(FIFO);
            DEBUG("rx[" << rx << "] = " << recvBuf[rx]);
            rx++;
        }
    }
    DEBUG("waiting for done");

    // Wait for DONE signal
    while (m_io.read(Control) & TransferDone);

    return Success;
}

BroadcomSPI::Result BroadcomSPI::write(u8 *buf, Size size)
{
    return Success;
}

BroadcomSPI::Result BroadcomSPI::read(u8 *buf, Size size)
{
    return Success;
}
