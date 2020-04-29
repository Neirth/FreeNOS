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

#ifndef __LIBUSB_USBPORT_H
#define __LIBUSB_USBPORT_H

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libusb
 * @{
 */

namespace USBPort
{
    /**
     * Features which can be (un)set on a USB port.
     */
    enum Feature
    {
        Connection       = 0,
        Enable           = 1,
        Suspend          = 2,
        OverCurrent      = 3,
        Reset            = 4,
        Power            = 5,
        LowSpeed         = 6,
        ClearConnection  = 16,
        ClearEnable      = 17,
        ClearSuspend     = 18,
        ClearOverCurrent = 19,
        ClearReset       = 20,
        Test             = 21,
        Indicator        = 22
    };

    /**
     * USB Port status and status changed format.
     */
    typedef struct Status
    {
        u16 current;
        u16 changed;
    }
    Status;
};

/**
 * @}
 * @}
 */

#endif /* __LIBUSB_USBPORT_H */
