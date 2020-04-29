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

#ifndef __LIBUSB_USBTRANSFERFILE_H
#define __LIBUSB_USBTRANSFERFILE_H

#include <File.h>
#include "USBController.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libusb
 * @{
 */

/**
 * USB transfer file for USBController (/usb/transfer)
 */
class USBTransferFile : public File
{
  public:

    /**
     * Constructor
     */
    USBTransferFile(USBController *controller);

    /**
     * Write bytes to the file.
     *
     * @param buffer Input/Output buffer to input bytes from.
     * @param size Number of bytes to write, at maximum.
     * @param offset Offset inside the file to start writing.
     *
     * @return Number of bytes written on success, Error on failure.
     */
    virtual Error write(IOBuffer & buffer, Size size, Size offset);

  private:

    /** Pointer to the USB controller for doing actual transfers */
    USBController *m_controller;
};

/**
 * @}
 * @}
 */

#endif /* __LIBUSB_USBTRANSFERFILE_H */
