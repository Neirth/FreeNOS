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
#include "ICMP.h"
#include "ICMPSocket.h"
#include "NetworkClient.h"

ICMPSocket::ICMPSocket(ICMP *icmp)
    : NetworkSocket(icmp->getMaximumPacketSize())
{
    m_icmp = icmp;
    m_gotReply = false;
    MemoryBlock::set(&m_info, 0, sizeof(m_info));
}

ICMPSocket::~ICMPSocket()
{
}

const IPV4::Address ICMPSocket::getAddress() const
{
    return m_info.address;
}

Error ICMPSocket::read(IOBuffer & buffer, Size size, Size offset)
{
    DEBUG("");

    if (!m_gotReply)
        return EAGAIN;

    m_gotReply = false;
    buffer.write(&m_reply, sizeof(m_reply));
    return sizeof(m_reply);
}

Error ICMPSocket::write(IOBuffer & buffer, Size size, Size offset)
{
    DEBUG("");

    // Receive socket information first?
    if (!m_info.address)
    {
        buffer.read(&m_info, sizeof(m_info));
        return size;
    }
    else
    {
        ICMP::Header header;
        buffer.read(&header, sizeof(header));

        Error r = m_icmp->sendPacket(m_info.address, &header);
        if (r != ESUCCESS)
            return r;
    }
    return size;
}

Error ICMPSocket::process(NetworkQueue::Packet *pkt)
{
    DEBUG("");
    return ESUCCESS;
}

void ICMPSocket::error(Error err)
{
    DEBUG("");
}

void ICMPSocket::setReply(ICMP::Header *header)
{
    DEBUG("");

    if (!m_gotReply)
    {
        MemoryBlock::copy(&m_reply, header, sizeof(ICMP::Header));
        m_gotReply = true;
    }
}
