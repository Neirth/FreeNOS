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

#ifndef __LIBUSB_USBMESSAGE_H
#define __LIBUSB_USBMESSAGE_H

#include "USBTransfer.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libusb
 * @{
 */

/**
 * USB Message implementation.
 *
 * USBMessages are communicated between the
 * USBController and USBDevice classes.
 */
typedef struct USBMessage
{
    USBTransfer::Setup       setup;
    USBTransfer::Direction   direction;
    USBTransfer::Speed       speed;
    USBTransfer::Type        type;
    USBTransfer::RequestType requestType;

    Address deviceId;    /**< USB device identifier number */
    Address endpointId;  /**< USB endpoint identifier number */
    Address hubAddress;  /**< USB hub address */
    Address portAddress; /**< USB port address */
    Size maxPacketSize;  /**< Maximum packet size */
    u8 packetId;         /**< PacketId to use for non-control transfers */

    Address buffer;     /**< I/O buffer for sending/receiving payload data */
    Size size;          /**< Number of bytes to transfer. */

    /**
     * State of the USBMessage request.
     */
    enum State
    {
        Setup,
        Data,
        Status,
        Success,
        Failure
    }
    state;
}
USBMessage;

/**
 * @}
 * @}
 */

#endif /* __LIBUSB_USBMESSAGE_H */
