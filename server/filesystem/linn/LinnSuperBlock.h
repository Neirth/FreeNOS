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

#ifndef __FILESYSTEM_LINNSUPERBLOCK_H
#define __FILESYSTEM_LINNSUPERBLOCK_H

#include <Types.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup linnfs
 * @{
 */

/**
 * @name Magic Numbers.
 * @{
 */

/** First magic number ('Linn'). Used to detect a valid superblock. */
#define LINN_SUPER_MAGIC0       0x4c696e6e

/** Second magic number (randomly chosen bytes). */
#define LINN_SUPER_MAGIC1       0x512ea9b0

/**
 * @}
 */

/**
 * @name Revision Numbers.
 * @{
 */

/** Current major revision number. */
#define LINN_SUPER_MAJOR        1

/** Current minor revision number. */
#define LINN_SUPER_MINOR        0

/**
 * @}
 */

/**
 * @name Filesystem States.
 * @{
 */

/** Filesystem is consistent. */
#define LINN_SUPER_VALID        0

/** The filesystem has not been properly unmounted. */
#define LINN_SUPER_UNCLEAN      1

/** Serious corruption has been detected. */
#define LINN_SUPER_CORRUPT      2

/**
 * @}
 */

/**
 * @name Superblock Constants.
 * @{
 */

/** Fixed offset in storage of the superblock. */
#define LINN_SUPER_OFFSET       1024

/**
 * @}
 */

/**
 * @name Superblock Macros.
 * @{
 */

/**
 * Calculate the number of block address pointers fitting in one block.
 *
 * @param sb LinnSuperBlock pointer.
 *
 * @return Number of block address pointers.
 */
#define LINN_SUPER_NUM_PTRS(sb) \
    ((sb)->blockSize / sizeof(u32))

/**
 * @}
 */

/**
 * Linnenbank Filesystem (LinnFS) super block.
 */
typedef struct LinnSuperBlock
{
    le32 magic0;                /**< Allows detection of valid superblocks. */
    le32 magic1;                /**< Allows detection of valid superblocks. */
    le16 majorRevision;         /**< Filesystem major revision level. */
    le16 minorRevision;         /**< Filesystem minor revision level. */
    le16 state;                 /**< Describes the current status. */

    le32 blockSize;             /**< Size of each data block. */
    le32 blocksPerGroup;        /**< Number of blocks per group. */
    le32 inodesPerGroup;        /**< Number of inodes per group. */

    le32 inodesCount;           /**< Total number of inodes. */
    le32 blocksCount;           /**< Total number of data blocks. */
    le32 freeBlocksCount;       /**< Number of free data blocks. */
    le32 freeInodesCount;       /**< Free inodes remaining. */

    le32 creationTime;          /**< Time when the filesystem was created. */
    le32 mountTime;             /**< Last time we where mounted (seconds since 1970). */
    le16 mountCount;            /**< Number of times we where mounted. */
    le32 lastCheck;             /**< Timestamp of the last check. */

    le32 groupsTable;           /**< Block address of the LinnGroup table. */
}
LinnSuperBlock;

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_LINNSUPERBLOCK_H */
