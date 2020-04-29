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

#include <FreeNOS/System.h>
#include <String.h>
#include <List.h>
#include <ListIterator.h>
#include "NetworkServer.h"
#include "NetworkDevice.h"
#include "IPV4.h"
#include "IPV4Address.h"
#include "UDP.h"
#include "ICMP.h"

IPV4::IPV4(NetworkServer *server,
           NetworkDevice *device)
    : NetworkProtocol(server, device)
{
    m_address = 0;
    m_icmp = 0;
    m_udp = 0;
    m_ether = 0;
    m_id = 1;
}

IPV4::~IPV4()
{
}

Error IPV4::initialize()
{
    DEBUG("");
    m_server->registerFile(this, "/ipv4");
    m_server->registerFile(new IPV4Address(this), "/ipv4/address");
    return ESUCCESS;
}

void IPV4::setICMP(::ICMP *icmp)
{
    m_icmp = icmp;
}

void IPV4::setEthernet(::Ethernet *ether)
{
    m_ether = ether;
}

void IPV4::setARP(::ARP *arp)
{
    m_arp = arp;
}

void IPV4::setUDP(::UDP *udp)
{
    m_udp = udp;
}

Error IPV4::getAddress(IPV4::Address *address)
{
    *address = m_address;
    return ESUCCESS;
}

Error IPV4::setAddress(IPV4::Address *address)
{
    m_address = *address;
    return ESUCCESS;
}

const String IPV4::toString(Address address)
{
    String s;

    s << ((address >> 24) & 0xff) << "."
      << ((address >> 16) & 0xff) << "."
      << ((address >> 8)  & 0xff) << "."
      << ((address & 0xff));

    return s;
}

const IPV4::Address IPV4::toAddress(const char *address)
{
    String input = address;
    List<String> lst = input.split('.');
    Size shift = 24;
    IPV4::Address addr = 0;

    // The address must be 4 bytes
    if (lst.count() != 4)
        return ZERO;

    // Extract bytes in dot format (xxx.xxx.xxx.xxx)
    for (ListIterator<String> i(lst); i.hasCurrent(); i++)
    {
        String & s = i.current();
        u8 byte = s.toLong();
        addr |= (byte << shift);
        shift -= 8;
    }
    // Done
    return addr;
}

Error IPV4::getTransmitPacket(NetworkQueue::Packet **pkt,
                              IPV4::Address destination,
                              IPV4::Protocol type,
                              Size size)
{
    Ethernet::Address ethAddr;
    Error r;

    // Find the ethernet address using ARP first
    if ((r = m_arp->lookupAddress(&destination, &ethAddr)) != ESUCCESS)
        return r;

    // Get a fresh ethernet packet
    *pkt = m_ether->getTransmitPacket(
        &ethAddr,
        Ethernet::IPV4
    );
    // Fill IP header
    Header *hdr = (Header *) ((*pkt)->data + (*pkt)->size);
    hdr->versionIHL     = (sizeof(Header) / sizeof(u32)) | (4 << 4);
    hdr->typeOfService  = 0;
    hdr->length         = cpu_to_be16(size + sizeof(Header));
    hdr->identification = cpu_to_be16(m_id);
    hdr->fragmentOffset = cpu_to_be16(0x4000); // dont fragment flag
    hdr->timeToLive     = 64;
    hdr->protocol       = type;
    hdr->source         = cpu_to_be32(m_address);
    hdr->destination    = cpu_to_be32(destination);
    hdr->checksum       = 0;
    hdr->checksum       = checksum(hdr, sizeof(Header));
    (*pkt)->size += sizeof(Header);
    m_id++;

    // Success
    return ESUCCESS;
}

const u16 IPV4::checksum(const void *buffer, Size len)
{
    const u16 *ptr = (const u16 *) buffer;
    uint sum = 0;

    // Calculate sum of the buffer
    while (len > 1)
    {
        sum += *ptr;
        ptr++;
        len -= 2;
    }

    // Add left-over byte, if any
    if (len > 0)
        sum += be16_to_cpu(*((u8 *)ptr) << 8);

    // Enforce 16-bit checksum
    while (sum >> 16)
        sum = (sum >> 16) + (sum & 0xFFFF);

    // Convert to one's complement
    return (~sum);
}

Error IPV4::process(NetworkQueue::Packet *pkt, Size offset)
{
    DEBUG("");

    Header *hdr = (Header *) (pkt->data + offset);

    if (be32_to_cpu(hdr->destination) != m_address)
    {
        DEBUG("dropped packet");
        return ERANGE;
    }
    switch (hdr->protocol)
    {
        case ICMP:
            m_icmp->process(pkt, offset + sizeof(Header));
            break;

        case UDP:
            m_udp->process(pkt, offset + sizeof(Header));
            break;

        default:
            break;
    }
    return EINVAL;
}
