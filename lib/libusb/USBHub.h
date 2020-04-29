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

#ifndef __LIBUSB_USBHUB_H
#define __LIBUSB_USBHUB_H

#include <Types.h>
#include "USBDevice.h"
#include "USBDescriptor.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libusb
 * @{
 */

/**
 * USB Hub driver.
 */
class USBHub : public USBDevice
{
    /**
     * USB Hub standard requests.
     */
    enum RequestType
    {
        GetStatus        = 0,
        ClearFeature     = 1,
        SetFeature       = 3,
        GetDescriptor    = 6,
        SetDescriptor    = 7,
        ClearTTBuffer    = 8,
        ResetTT          = 9,
        GetTTState       = 10,
        StopTT           = 11
    };

    /**
     * USB Hub current status and changed status format.
     */
    typedef struct Status
    {
        u16 current;
        u16 changed;
    }
    Status;

    /**
     * USB Hub Port Features.
     */
    enum PortFeature
    {
        PortConnection       = 0,
        PortEnable           = 1,
        PortSuspend          = 2,
        PortOverCurrent      = 3,
        PortReset            = 4,
        PortPower            = 8,
        PortLowSpeed         = 9,
        PortClearConnection  = 16,
        PortClearEnable      = 17,
        PortClearSuspsend    = 18,
        PortClearOverCurrent = 19,
        PortClearReset       = 20,
        PortTest             = 21,
        PortIndicator        = 22
    };

  public:

    /**
     * Constructor
     */
    USBHub(u8 deviceId, const char *usbPath = "/usb");

    /**
     * Destructor
     */
    virtual ~USBHub();

    /**
     * Initialize the hub.
     *
     * @return Result code
     */
    virtual Error initialize();

  private:

    /**
     * Set feature on a HUB port.
     */
    Error setPortFeature(u8 port, PortFeature feature);

    /**
     * Attach device on port.
     */
    Error portAttach(u8 port);

  private:

    /** Hub descriptor. */
    USBDescriptor::Hub *m_hub;
};

/**
 * @}
 * @}
 */

#endif /* __LIBUSB_USBHUB_H */
