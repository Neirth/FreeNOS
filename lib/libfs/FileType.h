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

#ifndef __FILESYSTEM_FILETYPE_H
#define __FILESYSTEM_FILETYPE_H

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/** Number of bits needed to store a FileType. */
#define FILETYPE_BITS   3

/** Masker value for all FileTypes. */
#define FILETYPE_MASK   7

/**
 * Convert from a (host system's) POSIX struct stat into a FileType.
 * @param st struct stat pointer.
 * @return FileType value.
 */
#define FILETYPE_FROM_ST(st) \
({ \
    FileType t = UnknownFile; \
    \
    switch ((st)->st_mode & S_IFMT) \
    { \
        case S_IFBLK:  t = BlockDeviceFile; break; \
        case S_IFCHR:  t = CharacterDeviceFile; break; \
        case S_IFIFO:  t = FIFOFile; break; \
        case S_IFREG:  t = RegularFile; break; \
        case S_IFDIR:  t = DirectoryFile; break; \
        case S_IFLNK:  t = SymlinkFile; break; \
        case S_IFSOCK: t = SocketFile; break; \
        default: break; \
    } \
    t; \
})

/**
 * All possible filetypes.
 */
typedef enum FileType
{
    RegularFile         = 0,
    DirectoryFile       = 1,
    BlockDeviceFile     = 2,
    CharacterDeviceFile = 3,
    SymlinkFile         = 4,
    FIFOFile            = 5,
    SocketFile          = 6,
    UnknownFile         = 7,
}
FileType;

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_FILETYPE_H */
