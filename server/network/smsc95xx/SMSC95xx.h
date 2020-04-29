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

#ifndef __LIBNET_SMSC95XX_H
#define __LIBNET_SMSC95XX_H

#include <NetworkDevice.h>
#include "SMSC95xxUSB.h"

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup smsc95xx
 * @{
 */

/**
 * SMSC95xx USB-based Ethernet controller.
 */
class SMSC95xx : public NetworkDevice
{
  private:

  public:

    /**
     * Constructor
     */
    SMSC95xx(u8 deviceId, const char *usbPath, NetworkServer *server);

    /**
     * Destructor
     */
    virtual ~SMSC95xx();

    /**
     * Initialize the controller.
     *
     * @return Result code
     */
    virtual Error initialize();

    /**
     * Read ethernet address.
     *
     * @param address Ethernet address reference for output
     *
     * @return Error code
     */
    virtual Error getAddress(Ethernet::Address *address);

    /**
     * Set ethernet address
     *
     * @param address New ethernet address to set
     *
     * @return Error code
     */
    virtual Error setAddress(Ethernet::Address *address);

    /**
     * Transmit one network packet
     *
     * @param pkt Network packet buffer
     */
    virtual Error transmit(NetworkQueue::Packet *pkt);

  private:

    /** USB enabled implementation class */
    SMSC95xxUSB *m_usb;
};

/**
 * @}
 * @}
 */

#endif /* __LIBNET_SMSC95XX_H */
