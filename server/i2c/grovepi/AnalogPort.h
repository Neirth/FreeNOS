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

#ifndef __SERVER_I2C_GROVEPI_ANALOGPORT_H
#define __SERVER_I2C_GROVEPI_ANALOGPORT_H

#include <Macros.h>
#include <Types.h>
#include <Device.h>
#include <I2C.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup grovepi
 * @{
 */

/**
 * @brief GrovePi Analog Port
 *
 * @see https://github.com/DexterInd/GrovePi/blob/master/Firmware/Source/v1.2/grove_pi_v1_2_5/README.md
 */
class AnalogPort : public Device
{
  public:

    /**
     * @brief Constructor function.
     */
    AnalogPort(I2C *i2c, uint port);

    /**
     * @brief Initializes the time class.
     *
     * @return Error status code.
     */
    virtual Error initialize();

    /**
     * @brief Read digital port value.
     *
     * @param buffer Buffer to save the read bytes.
     * @param size Number of bytes to read.
     * @param offset Offset in the file to read.
     *
     * @return Number of bytes on success and ZERO on failure.
     */
    virtual Error read(IOBuffer & buffer, Size size, Size offset);

  private:

    /** I2C controller */
    I2C *m_i2c;

    /** Analog port number. */
    uint m_port;
};

/**
 * @}
 * @}
 */

#endif /* __SERVER_I2C_GROVEPI_ANALOGPORT_H */
