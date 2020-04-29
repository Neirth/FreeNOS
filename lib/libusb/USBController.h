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

#ifndef __LIBUSB_USBCONTROLLER_H
#define __LIBUSB_USBCONTROLLER_H

#include <FreeNOS/System.h>
#include <Types.h>
#include <DeviceServer.h>
#include <FileSystemMessage.h>
#include "USBMessage.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libusb
 * @{
 */

/**
 * USB controller abstract interface.
 */
class USBController : public DeviceServer
{
  public:

    /**
     * Constructor
     */
    USBController(const char *path);

    /**
     * Initialize the Controller.
     *
     * @return Result code
     */
    virtual Error initialize();

    /**
     * Submit USB transfer.
     *
     * @return Result code
     */
    virtual Error transfer(const FileSystemMessage *msg,
                           USBMessage *usb) = 0;

  protected:

    /** I/O instance */
    Arch::IO m_io;
};

/**
 * @}
 * @}
 */

#endif /* __LIBUSB_USBCONTROLLER_H */
