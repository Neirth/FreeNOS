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

#ifndef __LIBUSB_USB_H
#define __LIBUSB_USB_H

#include <Types.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libusb
 * @{
 */

/**
 * USB 2.0 definitions.
 */
namespace USB
{
    /**
     * USB class codes.
     */
    enum Class
    {
        InterfaceSpecific = 0x00,
        Audio             = 0x01,
        CommAndCDCControl = 0x02,
        HID               = 0x03,
        Image             = 0x06,
        Printer           = 0x07,
        MassStorage       = 0x08,
        Hub               = 0x09,
        Video             = 0x0e,
        WirelessControl   = 0xe0,
        Misc              = 0xef,
        VendorSpecific    = 0xff
    };
};

/**
 * @}
 * @}
 */

#endif /* __LIBUSB_USB_H */
