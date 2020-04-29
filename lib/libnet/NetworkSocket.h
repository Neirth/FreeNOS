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

#ifndef __LIBNET_NETWORKSOCKET_H
#define __LIBNET_NETWORKSOCKET_H

#include <File.h>
#include <Types.h>
#include "NetworkQueue.h"
#include "NetworkClient.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libnet
 * @{
 */

/**
 * Network socket represents a single logical connection on a protocol.
 */
class NetworkSocket : public File
{
  public:

    /**
     * Constructor
     *
     * @param packetSize Size of each network packet
     */
    NetworkSocket(Size packetSize);

    /**
     * Destructor
     */
    virtual ~NetworkSocket();

    /**
     * Process incoming network packet.
     *
     * @return Error code
     */
    virtual Error process(NetworkQueue::Packet *pkt) = 0;

  protected:

    /** Process which owns the socket */
    ProcessID m_pid;

    /** Receive queue */
    NetworkQueue m_receive;

    /** Transmit queue */
    NetworkQueue m_transmit;

    /** Socket connection */
    NetworkClient::SocketInfo m_info;
};

/**
 * @}
 * @}
 */

#endif /* __LIBNET_NETWORKSOCKET_H */
