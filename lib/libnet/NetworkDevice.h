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

#ifndef __LIBNET_NETWORKDEVICE_H
#define __LIBNET_NETWORKDEVICE_H

#include <Types.h>
#include <Device.h>
#include "Ethernet.h"
#include "ARP.h"
#include "IPV4.h"
#include "ICMP.h"
#include "UDP.h"
#include "NetworkQueue.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libnet
 * @{
 */

/**
 * Network Device abstract class.
 */
class NetworkDevice : public Device
{
  public:

    /**
     * Constructor
     */
    NetworkDevice(NetworkServer *server);

    /**
     * Destructor
     */
    virtual ~NetworkDevice();

    /**
     * Initialize the NetworkDevice.
     *
     * @return Error code
     */
    virtual Error initialize();

    /**
     * Get maximum packet size
     *
     * @return Maximum packet size
     */
    const Size getMaximumPacketSize() const;

    /**
     * Read ethernet address.
     *
     * @param address Ethernet address reference for output
     *
     * @return Error code
     */
    virtual Error getAddress(Ethernet::Address *address) = 0;

    /**
     * Set ethernet address
     *
     * @param address New ethernet address to set
     * @return Error code
     */
    virtual Error setAddress(Ethernet::Address *address) = 0;

    /**
     * Get receive queue
     */
    NetworkQueue * getReceiveQueue();

    /**
     * Get transmit queue
     */
    NetworkQueue * getTransmitQueue();

    /**
     * Transmit one network packet
     *
     * @param buffer Input/Output buffer to input bytes from.
     * @param size Size of the network packet.
     *
     * @return Error code
     */
    virtual Error transmit(NetworkQueue::Packet *packet) = 0;

    /**
     * Process a received network packet.
     *
     * @param packet Network packet received by the device
     * @param offset Network packet payload offset
     *
     * @return Error code
     */
    virtual Error process(NetworkQueue::Packet *packet, Size offset = 0);

  protected:

    /** Maximum size of each packet */
    Size m_maximumPacketSize;

    NetworkQueue m_receive;

    NetworkQueue m_transmit;

    NetworkServer *m_server;

    Ethernet *m_eth;

    ARP *m_arp;

    IPV4 *m_ipv4;

    ICMP *m_icmp;

    UDP *m_udp;
};

/**
 * @}
 * @}
 */

#endif /* __LIBNET_NETWORKDEVICE_H */
