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

#ifndef __FILESYSTEM_LINN_GROUP_H
#define __FILESYSTEM_LINN_GROUP_H

#include <Types.h>
#include "LinnSuperBlock.h"

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup linnfs
 * @{
 */

/**
 * @name LinnGroup Block Index numbers.
 * @{
 */

/** Block index of the block bitmap. */
#define LINN_GROUP_BLOCK_BITMAP 0

/** Block index of the inode bitmap. */
#define LINN_GROUP_INODE_BITMAP 1

/** Block index of the inode table. */
#define LINN_GROUP_INODE_TABLE  2

/**
 * @}
 */

/**
 * @name LinnGroup Macros.
 * @{
 */

/**
 * @brief Calculate the number of LinnGroups in a filesystem.
 *
 * Calculation is based on the blocksCount and blocksPerGroup fields.
 *
 * @param sb LinnSuperBlock pointer.
 *
 * @return Number of LinnGroups in the filesystem.
 */
#define LINN_GROUP_COUNT(sb) \
    ((sb)->blocksCount / (sb)->blocksPerGroup ? \
     (sb)->blocksCount / (sb)->blocksPerGroup : 1)

/**
 * Calculate the number of blocks needed for the blocks bitmap.
 *
 * @param sb LinnSuperBlock pointer.
 *
 * @return Number of blocks needed for the blocks bitmap.
 */
#define LINN_GROUP_NUM_BLOCKMAP(sb) \
    ((sb)->blocksPerGroup / LINN_SUPER_NUM_PTRS(sb) ? \
     (sb)->blocksPerGroup / LINN_SUPER_NUM_PTRS(sb) : 1)

/**
 * Calculate the number of blocks needed for the inodes bitmap.
 *
 * @param sb LinnSuperBlock pointer.
 *
 * @return Number of blocks needed for the inodes bitmap.
 */
#define LINN_GROUP_NUM_INODEMAP(sb) \
    ((sb)->inodesPerGroup / LINN_SUPER_NUM_PTRS(sb) ? \
     (sb)->inodesPerGroup / LINN_SUPER_NUM_PTRS(sb) : 1)

/**
 * Calculate the number of blocks needed for the inodes table.
 *
 * @param sb LinnSuperBlock pointer.
 *
 * @return Number of blocks needed for the inodes table.
 */
#define LINN_GROUP_NUM_INODETAB(sb) \
    ((sb)->inodesPerGroup / ((sb)->blockSize / sizeof(LinnInode)) ? \
     (sb)->inodesPerGroup / ((sb)->blockSize / sizeof(LinnInode)) : 1)

/**
 * Calculate the number of LinnGroups which fit in one block.
 *
 * @param sb LinnSuperBlock pointer.
 *
 * @return Number of LinnGroups per block.
 */
#define LINN_GROUP_PER_BLOCK(sb) \
    ((sb)->blockSize / sizeof(LinnGroup))

/**
 * Number of blocks needed to store LinnGroups.
 *
 * @param sb LinnSuperBlock pointer.
 *
 * @return Number of blocks required for LinnGroups.
 */
#define LINN_GROUP_BLOCKS(sb) \
    (LINN_GROUP_COUNT(sb) / LINN_GROUP_PER_BLOCK(sb) ? \
     LINN_GROUP_COUNT(sb) / LINN_GROUP_PER_BLOCK(sb) : 1)

/**
 * @}
 */

/**
 * Structure of a group descriptor.
 */
typedef struct LinnGroup
{
    /** The number of free blocks in this group. */
    le32 freeBlocksCount;

    /** Number of free inodes in this group. */
    le32 freeInodesCount;

    /** Block bitmap. Used to mark blocks used/free. */
    le32 blockMap;

    /** Inode bitmap. Used to mark inodes used/free. */
    le32 inodeMap;

    /** Inode table contains pre-allocated inodes. */
    le32 inodeTable;
}
LinnGroup;

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_LINN_GROUP_H */
