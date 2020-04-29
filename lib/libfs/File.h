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

#ifndef __LIB_LIBFS_FILE_H
#define __LIB_LIBFS_FILE_H

#include <FreeNOS/System.h>
#include <FreeNOS/API.h>
#include <Types.h>
#include "FileSystemMessage.h"
#include "FileType.h"
#include "FileMode.h"
#include "IOBuffer.h"
#include <errno.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/**
 * Represents a file present on a FileSystem.
 *
 * @see FileSystem
 */
class File
{
  public:

    /**
     * Constructor function.
     *
     * @param type Type of file.
     * @param uid User identity.
     * @param gid Group identity.
     */
    File(FileType type = RegularFile, UserID uid = ZERO, GroupID gid = ZERO);

    /**
     * Destructor function.
     */
    virtual ~File();

    /**
     * Retrieve our filetype.
     *
     * @return FileType object.
     */
    FileType getType() const;

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

    /**
     * Retrieve file statistics.
     *
     * @param st Buffer to write statistics to.
     *
     * @return Error code
     */
    virtual Error status(FileSystemMessage *msg);

  protected:

    /** Type of this file. */
    FileType m_type;

    /** Access permissions. */
    FileModes m_access;

    /** Size of the file, in bytes. */
    Size m_size;

    /** Owner of the file. */
    UserID m_uid;

    /** Group of the file. */
    GroupID m_gid;

    /** Device major/minor ID. */
    DeviceID m_deviceId;
};

/**
 * @}
 * @}
 */

#endif /* __LIB_LIBFS_FILE_H */
