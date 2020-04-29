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

#ifndef _INTEL_SERIAL_H
#define _INTEL_SERIAL_H

#include <Log.h>

/** Forward declare */
class IntelIO;

/**
 * @addtogroup kernel
 * @{
 *
 * @addtogroup kernel_intel
 * @{
 */

/**
 * Intel Serial port debug log class.
 *
 * @note This class is a singleton
 */
class IntelSerial : public Log
{
  public:

    /**
     * Constructor.
     *
     * @param base I/O base to use
     */
    IntelSerial(u16 base);

  protected:

    /**
     * Write to the actual output device.
     */
    virtual void write(const char *str);

  private:

    /**
     * Constants used to communicate with the UART.
     */
    enum IntelSerialReg
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

  private:

    /** I/O instance */
    IntelIO m_io;
};

/**
 * @}
 * @}
 */

#endif /* _INTEL_SERIAL_H */
