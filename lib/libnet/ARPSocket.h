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

#ifndef __LIBNET_ARPSOCKET_H
#define __LIBNET_ARPSOCKET_H

#include "IPV4.h"
#include "NetworkSocket.h"
#include "NetworkQueue.h"

class ARP;

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libnet
 * @{
 */

/**
 * Address Resolution Protocol (ARP) socket.
 *
 * ARP sockets receive a copy of ARP reply packets.
 */
class ARPSocket : public NetworkSocket
{
  public:

    /**
     * Constructor
     */
    ARPSocket(ARP *arp);

    /**
     * Destructor
     */
    virtual ~ARPSocket();

    /**
     * Receive ARP response
     *
     * @param buffer Input/Output buffer to output bytes to.
     * @param size Number of bytes to read, at maximum.
     * @param offset Offset inside the file to start reading.
     *
     * @return Number of bytes read on success, Error on failure.
     */
    virtual Error read(IOBuffer & buffer, Size size, Size offset);

    /**
     * Send ARP request
     *
     * @param buffer Input/Output buffer to input bytes from.
     * @param size Number of bytes to write, at maximum.
     * @param offset Offset inside the file to start writing.
     *
     * @return Number of bytes written on success, Error on failure.
     */
    virtual Error write(IOBuffer & buffer, Size size, Size offset);

    /**
     * Process incoming network packet.
     *
     * @return Error code
     */
    virtual Error process(NetworkQueue::Packet *pkt);

    /**
     * Set error status
     *
     * @param err Error code
     */
    virtual void error(Error err);

  private:

    /** ARP protocol instance */
    ARP *m_arp;

    /** IPV4 address for request. */
    IPV4::Address m_ipAddr;

    /** Ethernet address for reply. */
    Ethernet::Address m_ethAddr;
};

/**
 * @}
 * @}
 */

#endif /* __LIBNET_ARPSOCKET_H */
