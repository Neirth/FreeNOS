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

#ifndef __LIBI2C_BROADCOMI2C_H
#define __LIBI2C_BROADCOMI2C_H

#include <FreeNOS/System.h>
#include <Types.h>
#include <arm/broadcom/BroadcomGPIO.h>
#include "I2C.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libi2c
 * @{
 */

/**
 * Broadcom (2835) I2C controller implementation.
 *
 * This class implements access to the I2C controller
 * used in the Broadcom 2835 System-on-Chip. The controller
 * features a 16-byte FIFO for data reads and writes and allows
 * the use of interrupts.
 *
 * @see BCM2835 ARM Peripherals.pdf
 * @see http://www.airspayce.com/mikem/bcm2835/
 * @see https://github.com/raspberrypi/linux/issues/254
 * @see https://www.raspberrypi.org/forums/viewtopic.php?f=44&t=13771&start=25
 * @see http://www.advamation.com/knowhow/raspberrypi/rpi-i2c-bug.html
 * @see http://lxr.free-electrons.com/source/drivers/i2c/busses/i2c-bcm2835.c
 */
class BroadcomI2C : public I2C
{
  private:

    static const uint BSC0 = 0x205000;
    static const uint BSC1 = 0x804000;
    static const uint BSC2 = 0x805000;
    static const Size FIFOSize = 16;

    /**
     * Hardware registers.
     */
    enum Registers
    {
        Control      = 0x0,
        Status       = 0x4,
        DataLength   = 0x8,
        SlaveAddress = 0xc,
        FIFO         = 0x10,
        ClockDivider = 0x14,
        DataDelay    = 0x18,
        ClockStretch = 0x1c
    };

    /**
     * Control Register Flags.
     */
    enum ControlFlags
    {
        Enable    = (1 << 15),
        RxIntr    = (1 << 10),
        TxIntr    = (1 << 9),
        Transfer  = (1 << 7),
        ClearFIFO = (1 << 5),
        ReadMode  = (1),
        WriteMode = (0)
    };

    /**
     * Status Register Flags.
     */
    enum StatusFlags
    {
        ClockStretchTimeout = (1 << 9),
        AcknowledgeError    = (1 << 8),
        RxFIFOFull          = (1 << 7),
        TxFIFOEmpty         = (1 << 6),
        RxFIFOHasData       = (1 << 5),
        TxFIFOHasData       = (1 << 4),
        RxFIFOReady         = (1 << 3),
        TxFIFOReady         = (1 << 2),
        TransferDone        = (1 << 1),
        TransferActive      = (1 << 0)
    };

  public:

    /**
     * Constructor
     */
    BroadcomI2C();

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
     * Set Slave address.
     *
     * @param addr The 7-bit address of the I2C slave device.
     *
     * @return Result code.
     */
    virtual Result setAddress(Address addr);

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

#endif /* __LIBI2C_BROADCOMI2C_H */
