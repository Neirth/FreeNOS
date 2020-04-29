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

#ifndef __SERIAL_I8250_H
#define __SERIAL_I8250_H

#include <Macros.h>
#include <Types.h>
#include <Device.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup serial
 * @{
 */

/**
 * i8250 serial UART.
 */
class i8250 : public Device
{
  public:

    /**
     * Constants used to communicate with the UART.
     */
    enum Constants
    {
        TRANSMIT     = 0,
        RECEIVE      = 0,
        DIVISORLOW   = 0,
        DIVISORHIGH  = 1,
        RXREADY      = 1,
        IRQCONTROL   = 1,
        IRQSTATUS    = 2,
        FIFOCONTROL  = 2,
        LINECONTROL  = 3,
        MODEMCONTROL = 4,
        LINESTATUS   = 5,
        TXREADY      = 0x20,
        DLAB         = 0x80,
        BAUDRATE     = 9600,
    };

    /**
     * Constructor function.
     *
     * @param base I/O base port.
     */
    i8250(u16 base, u16 irq);

    /**
     * @brief Initializes the i8250 serial UART.
     *
     * @return Error status code.
     */
    virtual Error initialize();

    /**
     * Called when an interrupt has been triggered for this device.
     *
     * @param vector Vector number of the interrupt.
     *
     * @return Error result code.
     */
    virtual Error interrupt(Size vector);

    /**
     * Read bytes from the i8250.
     *
     * @param buffer Buffer to save the read bytes.
     * @param size Number of bytes to read.
     * @param offset Unused.
     *
     * @return Number of bytes on success and ZERO on failure.
     */
    virtual Error read(IOBuffer & buffer, Size size, Size offset);

    /**
     * Write bytes to the device.
     *
     * @param buffer Buffer containing bytes to write.
     * @param size Number of bytes to write.
     * @param offset Unused.
     *
     * @return Number of bytes on success and ZERO on failure.
     */
    virtual Error write(IOBuffer & buffer, Size size, Size offset);

  private:

    /** Base I/O port. */
    u16 base;

    /** Interrupt vector. */
    u16 irq;
};

/**
 * @}
 * @}
 */

#endif /* __SERIAL_I8250_H */
