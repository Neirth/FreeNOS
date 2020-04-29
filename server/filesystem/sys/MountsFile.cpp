/*
 * Copyright (C) 2019 Niek Linnenbank
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

#include <FileSystemMount.h>
#include <Runtime.h>
#include "MountsFile.h"

MountsFile::MountsFile() : File(RegularFile)
{
    m_access = OwnerRW;
    m_size = sizeof(FileSystemMount) * FILESYSTEM_MAXMOUNTS;
}

MountsFile::~MountsFile()
{
}

Error MountsFile::read(IOBuffer & buffer, Size size, Size offset)
{
    FileSystemMount *mounts = getMounts();

    // Bounds checking
    if (offset > 0 || size < m_size)
        return EIO;

    // Copy the entire mounts table
    return buffer.write(mounts, m_size);
}

Error MountsFile::write(IOBuffer & buffer, Size size, Size offset)
{
    FileSystemMount fs;
    FileSystemMount *mounts = getMounts();
    Error r;

    // Input must be exactly one FileSystemMount struct
    if (size != sizeof(fs))
        return EIO;

    // Copy the input mount struct
    if ((r = buffer.read(&fs, sizeof(fs))) <= 0)
        return r;

    // Append to our filesystem mounts table
    for (Size i = 0; i < FILESYSTEM_MAXMOUNTS; i++)
    {
        if (!mounts[i].path[0])
        {
            memcpy((void *)&mounts[i], &fs, sizeof(fs));
            NOTICE("mounted " << mounts[i].path);
            return size;
        }
    }

    // Mounts table is full
    return ENOBUFS;
}
