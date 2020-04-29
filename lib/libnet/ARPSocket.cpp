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
#include "Ethernet.h"
#include "IPV4.h"
#include "ARP.h"
#include "ARPSocket.h"

ARPSocket::ARPSocket(ARP *arp)
    : NetworkSocket(arp->getMaximumPacketSize())
{
    m_arp = arp;
    m_ipAddr = 0;
    MemoryBlock::set(&m_ethAddr, 0, sizeof(Ethernet::Address));
}

ARPSocket::~ARPSocket()
{
}

Error ARPSocket::read(IOBuffer & buffer, Size size, Size offset)
{
    DEBUG("");

    if (size != sizeof(Ethernet::Address))
        return ERANGE;

    if (offset >= sizeof(Ethernet::Address))
        return 0;

    Error r = m_arp->lookupAddress(&m_ipAddr, &m_ethAddr);
    if (r != ESUCCESS)
        return r;

    buffer.write(&m_ethAddr, sizeof(Ethernet::Address));
    return sizeof(Ethernet::Address);
}

Error ARPSocket::write(IOBuffer & buffer, Size size, Size offset)
{
    DEBUG("");

    // Save the request address
    buffer.read(&m_ipAddr, sizeof(IPV4::Address));

    // Send request
    // return m_arp->sendRequest(m_ipAddr);
    Error r = m_arp->lookupAddress(&m_ipAddr, &m_ethAddr);
    if (r != ESUCCESS)
        return r;
    else
        return size;
}

Error ARPSocket::process(NetworkQueue::Packet *pkt)
{
    DEBUG("");
    return ESUCCESS;
}

void ARPSocket::error(Error err)
{
    DEBUG("");
}
