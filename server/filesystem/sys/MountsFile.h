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

#ifndef __LIB_LIBFS_MOUNTSFILE_H
#define __LIB_LIBFS_MOUNTSFILE_H

#include <File.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup sysfs
 * @{
 */

/**
 * The filesystem mounts file exports the FileSystemMount table.
 *
 * To retrieve the currently mounted filesystems, a process simply reads this entire file.
 * To mount a new file system, a process writes a new 'FileSystemMount' structure to this file.
 *
 * @see FileSystem
 */
class MountsFile : public File
{
  public:

    /**
     * Constructor function.
     */
    MountsFile();

    /**
     * Destructor function.
     */
    virtual ~MountsFile();

    /**
     * @brief Read bytes from the file.
     *
     * @param buffer Input/Output buffer to output bytes to.
     * @param size Number of bytes to read, at maximum.
     * @param offset Offset inside the file to start reading.
     *
     * @return Number of bytes read on success, Error on failure.
     */
    virtual Error read(IOBuffer & buffer, Size size, Size offset);

    /**
     * Write bytes to the file.
     *
     * @param buffer Input/Output buffer to input bytes from.
     * @param size Number of bytes to write, at maximum.
     * @param offset Offset inside the file to start writing.
     *
     * @return Number of bytes written on success, Error on failure.
     */
    virtual Error write(IOBuffer & buffer, Size size, Size offset);
};

/**
 * @}
 * @}
 */

#endif /* __LIB_LIBFS_MOUNTSFILE_H */
