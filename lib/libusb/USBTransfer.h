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

#ifndef __LIBUSB_USBTRANSFER_H
#define __LIBUSB_USBTRANSFER_H

#include <Types.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libusb
 * @{
 */

/**
 * USB 2.0 standard transfers.
 */
namespace USBTransfer
{
    /**
     * Transfer direction.
     */
    enum Direction
    {
        Out = 0,
        In  = 1
    };

    /**
     * Transfer speeds.
     */
    enum Speed
    {
        High = 0,
        Full = 1,
        Low  = 2
    };

    /**
     * Various types of transfers.
     */
    enum Type
    {
        Control     = 0,
        Isochronous = 1,
        Bulk        = 2,
        Interrupt   = 3,
    };

    /**
     * Transfer sizes.
     */
    enum Size
    {
        Size8  = 0,
        Size16 = 1,
        Size32 = 2,
        Size64 = 3
    };

    /**
     * USB Standard Device Requests.
     */
    enum DeviceRequest
    {
        GetStatus        = 0,
        ClearFeature     = 1,
        SetFeature       = 3,
        SetAddress       = 5,
        GetDescriptor    = 6,
        SetDescriptor    = 7,
        GetConfiguration = 8,
        SetConfiguration = 9,
        GetInterface     = 10,
        SetInterface     = 11,
        SyncFrame        = 12
    };

    /**
     * USB Request Types
     */
    enum RequestType
    {
        Standard = 0,
        Class    = 1,
        Vendor   = 2,
        Reserved = 3
    };

    /**
     * Specify receiver of the transfer.
     */
    enum Recipient
    {
        Device    = 0,
        Interface = 1,
        Endpoint  = 2,
        Other     = 3
    };

    /**
     * SETUP phase data for control transfers.
     *
     * @see USB 2.0 Specification, Section 9.3, Table 9-2
     */
    typedef struct Setup
    {
        u8  requestType;
        u8  request;
        u16 value;
        u16 index;
        u16 length;
    }
    PACKED Setup;
};

/**
 * @}
 * @}
 */

#endif /* __LIBUSB_USBDEVICE_H */
