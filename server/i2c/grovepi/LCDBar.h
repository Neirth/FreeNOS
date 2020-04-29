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

#ifndef __SERVER_I2C_GROVEPI_LCDBAR_H
#define __SERVER_I2C_GROVEPI_LCDBAR_H

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
 * @brief GrovePi LCD Bar
 *
 * @see https://github.com/DexterInd/GrovePi/blob/master/Firmware/Source/v1.2/grove_pi_v1_2_5/README.md
 */
class LCDBar : public Device
{
  private:

    static const uint RGBAddr  = 0x62;
    static const uint TextAddr = 0x3e;

    /**
     * LCDBar commands.
     */
    enum Command
    {
        ClearDisplay = 0x01,
        NoCursor     = 0x04,
        DisplayOn    = 0x08,
        DoubleLine   = 0x28,
        Newline      = 0xc0,
        WriteChar    = 0x40
    };

  public:

    /**
     * @brief Constructor function.
     */
    LCDBar(I2C *i2c);

    /**
     * @brief Initializes the class.
     *
     * @return Error status code.
     */
    virtual Error initialize();

    /** 
     * @brief Set LCD Bar text.
     *
     * @param buffer Buffer to save the read bytes.
     * @param size Number of bytes to read.
     * @param offset Offset in the file to read.
     *
     * @return Number of bytes on success and ZERO on failure. 
     */
    virtual Error write(IOBuffer & buffer, Size size, Size offset);

  private:

    /**
     * Set LCD color.
     */
    void setRGB(uint r, uint g, uint b);

    /**
     * Set LCD text.
     */
    void setText(const char *text, Size max);

    /**
     * Send I2C text command.
     */
    void textCommand(Command cmd);

  private:

    /** I2C controller */
    I2C *m_i2c;
};

/**
 * @}
 * @}
 */

#endif /* __TIME_TIME_H */
