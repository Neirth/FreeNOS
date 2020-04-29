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

#ifndef __FILESYSTEM_LINN_FILE_H
#define __FILESYSTEM_LINN_FILE_H
#ifndef __HOST__

#include <File.h>
#include <FileSystemMessage.h>
#include <Types.h>
#include "LinnFileSystem.h"
#include "LinnInode.h"
#include "IOBuffer.h"
#include <errno.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup linnfs
 * @{
 */

/**
 * Represents a file on a mounted LinnFS filesystem.
 */
class LinnFile : public File
{
  public:

    /**
     * Constructor function.
     *
     * @param fs LinnFS filesystem pointer.
     * @param inode Inode pointer.
     */
    LinnFile(LinnFileSystem *fs, LinnInode *inode);

    /**
     * Destructor function.
     */
    virtual ~LinnFile();

    /**
     * @brief Read out the file.
     *
     * @param buffer Input/Output buffer to write bytes to.
     * @param size Number of bytes to copy at maximum.
     * @param offset Offset in the file to start reading.
     * @return Number of bytes read, or Error number.
     *
     * @see IOBuffer
     */
    virtual Error read(IOBuffer & buffer, Size size, Size offset);

  private:

    /** Filesystem pointer. */
    LinnFileSystem *fs;

    /** Inode pointer. */
    LinnInode *inode;
};

/**
 * @}
 * @}
 */

#endif /* __HOST__ */
#endif /* __FILESYSTEM_LINN_FILE_H */
