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

#ifndef __FILESYSTEM_FILEMODE_H
#define __FILESYSTEM_FILEMODE_H

#include <Types.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/** Number of bits required for all FileModes. */
#define FILEMODE_BITS 9

/** Masker value for all FileMode values. */
#define FILEMODE_MASK 0777

/**
 * Converts an (host system's) POSIX struct st into a FileMode.
 *
 * @param st struct st pointer.
 *
 * @return FileMode value.
 */
#define FILEMODE_FROM_ST(st) \
    (FileMode)((st)->st_mode & FILEMODE_MASK)

/**
 * File access permissions.
 */
typedef enum FileMode
{
    OwnerR   = 0400,
    OwnerW   = 0200,
    OwnerX   = 0100,
    OwnerRW  = 0600,
    OwnerRX  = 0500,
    OwnerRWX = 0700,
    GroupR   = 0040,
    GroupW   = 0020,
    GroupX   = 0010,
    GroupRW  = 0060,
    GroupRX  = 0050,
    GroupRWX = 0070,
    OtherR   = 0004,
    OtherW   = 0002,
    OtherX   = 0001,
    OtherRW  = 0006,
    OtherRX  = 0005,
    OtherRWX = 0007,
}
FileMode;

/** Multiple FileMode values combined. */
typedef u16 FileModes;

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_FILEMODE_H */
