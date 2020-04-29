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

#ifndef __FILESYSTEM_FILESTAT_H
#define __FILESYSTEM_FILESTAT_H

#include <Types.h>
#include "FileType.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/**
 * Contains file information.
 */
typedef struct FileStat
{
    /** File type. */
    FileType type;

    /** File access permission bits. */
    FileModes access;

    /** Size of the file in bytes. */
    Size size;

    /** User identity. */
    UserID userID;

    /** Group identity. */
    GroupID groupID;

    /** Device identity. */
    DeviceID deviceID;
}
FileStat;

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_FILESTAT_H */
