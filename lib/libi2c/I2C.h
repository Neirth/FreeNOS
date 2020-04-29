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

#ifndef __LIBI2C_I2C_H
#define __LIBI2C_I2C_H

#include <FreeNOS/System.h>
#include <Types.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libi2c
 * @{
 */

/**
 * I2C controller abstract interface.
 */
class I2C
{
  public:

    /**
     * Result codes.
     */
    enum Result
    {
        Success,
        NotFound,
        IOError,
        RangeError
    };

    /**
     * Constructor
     */
    I2C();

    /**
     * Initialize the Controller.
     *
     * @return Result code
     */
    virtual Result initialize() = 0;

    /**
     * Set Slave address.
     *
     * @param addr The 7-bit address of the I2C slave device.
     *
     * @return Result code.
     */
    virtual Result setAddress(Address addr) = 0;

    /**
     * Write data to slave.
     *
     * @param buf Buffer containing bytes to write.
     * @param size Number of bytes to write.
     *
     * @return Result code.
     */
    virtual Result write(u8 *buf, Size size) = 0;

    /**
     * Read data from slave
     *
     * @param buf Buffer for storing bytes read.
     * @param size Number of bytes to read.
     *
     * @return Result code.
     */
    virtual Result read(u8 *buf, Size size) = 0;

  protected:

    /** I/O instance */
    Arch::IO m_io;
};

/**
 * @}
 * @}
 */

#endif /* __LIBI2C_I2C_H */
