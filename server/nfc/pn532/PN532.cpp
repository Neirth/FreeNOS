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
#include <stdio.h>
#include <string.h>
#include "PN532.h"

PN532::PN532(SPI *spi)
    : Device(CharacterDeviceFile)
{
    m_spi = spi;
    m_identifier << "pn532";
}

Error PN532::initialize()
{
    u8 packet[255], rxPacket[255];
    FrameHeader *header = (FrameHeader *) packet;
    FrameFooter *footer = 0;
    u8 *data = (u8 *)(packet + sizeof(FrameHeader));
    Size dataSz = 1;
    Size sz = sizeof(FrameHeader) + sizeof(FrameFooter) + dataSz;

    header->preamble = 0;
    header->start[0] = 0;
    header->start[1] = 0;
    header->length   = 2;
    header->lengthChecksum = 256 - (header->length+1);
    header->frameIdent = HostToPN;
    data[0] = GetFirmwareVersion;

    footer->postamble = 0;
    footer->dataChecksum = 256 - 0xd4;
    for (Size i = 0; i < dataSz; i++)
        footer->dataChecksum -= data[i];


    DEBUG("check: " << header->length + header->lengthChecksum);

    m_spi->transfer(packet, rxPacket, sz);
    DEBUG("read back: ");

    memset(packet, 0, sizeof(packet));
    memset(rxPacket, 0, sizeof(rxPacket));
    m_spi->transfer(packet, rxPacket, sz + 4);
    DEBUG("done");

    return ESUCCESS;
}

Error PN532::read(IOBuffer & buffer, Size size, Size offset)
{
    if (offset >= 1)
        return 0;

    return 0;
}
