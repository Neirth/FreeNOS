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

#ifndef __LIBSPI_BROADCOMSPI_H
#define __LIBSPI_BROADCOMSPI_H

#include <FreeNOS/System.h>
#include <Types.h>
#include <arm/broadcom/BroadcomGPIO.h>
#include "SPI.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libspi
 * @{
 */

/**
 * Broadcom (2835) SPI controller implementation.
 */
class BroadcomSPI : public SPI
{
  private:

    static const uint SPI0 = 0x204000;

    /**
     * Hardware registers.
     */
    enum Registers
    {
        Control      = 0x0,
        FIFO         = 0x4,
        ClockDivider = 0x8,
        LoSSI        = 0x10,
        DMACtl       = 0x14
    };

    /**
     * Control Register Flags.
     */
    enum ControlFlags
    {
        ChipSelect0  = (0),
        ChipSelect1  = (1 << 0),
        ChipSelect2  = (1 << 1),
        ClockPolHigh = (1 << 3),
        ClearFIFO    = 0x30,
        ClearFIFORx  = 0x20,
        ClearFIFOTx  = 0x10,
        ChipSelActiveHigh = (1 << 6),
        TransferActive    = (1 << 7),
        DMAEnable    = (1 << 8),
        ChipSelAutoDeassert = (1 << 11),
        ReadEnable   = (1 << 12),
        TransferDone = (1 << 16),
        RxHasData    = (1 << 17),
        TxHasSpace   = (1 << 18),
        RxNeedRead   = (1 << 19),
        RxIsFull     = (1 << 20),
        ChipSelect0ActiveHigh = (1 << 21),
        ChipSelect1ActiveHigh = (1 << 22),
        ChipSelect2ActiveHigh = (1 << 23)
    };

  public:

    /**
     * Constructor
     */
    BroadcomSPI();

    /**
     * Initialize the Controller.
     *
     * @return Result code
     */
    virtual Result initialize();

    /**
     * Set clock divider.
     *
     * @param divider Clock divider to use.
     *
     * @return Result code.
     */
    virtual Result setClockDivider(Size divider);

    /**
     * Read and Write data to/from slave.
     *
     * @param sendBuf Buffer containing bytes to write, and to read.
     * @param recvBuf Buffer for receiving bytes.
     * @param size Number of bytes to write/read.
     *
     * @return Result code.
     */
    virtual Result transfer(u8 *sendBuf, u8 *recvBuf, Size size);

    /**
     * Write data to slave.
     *
     * @param buf Buffer containing bytes to write.
     * @param size Number of bytes to write.
     *
     * @return Result code.
     */
    virtual Result write(u8 *buf, Size size);

    /**
     * Read data from slave
     *
     * @param buf Buffer for storing bytes read.
     * @param size Number of bytes to read.
     *
     * @return Result code.
     */
    virtual Result read(u8 *buf, Size size);

  private:

    /** GPIO controller. */
    BroadcomGPIO m_gpio;
};

/**
 * @}
 * @}
 */

#endif /* __LIBI2C_BROADCOMSPI_H */
