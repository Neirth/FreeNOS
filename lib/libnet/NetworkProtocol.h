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

#ifndef __LIBNET_NETWORKPROTOCOL_H
#define __LIBNET_NETWORKPROTOCOL_H

#include <Directory.h>
#include <Types.h>
#include "NetworkQueue.h"

class NetworkServer;
class NetworkDevice;

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libnet
 * @{
 */

/**
 * Network protocol abstraction class.
 */
class NetworkProtocol : public Directory
{
  public:

    /**
     * Constructor
     */
    NetworkProtocol(NetworkServer *server,
                    NetworkDevice *device);

    /**
     * Destructor
     */
    virtual ~NetworkProtocol();


    /**
     * Get maximum packet size.
     *
     * @return Packet size in bytes
     */
    virtual const Size getMaximumPacketSize() const;

    /**
     * Perform initialization.
     *
     * @return Error code
     */
    virtual Error initialize() = 0;

    /**
     * Process incoming network packet.
     *
     * @return Error code
     */
    virtual Error process(NetworkQueue::Packet *pkt, Size offset) = 0;

  protected:

    /** Network server instance */
    NetworkServer *m_server;

    /** Network device instance */
    NetworkDevice *m_device;
};

/**
 * @}
 * @}
 */

#endif /* __LIBNET_NETWORKPROTOCOL_H */
