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

#ifndef __LIBPOSIX_FCNTL_H
#define __LIBPOSIX_FCNTL_H

#include <Macros.h>
#include <stdio.h>
#include "sys/types.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/**
 * @name File Constants
 * @{
 */

/** Create file if it does not exist. */
#define O_CREAT         (1 << 0)

/** Exclusive use flag. */
#define O_EXCL          (1 << 1)

/** Do not assign controlling terminal. */
#define O_NOCTTY        (1 << 2)

/** Truncate flag. */
#define O_TRUNC         (1 << 3)

/**
 * @brief Set the termios structure terminal parameters to a state that
 *        provides conforming behavior; see Parameters that Can be Set.
 */
#define O_TTY_INIT      (1 << 4)

/** Set append mode. */
#define O_APPEND        (1 << 5)

/** Write according to synchronized I/O data integrity completion. */
#define O_DSYNC         (1 << 6)

/** Non-blocking mode. */
#define O_NONBLOCK      (1 << 7)

/** Synchronized read I/O operations. */
#define O_RSYNC         (1 << 8)

/** Write according to synchronized I/O file integrity completion. */
#define O_SYNC          (1 << 9)

/** Mask for file access modes. */
#define O_ACCMODE       (O_EXEC | O_RDONLY | O_RDWR | O_SEARCH | O_WRONLY)

/**
 * @brief Open for execute only (non-directory files).
 *        The result is unspecified if this flag is applied to a directory.
 */
#define O_EXEC          (1 << 10)

/** Open for reading only. */
#define O_RDONLY        (1 << 11)

/** Open for reading and writing. */
#define O_RDWR          (1 << 12)

/**
 * @brief Open directory for search only.
 *        The result is unspecified if this flag is applied to a non-directory file.
 */
#define O_SEARCH        (1 << 13)

/** Open for writing only. */
#define O_WRONLY        (1 << 14)

/**
 * @}
 */

/**
 * Open file relative to directory file descriptor.
 *
 * @param path The path argument points to a pathname naming the file.
 * @param oflag Values for oflag are constructed by a bitwise-inclusive OR of
 *              flags from the following list, defined in <fcntl.h>.
 *
 * @return Upon successful completion, these functions shall open the file and
 *         return a non-negative integer representing the lowest numbered unused
 *         file descriptor. Otherwise, these functions shall return -1 and set
 *         errno to indicate the error. If - is returned, no files shall be created or modified.
 */
extern C int open(const char *path, int oflag, ...);

/**
 * @}
 * @}
 */

#endif /* __LIBPOSIX_UNISTD_H */
