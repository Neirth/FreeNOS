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
#include "NetworkProtocol.h"
#include "ICMP.h"
#include "ICMPFactory.h"
#include "ICMPSocket.h"
#include "IPV4.h"

ICMP::ICMP(NetworkServer *server,
           NetworkDevice *device)
    : NetworkProtocol(server, device)
{
    m_ipv4  = ZERO;
}

ICMP::~ICMP()
{
}

void ICMP::setIP(::IPV4 *ip)
{
    m_ipv4 = ip;
}

Error ICMP::initialize()
{
    DEBUG("");
    m_factory = new ICMPFactory(this);
    m_server->registerFile(this, "/icmp");
    m_server->registerFile(m_factory, "/icmp/factory");
    return ESUCCESS;
}

Error ICMP::process(NetworkQueue::Packet *pkt, Size offset)
{
    IPV4::Header *iphdr = (IPV4::Header *) (pkt->data + offset - sizeof(IPV4::Header));
    Header *hdr = (Header *) (pkt->data + offset);
    IPV4::Address source = be32_to_cpu(iphdr->source);

    DEBUG("source = " << (uint)source << " type = " << hdr->type << " code = " << hdr->code << " id = " << hdr->id);

    switch (hdr->type)
    {
        case EchoRequest: {
            DEBUG("request");
            hdr->type     = EchoReply;
            hdr->checksum = 0;
            hdr->checksum = checksum(hdr);
            return sendPacket(source, hdr);
        }
        case EchoReply: {
            DEBUG("reply");

            for (Size i = 0; i < m_sockets.size(); i++)
            {
                ICMPSocket *s = (ICMPSocket *) m_sockets.get(i);
                if (s && s->getAddress() == source)
                    s->setReply(hdr);
            }
            break;
        }
    }
    return ESUCCESS;
}

ICMPSocket * ICMP::createSocket(String & path)
{
    DEBUG("");

    ICMPSocket *sock = new ICMPSocket(this);
    if (!sock)
        return ZERO;

    int pos = m_sockets.insert(*sock);
    if (pos == -1)
    {
        delete sock;
        return ZERO;
    }
    String filepath;
    filepath << "/icmp/" << pos;

    path << m_server->getMountPath() << filepath;
    m_server->registerFile(sock, *filepath);
    return sock;
}

Error ICMP::sendPacket(IPV4::Address ip, ICMP::Header *header)
{
    DEBUG("");

    NetworkQueue::Packet *pkt;
    Error r;

    // Get a fresh IP packet
    r = m_ipv4->getTransmitPacket(
        &pkt, ip, IPV4::ICMP, sizeof(Header)
    );
    if (r != ESUCCESS)
        return r;

    // Fill payload
    MemoryBlock::copy(pkt->data + pkt->size, header, sizeof(ICMP::Header));
    pkt->size += sizeof(ICMP::Header);

    // Transmit the packet
    return m_device->transmit(pkt);
}

const u16 ICMP::checksum(Header *header)
{
    return IPV4::checksum(header, sizeof(*header));
}
