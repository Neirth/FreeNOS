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

#ifndef __FILESYSTEM_LINN_INODE_H
#define __FILESYSTEM_LINN_INODE_H

#include <Types.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup linnfs
 * @{
 */

/**
 * @name Special Inode Numbers
 * @{
 */

/** Root inode. */
#define LINN_INODE_ROOT         0

/** Boot loader inode. */
#define LINN_INODE_LOADER       1

/** Bad blocks inode. */
#define LINN_INODE_BAD          2

/** Journal inode. */
#define LINN_INODE_JOURNAL      3

/** First non-reserved inode. */
#define LINN_INODE_FIRST        4

/**
 * @}
 */

/**
 * @name Inode (in)direct block counts.
 * @{
 */

/** Direct blocks. */
#define LINN_INODE_DIR_BLOCKS   4

/** Indirect blocks. */
#define LINN_INODE_IND_BLOCKS   (LINN_INODE_DIR_BLOCKS  + 1)

/** Double indirect blocks. */
#define LINN_INODE_DIND_BLOCKS  (LINN_INODE_IND_BLOCKS  + 1)

/** Triple indirect blocks. */
#define LINN_INODE_TIND_BLOCKS  (LINN_INODE_DIND_BLOCKS + 1)

/** Total number of block pointers in an LinnInode. */
#define LINN_INODE_BLOCKS       (LINN_INODE_TIND_BLOCKS + 1)

/**
 * @}
 */

/**
 * @name Inode macros.
 * @{
 */

/**
 * Calculate the number of blocks used in an LinnInode.
 *
 * @param super LinnSuperBlock pointer.
 * @param inode LinnInode pointer.
 *
 * @return Number of blocks used.
 */
#define LINN_INODE_NUM_BLOCKS(super,inode) \
    ((inode)->size % (super)->blockSize ? \
     (inode)->size / (super)->blockSize + 1 : \
     (inode)->size / (super)->blockSize)

/**
 * @}
 */

/**
 * Structure of an inode on the disk in the LinnFS filesystem.
 */
typedef struct LinnInode
{
    le16 type:3;        /**< Type of file, as an FileType. */
    le16 mode:13;       /**< Access permissions, as an FileMode. */
    le16 uid;           /**< User Identity. */
    le16 gid;           /**< Group Identity. */
    le32 size;          /**< Size in bytes. */
    le32 accessTime;    /**< Access time. */
    le32 createTime;    /**< Creation time. */
    le32 modifyTime;    /**< Modification time. */
    le32 changeTime;    /**< Status change timestamp. */
    le16 links;         /**< Links count. */
    le32 block[LINN_INODE_BLOCKS]; /**< Pointers to blocks. */
}
LinnInode;

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_LINN_INODE_H */
