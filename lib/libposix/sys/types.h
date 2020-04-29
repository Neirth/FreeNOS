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

#ifndef __LIBPOSIX_TYPES_H
#define __LIBPOSIX_TYPES_H

#include <Types.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/** Used for process IDs and process group IDs. */
typedef ProcessID pid_t;

/** Used for sizes of objects. */
typedef Size size_t;

/** Used for a count of bytes or an error indication. */
typedef slong ssize_t;

/** Used for device IDs. */
typedef DeviceID dev_t;

/** Used for file serial numbers. */
typedef uint ino_t;

/** Used for some file attributes. */
typedef uint mode_t;

/** Used for user IDs. */
typedef u16 uid_t;

/** Used for group IDs. */
typedef u16 gid_t;

/** Used for link counts. */
typedef Size nlink_t;

/** Used for file sizes. */
typedef sint off_t;

/** Used for file block counts. */
typedef sint blkcnt_t;

/** Used for block sizes. */
typedef sint blksize_t;

/** Used for time in seconds. */
typedef u64 time_t;

/**
 * @}
 * @}
 */

#endif /* __LIBPOSIX_TYPES_H */
