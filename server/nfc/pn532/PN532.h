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

#ifndef __SERVER_NFC_PN532_H
#define __SERVER_NFC_PN532_H

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup pn532
 * @{
 */

#include <Macros.h>
#include <Types.h>
#include <Device.h>
#include <SPI.h>

/**
 * @brief PN532 NFC driver
 */
class PN532 : public Device
{
  private:

    enum FrameIdent
    {
        HostToPN = 0xd4,
        PnToHost = 0xd5
    };

    enum Commands
    {
        GetFirmwareVersion = 0x02
    };

    typedef struct FrameHeader
    {
        u8 preamble;
        u8 start[2];
        u8 length;
        u8 lengthChecksum;
        u8 frameIdent;
    }
    FrameHeader;

    typedef struct FrameFooter
    {
        u8 dataChecksum;
        u8 postamble;
    }
    FrameFooter;

  public:

    /**
     * @brief Constructor function.
     */
    PN532(SPI *spi);

    /**
     * @brief Initializes the class.
     *
     * @return Error status code.
     */
    virtual Error initialize();

    /**
     * @brief Read NFC device ID.
     *
     * @param buffer Buffer to save the read bytes.
     * @param size Number of bytes to read.
     * @param offset Offset in the file to read.
     *
     * @return Number of bytes on success and ZERO on failure. 
     */
    virtual Error read(IOBuffer & buffer, Size size, Size offset);

  private:

    /** SPI controller */
    SPI *m_spi;
};

/**
 * @}
 * @}
 */

#endif /* __TIME_TIME_H */
