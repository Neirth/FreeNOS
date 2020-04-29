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

#ifndef __LIBNET_IPV4_H
#define __LIBNET_IPV4_H

#include <Types.h>
#include "Ethernet.h"
#include "IPV4.h"

class ICMP;
class ARP;
class UDP;
class Ethernet;

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libnet
 * @{
 */

/**
 * Internet Protocol Version 4
 */
class IPV4 : public NetworkProtocol
{
  public:

    /**
     * IP-address
     */
    typedef u32 Address;

    /**
     * Protocol types
     */
    enum Protocol
    {
        ICMP = 1,
        IGMP = 2,
        UDP  = 17,
        TCP  = 6
    };

    /**
     * IP network packet header.
     *
     * This header is prepended to every
     * IP packet send on the actual physical link.
     */
    typedef struct Header
    {
        u8  versionIHL; /**< Version and header length (IHL) */
        u8  typeOfService;
        u16 length;
        u16 identification;
        u16 fragmentOffset;
        u8  timeToLive;
        u8  protocol;
        u16 checksum;
        Address source;
        Address destination;
    }
    PACKED Header;

    /**
     * Pseudo Header.
     *
     * This header can be used by sub-protocols for
     * checksum calculation (e.g. TCP and UDP).
     */
    typedef struct PseudoHeader
    {
        Address source;
        Address destination;
        u8      reserved;
        u8      protocol;
        u16     length;
    }
    PseudoHeader;

  public:

    /**
     * Constructor
     */
    IPV4(NetworkServer *server,
       NetworkDevice *device);

    /**
     * Destructor
     */
    virtual ~IPV4();

    /**
     * Perform initialization.
     *
     * @return Error code
     */
    virtual Error initialize();

    /**
     * Set ICMP instance
     *
     * @param icmp ICMP instance
     */
    void setICMP(::ICMP *icmp);

    void setARP(::ARP *arp);

    void setUDP(::UDP *udp);

    void setEthernet(::Ethernet *ether);

    /**
     * Get current IP address
     *
     * @param address IPV4 address buffer
     *
     * @return Error code
     */
    virtual Error getAddress(Address *address);

    /**
     * Set current IP address
     *
     * @param address IPV4 address buffer
     *
     * @return Error code
     */
    virtual Error setAddress(Address *address);

    /**
     * Convert address to string
     *
     * @param address Input IP address
     * @return Text value of the IP
     */
    static const String toString(Address address);

    /**
     * Convert string to IPV4 address.
     *
     * @param address Input textual IP address in dot notation (xxx.xxx.xxx.xxx)
     * @return IPV4::Address or ZERO if not valid
     */
    static const Address toAddress(const char *address);

    /**
     * Get a new packet for transmission
     *
     * @param destination
     *
     * @return Error code
     */
    Error getTransmitPacket(NetworkQueue::Packet **pkt,
                            Address destination,
                            Protocol type,
                            Size size);

    /**
     * Process incoming network packet.
     *
     * @return Error code
     */
    virtual Error process(NetworkQueue::Packet *pkt, Size offset);

    /**
     * Calculate IP checksum
     *
     * @param buffer Input buffer for calculating checksum
     * @param length Number of bytes in the buffer
     *
     * @return IP checksum value for the given buffer
     */
    static const u16 checksum(const void *buffer, Size length);

  private:

    /** Current IP address */
    Address m_address;

    /** ICMP instance */
    ::ICMP *m_icmp;

    /** ARP instance */
    ::ARP *m_arp;

    /** UDP instance */
    ::UDP *m_udp;

    /** Ethernet instance */
    ::Ethernet *m_ether;

    /** Packet ID for IPV4 */
    u16 m_id;
};

/**
 * @}
 * @}
 */

#endif /* __LIBNET_IPV4_H */
