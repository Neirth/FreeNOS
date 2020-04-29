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

#ifndef __FILESYSTEM_LINN_FILESYSTEM_H
#define __FILESYSTEM_LINN_FILESYSTEM_H

#include <FileSystem.h>
#include <FileSystemPath.h>
#include <FileSystemMessage.h>
#include <Storage.h>
#include <Types.h>
#include <Vector.h>
#include <HashTable.h>
#include "LinnSuperBlock.h"
#include "LinnInode.h"
#include "LinnGroup.h"

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup linnfs
 * @{
 */

/** Default filename of the embedded root filesystem (ramfs) */
#define LINNFS_ROOTFS_FILE "./rootfs.linn"

/**
 * @name Filesystem limits.
 * @{
 */

/** Minimum blocksize. */
#define LINN_MIN_BLOCK_SIZE 1024

/** Maximum blocksize. */
#define LINN_MAX_BLOCK_SIZE 4096

/**
 * @}
 */

#ifndef __HOST__

/**
 * @brief Linnenbank FileSystem (LinnFS).
 *
 * This filesystem is rougly based on the Extended 2 FileSystem. Some
 * changes have been made to the superblock: leaving out unused fields
 * and simplified the meaning of existing onces. Directory entries are now
 * static in size, i.e. 64-bytes. Those changes make it easier to program
 * the FileSystem implementation, thus easier to understand and learn from.
 *
 * @see FileSystem
 * @see Ext2FileSystem
 */
class LinnFileSystem : public FileSystem
{
  public:

    /**
     * Class constructor function.
     *
     * @param path Path to which we are mounted.
     * @param storage Storage provider.
     */
    LinnFileSystem(const char *path, Storage *storage);

    /**
     * Retrieve the superblock pointer.
     *
     * @return Pointer to the superblock for this filesystem.
     *
     * @see LinnSuperBlock
     */
    LinnSuperBlock * getSuperBlock()
    {
        return &super;
    }

    /**
     * Get the underlying Storage object.
     *
     * @return Storage pointer.
     *
     * @see Storage
     */
    Storage * getStorage()
    {
        return storage;
    }

    /**
     * Read an inode from the filesystem.
     *
     * @param inodeNum Inode number.
     *
     * @return Pointer to an LinnInode on success, ZERO on failure.
     *
     * @see LinnInode
     */
    LinnInode * getInode(u32 inodeNum);

    /**
     * Read a group descriptor from the filesystem.
     *
     * @param groupNum Group descriptor number.
     *
     * @return Pointer to an LinnGroup on success, ZERO on failure.
     *
     * @see LinnGroup
     */
    LinnGroup * getGroup(u32 groupNum);

    /**
     * Read a group descriptor from the filesystem, given an inode number.
     *
     * @param inodeNum Find the corresponding group via this inode number.
     *
     * @return Pointer to an LinnGroup on success, ZERO on failure.
     *
     * @see LinnGroup
     * @see LinnInode
     */
    LinnGroup * getGroupByInode(u32 inodeNum);

    /**
     * Calculates the offset inside storage for a given block.
     *
     * @param inode LinnInode pointer.
     * @param blk Calculate the offset for this block.
     *
     * @return Offset in bytes in storage.
     *
     * @see LinnInode
     */
    u64 getOffset(LinnInode *inode, u32 blk);

  private:

    /**
     * Callback handler for unsupported operations
     *
     * @param msg FileSystemMessage pointer
     */
    void notSupportedHandler(FileSystemMessage *msg);

  private:

    /** Provides storage. */
    Storage *storage;

    /** Describes the filesystem. */
    LinnSuperBlock super;

    /** Group descriptors. */
    Vector<LinnGroup *> *groups;

    /** Inode cache. */
    HashTable<u32, LinnInode *> inodes;
};

#endif /* __HOST__ */

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_LINN_FILESYSTEM_H */
