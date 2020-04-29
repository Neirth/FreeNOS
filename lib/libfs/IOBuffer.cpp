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

#include "IOBuffer.h"

IOBuffer::IOBuffer(const FileSystemMessage *msg)
    : m_message(msg)
{
    if (msg->action == ReadFile || msg->action == WriteFile)
        m_buffer = new u8[msg->size];
    else
        m_buffer = 0;

    m_size   = msg->size;
    m_count  = 0;
}

IOBuffer::~IOBuffer()
{
    if (m_buffer)
        delete m_buffer;
}

Size IOBuffer::getCount() const
{
    return m_count;
}

const FileSystemMessage * IOBuffer::getMessage() const
{
    return m_message;
}

const u8 * IOBuffer::getBuffer() const
{
    return m_buffer;
}

Error IOBuffer::bufferedRead()
{
    m_count = read(m_buffer, m_message->size, 0);
    return m_count;
}

Error IOBuffer::bufferedWrite(const void *buffer, Size size)
{
    Size i = 0;

    if (!m_buffer)
        m_buffer = new u8[m_message->size];

    for (i = 0; i < size && m_count < m_size; i++)
    {
        m_buffer[m_count++] = ((u8 *)buffer)[i];
    }
    return i;
}

Error IOBuffer::read(void *buffer, Size size, Size offset) const
{
    return VMCopy(m_message->from, API::Read,
                 (Address) buffer,
                 (Address) m_message->buffer + offset, size);
}

Error IOBuffer::write(void *buffer, Size size, Size offset) const
{
    return VMCopy(m_message->from, API::Write,
                 (Address) buffer,
                 (Address) m_message->buffer + offset, size);
}

Error IOBuffer::flush() const
{
    return write(m_buffer, m_count, 0);
}

u8 IOBuffer::operator[](Size index) const
{
    return index < m_size ? m_buffer[index] : 0;
}
