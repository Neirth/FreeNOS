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

#include "NetworkClient.h"
#include "NetworkServer.h"
#include "NetworkDevice.h"
#include "ARP.h"
#include "ARPSocket.h"

ARP::ARP(NetworkServer *server, NetworkDevice *device)
    : NetworkProtocol(server, device)
{
    m_ip = 0;
    m_ether = 0;
}

ARP::~ARP()
{
}

Error ARP::initialize()
{
    m_sock = new ARPSocket(this);
    m_server->registerFile(this, "/arp");
    m_server->registerFile(m_sock, "/arp/socket");
    return ESUCCESS;
}

void ARP::setIP(::IPV4 *ip)
{
    m_ip = ip;
}

void ARP::setEthernet(::Ethernet *eth)
{
    m_ether = eth;
}

ARP::ARPCache * ARP::getCacheEntry(IPV4::Address ipAddr)
{
    ARPCache **entry = (ARPCache **) m_cache.get(ipAddr);
    if (entry)
        return (*entry);
    else
        return insertCacheEntry(ipAddr);
}

ARP::ARPCache * ARP::insertCacheEntry(IPV4::Address ipAddr)
{
    ARPCache *entry = new ARPCache;
    MemoryBlock::set(entry, 0, sizeof(*entry));
    entry->valid  = false;

    // Insert to the ARP cache
    m_cache.insert(ipAddr, entry);
    return entry;
}

void ARP::updateCacheEntry(IPV4::Address ipAddr,
                           Ethernet::Address ethAddr)
{
    ARPCache *entry = getCacheEntry(ipAddr);
    if (entry)
    {
        entry->valid = true;
        MemoryBlock::copy(&entry->ethAddr, &ethAddr, sizeof(Ethernet::Address));
    }
}

Error ARP::lookupAddress(IPV4::Address *ipAddr,
                         Ethernet::Address *ethAddr)
{
    DEBUG("");

    // See if we have the IP cached
    ARPCache *entry = getCacheEntry(*ipAddr);
    if (entry && entry->valid)
    {
        MemoryBlock::copy(ethAddr, &entry->ethAddr, sizeof(Ethernet::Address));
        return ESUCCESS;
    }
    // Send an ARP request
    Error r = sendRequest(*ipAddr);
    if (r < 0)
        return r;

    // Make sure we are called again in about 500msec
    m_server->setTimeout(500);
    return EAGAIN;
}

Error ARP::sendRequest(IPV4::Address address)
{
    DEBUG("");

    // Get cache entry
    ARPCache *entry = getCacheEntry(address);
    if (!entry)
        return EHOSTUNREACH;

    // Update the cache entry administration
    entry->valid = false;
    entry->retryCount++;
    if (entry->retryCount > MaxRetries)
    {
        entry->retryCount = 0;
        return EHOSTUNREACH;
    }

    // Destination is broadcast ethernet address
    Ethernet::Address destAddr;
    MemoryBlock::set(&destAddr, 0xff, sizeof(destAddr));

    // Get a fresh ethernet packet
    NetworkQueue::Packet *pkt = m_ether->getTransmitPacket(
        &destAddr,
        Ethernet::ARP
    );
    if (!pkt)
        return EAGAIN;

    Ethernet::Header *ether = (Ethernet::Header *) (pkt->data + pkt->size - sizeof(Ethernet::Header));
    ARP::Header *arp = (ARP::Header *) (pkt->data + pkt->size);
    pkt->size += sizeof(ARP::Header);

    // Fill the ARP packet
    arp->hardwareType   = cpu_to_be16(ARP::Ethernet);
    arp->protocolType   = cpu_to_be16(ARP::IPV4);
    arp->hardwareLength = sizeof(Ethernet::Address);
    arp->protocolLength = sizeof(IPV4::Address);
    arp->operation      = cpu_to_be16(ARP::Request);

    // Get our current IP
    IPV4::Address ipaddr;
    m_ip->getAddress(&ipaddr);

    // Fill source and destinations
    MemoryBlock::copy(&arp->etherSender, &ether->source, sizeof(Ethernet::Address));
    arp->ipSender = cpu_to_be32(ipaddr);
    MemoryBlock::copy(&arp->etherTarget, &ether->destination, sizeof(Ethernet::Address));
    arp->ipTarget = cpu_to_be32(address);

    // Send the packet using the network device
    return m_device->transmit(pkt);
}

Error ARP::sendReply(const Ethernet::Address *ethAddr, const IPV4::Address ipAddr)
{
    DEBUG("");

    // Get a fresh ethernet packet
    NetworkQueue::Packet *pkt = m_ether->getTransmitPacket(
        ethAddr,
        Ethernet::ARP
    );
    if (!pkt)
        return EAGAIN;

    if (!m_ip)
        return EINVAL;

    IPV4::Address myip;
    m_ip->getAddress(&myip);

    // Fill the ARP packet
    Ethernet::Header *ether = (Ethernet::Header *)(pkt->data + pkt->size - sizeof(Ethernet::Header));
    ARP::Header *arp = (ARP::Header *)(pkt->data + pkt->size);
    pkt->size += sizeof(ARP::Header);

    DEBUG("eth: source=" << ether->source << " dest=" << ether->destination);

    // ARP packet
    arp->hardwareType   = cpu_to_be16(ARP::Ethernet);
    arp->protocolType   = cpu_to_be16(ARP::IPV4);
    arp->hardwareLength = sizeof(Ethernet::Address);
    arp->protocolLength = sizeof(IPV4::Address);
    arp->operation      = cpu_to_be16(ARP::Reply);

    // Fill source and destinations
    MemoryBlock::copy(&arp->etherSender, &ether->source, sizeof(Ethernet::Address));
    arp->ipSender = cpu_to_be32(myip);
    MemoryBlock::copy(&arp->etherTarget, &ether->destination, sizeof(Ethernet::Address));
    arp->ipTarget = ipAddr;

    // Send the packet using the network device
    return m_device->transmit(pkt);
}

Error ARP::process(NetworkQueue::Packet *pkt, Size offset)
{
    DEBUG("");

    const Ethernet::Header *ether = (Ethernet::Header *) (pkt->data + offset - sizeof(Ethernet::Header));
    const Header *arp = (Header *) (pkt->data + offset);
    IPV4::Address ipAddr;

    if (!m_ip)
        return EINVAL;

    m_ip->getAddress(&ipAddr);

    switch (be16_to_cpu(arp->operation))
    {
        case Request:
            // Only send reply if the request is for our IP
            if (be32_to_cpu(arp->ipTarget) == ipAddr)
            {
                updateCacheEntry(be32_to_cpu(arp->ipSender),
                                 arp->etherSender);
                return sendReply(&ether->source, arp->ipSender);
            }
            break;

        case Reply: {
            updateCacheEntry(be32_to_cpu(arp->ipSender),
                             arp->etherSender);
            return m_sock->process(pkt);
        }
    }
    // Unknown ARP operation
    return ENOTSUP;
}
