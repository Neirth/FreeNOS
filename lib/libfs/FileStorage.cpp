/*
 * Copyright (C) 2009 Niek Linnenbank
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

#include <Types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include "FileStorage.h"

FileStorage::FileStorage(const char *path, Size offset)
{
    m_file   = open(path, O_RDWR);
    m_offset = offset;
    stat(path, &m_stat);
}

FileStorage::~FileStorage()
{
    close(m_file);
}

Error FileStorage::read(u64 offset, void *buffer, Size size)
{
    int result;

    if (m_file >= 0)
    {
        lseek(m_file, m_offset + offset, SEEK_SET);
        result = ::read(m_file, buffer, size);

        return result >= 0 ? result : errno;
    }
    else
        return errno;
}

Error FileStorage::write(u64 offset, void *buffer, Size size)
{
    int result;

    if (m_file >= 0)
    {
        lseek(m_file, m_offset + offset, SEEK_SET);
        result = ::write(m_file, buffer, size);

        return result >= 0 ? result : errno;
    }
    else
        return errno;
}

u64 FileStorage::capacity() const
{
    return m_stat.st_size;
}
