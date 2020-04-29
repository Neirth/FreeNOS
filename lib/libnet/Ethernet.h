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

#ifndef __LIBNET_ETHERNET_H
#define __LIBNET_ETHERNET_H

#include <Types.h>
#include <Macros.h>
#include "NetworkProtocol.h"

class ARP;
class IPV4;
class ICMP;

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libnet
 * @{
 */

/**
 * Ethernet networking protocol
 */
class Ethernet : public NetworkProtocol
{
  public:

    /** Size of the CRC checksum (which is set after the payload) */
    static const Size CRCSize = 4;

    /**
     * Ethernet network address
     */
    typedef struct Address
    {
        u8 addr[6];
    }
    PACKED Address;

    /**
     * Ethernet network packet header.
     *
     * This header is prepended to every
     * ethernet packet send on the actual physical link.
     */
    typedef struct Header
    {
        Address destination;    /**< packet destination address */
        Address source;         /**< packet source address */
        u16 type;               /**< payload type */
    }
    PACKED Header;

    /**
     * List of ethernet payload types.
     */
    enum PayloadType
    {
        IPV4 = 0x0800u,         /**< Internet protocol v4 */
        IPV6 = 0x86ddu,         /**< Internet protocol v6 */
        ARP  = 0x0806u          /**< Address resolution protocol */
    };

  public:

    /**
     * Constructor
     */
    Ethernet(NetworkServer *server, NetworkDevice *device);

    /**
     * Destructor
     */
    virtual ~Ethernet();

    /**
     * Perform initialization.
     */
    virtual Error initialize();

    /**
     * Retrieve Ethernet address
     *
     * @param address Output buffer to store the address
     *
     * @return Error code
     */
    virtual Error getAddress(Address *address);

    /**
     * Set Ethernet address
     *
     * @param address Input address
     *
     * @return Error code
     */
    virtual Error setAddress(Address *address);

    /**
     * Get a new packet for transmission
     *
     * @param destination
     */
    NetworkQueue::Packet *getTransmitPacket(const Address *destination,
                                            PayloadType type);


    /**
     * Convert address to string
     *
     * @param address Input ethernet address
     * @return Text value of the ethernet address
     */
    static const String toString(Address address);

    /**
     * Set ARP instance
     *
     * @param arp ARP object instance
     */
    void setARP(::ARP *arp);

    /**
     * Set IPV4 instance
     *
     * @param ip IPV4 instance
     */
    void setIP(::IPV4 *ip);

    /**
     * Process incoming network packet.
     *
     * @return Error code
     */
    virtual Error process(NetworkQueue::Packet *pkt, Size offset);

  private:

    /** Current ethernet address */
    Address m_address;

    /** ARP protocol */
    ::ARP *m_arp;

    /** IPV4 protocol */
    ::IPV4 *m_ipv4;
};

Log & operator << (Log &log, const Ethernet::Address & addr);

/**
 * @}
 * @}
 */

#endif /* __LIBNET_ETHERNET_H */
