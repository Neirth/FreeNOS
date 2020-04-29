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

#include "NetworkQueue.h"

NetworkQueue::NetworkQueue(Size packetSize, Size headerSize, Size queueSize)
{
    m_packetSize   = packetSize;
    m_packetHeader = headerSize;

    for (Size i = 0; i < queueSize; i++)
    {
        Packet *packet = new Packet;
        packet->size = m_packetHeader;
        packet->data = new u8[packetSize];
        m_free.insert(*packet);
    }
}

NetworkQueue::~NetworkQueue()
{
    for (Size i = 0; i < m_free.size(); i++)
    {
        Packet *p = (Packet *) m_free.get(i);
        if (p)
        {
            delete p->data;
            delete p;
        }
    }
}

void NetworkQueue::setHeaderSize(Size size)
{
    m_packetHeader = size;
}

NetworkQueue::Packet * NetworkQueue::get()
{
    for (Size i = 0; i < m_free.size(); i++)
    {
        Packet *p = (Packet *) m_free.get(i);
        if (p)
        {
            p->size = m_packetHeader;
            m_free.remove(i);
            return p;
        }
    }
    return ZERO;
}

void NetworkQueue::release(NetworkQueue::Packet *packet)
{
    packet->size = m_packetHeader;
    m_free.insert(*packet);
}

void NetworkQueue::push(NetworkQueue::Packet *packet)
{
    m_data.insert(*packet);
}

NetworkQueue::Packet * NetworkQueue::pop()
{
    for (Size i = 0; i < m_data.size(); i++)
    {
        Packet *p = (Packet *) m_data.get(i);
        if (p)
        {
            m_data.remove(i);
            return p;
        }
    }
    return ZERO;
}
