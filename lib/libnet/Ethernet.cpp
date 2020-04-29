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

#include "NetworkServer.h"
#include "NetworkDevice.h"
#include "Ethernet.h"
#include "EthernetAddress.h"
#include "ICMP.h"

Ethernet::Ethernet(NetworkServer *server, NetworkDevice *device)
    : NetworkProtocol(server, device)
{
    m_arp  = ZERO;
    m_ipv4 = ZERO;
    device->getAddress(&m_address);
}

Ethernet::~Ethernet()
{
}

Error Ethernet::initialize()
{
    m_server->registerFile(this, "/ethernet");
    m_server->registerFile(new EthernetAddress(this), "/ethernet/address");
    return ESUCCESS;
}

Error Ethernet::getAddress(Ethernet::Address *address)
{
    MemoryBlock::copy(address, &m_address, sizeof(Ethernet::Address));
    return ESUCCESS;
}

Error Ethernet::setAddress(Ethernet::Address *address)
{
    Error r = m_device->setAddress(address);
    if (r == ESUCCESS)
    {
        MemoryBlock::copy(&m_address, address, sizeof(Ethernet::Address));
    }
    return r;
}

void Ethernet::setARP(::ARP *arp)
{
    m_arp = arp;
}

void Ethernet::setIP(::IPV4 *ip)
{
    m_ipv4 = ip;
}

const String Ethernet::toString(Ethernet::Address address)
{
    String s;

    s << Number::Hex;
    s << address.addr[0] << ":"
      << address.addr[1] << ":"
      << address.addr[2] << ":"
      << address.addr[3] << ":"
      << address.addr[4] << ":"
      << address.addr[5];

    return s;
}

NetworkQueue::Packet * Ethernet::getTransmitPacket(const Ethernet::Address *destination,
                                                   Ethernet::PayloadType type)
{
    NetworkQueue::Packet *pkt = m_device->getTransmitQueue()->get();
    if (!pkt)
        return ZERO;

    Ethernet::Header *ether = (Ethernet::Header *) (pkt->data + pkt->size);
    MemoryBlock::copy(&ether->source, &m_address, sizeof(Address));
    MemoryBlock::copy(&ether->destination, destination, sizeof(Address));
    ether->type = cpu_to_be16(type);

    pkt->size += sizeof(Ethernet::Header);
    return pkt;
}

Error Ethernet::process(NetworkQueue::Packet *pkt, Size offset)
{
    DEBUG("");

    const Ethernet::Header *ether = (Ethernet::Header *) (pkt->data + offset);
    u16 type = be16_to_cpu(ether->type);

    switch (type)
    {
        case Ethernet::ARP:
            if (m_arp)
                return m_arp->process(pkt, offset + sizeof(Ethernet::Header));
            break;

        case Ethernet::IPV4:
            if (m_ipv4)
                return m_ipv4->process(pkt, offset + sizeof(Ethernet::Header));
            break;

        default:
            DEBUG("dropped unknown ethernet type: " << (int) ether->type);
            break;
    }
    return EINVAL;
}

Log & operator << (Log &log, const Ethernet::Address & addr)
{
    String str;

    str << Number::Hex;

    for (Size i = 0; i < sizeof(Ethernet::Address); i++)
        str << addr.addr[i] << ":";

    log.append(*str);

    return log;
}
