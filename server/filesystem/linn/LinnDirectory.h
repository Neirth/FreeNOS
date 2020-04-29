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

#ifndef __FILESYSTEM_LINN_DIRECTORY_H
#define __FILESYSTEM_LINN_DIRECTORY_H
#ifndef __HOST__

#include <FileSystemMessage.h>
#include <Directory.h>
#include <Types.h>
#include "LinnDirectoryEntry.h"
#include "LinnFileSystem.h"
#include "LinnInode.h"
#include "IOBuffer.h"

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup linnfs
 * @{
 */

/**
 * Represents an directory on a LinnFS filesystem.
 *
 * @see Directory
 * @see LinnDirectoryEntry
 * @see LinnFileSystem
 */
class LinnDirectory : public Directory
{
  public:

    /**
     * Constructor function.
     *
     * @param fs Filesystem pointer.
     * @param inode Inode pointer.
     *
     * @see LinnFileSystem
     * @see LinnInode
     */
    LinnDirectory(LinnFileSystem *fs, LinnInode *inode);

    /**
     * @brief Read directory entries.
     *
     * @param buffer Input/Output buffer to write bytes to.
     * @param size Number of bytes to copy at maximum.
     * @param offset Offset in the file to start reading.
     *
     * @return Number of bytes read on success, Error on failure.
     *
     * @see IOBuffer
     */
    virtual Error read(IOBuffer & buffer, Size size, Size offset);

    /**
     * @brief Retrieves a File pointer for the given entry name.
     *
     * This function reads a file from disk corresponding
     * to the LinnInode of the given entry name. It returns
     * an File object associated with that LinnInode.
     *
     * @param name Name of the entry to lookup.
     *
     * @return Pointer to a File if found, or ZERO otherwise.
     *
     * @see File
     * @see LinnInode
     */
    virtual File * lookup(const char *name);

  private:

    /**
     * Retrieve a directory entry.
     *
     * @param dent LinnDirectoryEntry buffer pointer.
     * @param name Unique name of the entry.
     *
     * @return True if successful, false otherwise.
     */
    bool getLinnDirectoryEntry(LinnDirectoryEntry *dent,
                               const char *name);

  private:

    /** Filesystem pointer. */
    LinnFileSystem *fs;

    /** Inode which describes the directory. */
    LinnInode *inode;
};

/**
 * @}
 * @}
 */

#endif /* __HOST__ */
#endif /* __FILESYSTEM_EXT2DIRECTORY_H */
