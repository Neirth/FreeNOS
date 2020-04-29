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

#ifndef __LIBSPI_SPI_H
#define __LIBSPI_SPI_H

#include <FreeNOS/System.h>
#include <Types.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libspi
 * @{
 */

/**
 * SPI controller abstract interface.
 */
class SPI
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

  public:

    /**
     * Constructor
     */
    SPI();

    /**
     * Initialize the Controller.
     *
     * @return Result code
     */
    virtual Result initialize() = 0;

    /**
     * Read and Write data to/from slave.
     *
     * @param sendBuf Buffer containing bytes to write, and to read.
     * @param recvBuf Buffer for receiving bytes.
     * @param size Number of bytes to write/read.
     *
     * @return Result code.
     */
    virtual Result transfer(u8 *sendBuf, u8 *recvBuf, Size size) = 0;

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

#endif /* __LIBSPI_SPI_H */
