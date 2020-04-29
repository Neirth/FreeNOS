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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "PseudoFile.h"

PseudoFile::PseudoFile()
    : File(RegularFile)
{
    m_size   = ZERO;
    m_buffer = ZERO;
    m_access = OwnerRW;
}

PseudoFile::PseudoFile(const char *str)
{
    m_access = OwnerRW;
    m_size   = strlen(str);
    m_buffer = new char[m_size + 1];
    strlcpy(m_buffer, str, m_size + 1);
}


PseudoFile::PseudoFile(const char *format, ...)
{
    va_list args;

    // Allocate buffer
    m_buffer = new char[512];

    // Format the input
    va_start(args, format);
    m_size = vsnprintf(m_buffer, 512, format, args);
    va_end(args);

    // Set members
    m_access = OwnerRW;
}

PseudoFile::~PseudoFile()
{
    if (m_buffer)
        delete m_buffer;
}

Error PseudoFile::read(IOBuffer & buffer, Size size, Size offset)
{
    // Bounds checking
    if (offset >= m_size)
        return 0;

    // How much bytes to copy?
    Size bytes = m_size - offset > size ? size : m_size - offset;

    // Copy the buffers
    return buffer.write(m_buffer + offset, bytes);
}

Error PseudoFile::write(IOBuffer & buffer, Size size, Size offset)
{
    // Check for the buffer size
    if (!m_buffer || m_size < (size + offset))
    {
        // Allocate a new buffer and copy the old data
        char *new_buffer = new char[size+offset];
        memset(new_buffer, 0, sizeof(size+offset));

        // Inherit from the old buffer, if needed
        if (m_buffer)
        {
            memcpy(new_buffer, m_buffer, m_size);
            delete m_buffer;
        }
        // Assign buffer
        m_buffer = new_buffer;
        m_size = size+offset;
    }
    // Copy the input data in the current buffer
    buffer.read(m_buffer + offset, size);
    return size;
}
