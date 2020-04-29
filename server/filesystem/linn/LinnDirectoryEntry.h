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

#ifndef __FILESYSTEM_LINN_DIRECTORY_ENTRY_H
#define __FILESYSTEM_LINN_DIRECTORY_ENTRY_H

#include <Types.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup linnfs
 * @{
 */

/**
 * Calculates the number of LinnDirectoryEntry's fitting in one block.
 * @return Number of entries.
 */
#define LINN_DIRENT_PER_BLOCK(sb) \
    ((sb)->blockSize / sizeof(LinnDirectoryEntry))

/** Length of the name field in an directory entry. */
#define LINN_DIRENT_NAME_LEN 59

/**
 * Struct of an directory entry in LinnFS.
 */
typedef struct LinnDirectoryEntry
{
    /** Inode number. */
    le32 inode;

    /** Type of file, as an FileType. */
    u8 type;

    /** File name. Null terminated. */
    char name[LINN_DIRENT_NAME_LEN];
}
LinnDirectoryEntry;

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_LINN_DIRECTORY_ENTRY_H */
