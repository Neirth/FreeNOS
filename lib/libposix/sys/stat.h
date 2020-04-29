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

#ifndef __LIBPOSIX_STAT_H
#define __LIBPOSIX_STAT_H

#include <FileType.h>
#include <FileMode.h>
#include <FileStat.h>
#include <Macros.h>
#include <errno.h>
#include <time.h>
#include "types.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/**
 * @name File stat type bits.
 *
 * The <sys/stat.h> header shall define the following symbolic constants for
 * the file types encoded in type mode_t. The values shall be suitable for
 * use in #if preprocessing directives.
 *
 * @{
 */

/** Type of file. */
#define S_IFMT   (FILETYPE_MASK << FILEMODE_BITS)

/** Block special. */
#define S_IFBLK  (BlockDeviceFile << FILEMODE_BITS)

/** Character special. */
#define S_IFCHR  (CharacterDeviceFile << FILEMODE_BITS)

/** FIFO special. */
#define S_IFIFO  (FIFOFile << FILEMODE_BITS)

/** Regular. */
#define S_IFREG  (RegularFile << FILEMODE_BITS)

/** Directory. */
#define S_IFDIR  (DirectoryFile << FILEMODE_BITS)

/** Symbolic link. */
#define S_IFLNK  (SymlinkFile << FILEMODE_BITS)

/** Socket. */
#define S_IFSOCK (SocketFile << FILEMODE_BITS)

/** @} */

/**
 * @brief File mode bits.
 *
 * The <sys/stat.h> header shall define the following symbolic constants for the
 * file mode bits encoded in type mode_t, with the indicated numeric values.
 * These macros shall expand to an expression which has a type that allows them
 * to be used, either singly or OR'ed together, as the third argument to open()
 * without the need for a mode_t cast. The values shall be suitable for use in
 * #if preprocessing directives.
 *
 * @{
 */

/** Read, write, execute/search by owner. */
#define S_IRWXU  OwnerRWX

/** Read permission, owner. */
#define S_IRUSR  OwnerR

/** Write permission, owner. */
#define S_IWUSR  OwnerW

/** Execute/search permission, owner. */
#define S_IXUSR  OwnerX

/** Read, write, execute/search by group. */
#define S_IRWXG  GroupRWX

/** Read permission, group. */
#define S_IRGRP  GroupR

/** Write permission, group. */
#define S_IWGRP  GroupW

/** Execute/search permission, group. */
#define S_IXGRP  GroupX

/** Read, write, execute/search by others. */
#define S_IRWXO  OtherRWX

/** Read permission, others. */
#define S_IROTH  OtherR

/** Write permission, others. */
#define S_IWOTH  OtherW

/** Execute/search permission, others. */
#define S_IXOTH  OtherX

/** @} */

/**
 * @name File type macros.
 *
 * The following macros shall be provided to test whether a file is of the
 * specified type. The value m supplied to the macros is the value of st_mode
 * from a stat structure. The macro shall evaluate to a non-zero value if the
 * test is true; 0 if the test is false.
 *
 * @{
 */

/** Test for mode bits. */
#define S_ISTYPE(mode, mask) (((mode) & S_IFMT) == (mask))

/** Test for a block special file. */
#define S_ISBLK(m)  S_ISTYPE(m, S_IFBLK)

/** Test for a character special file. */
#define S_ISCHR(m)  S_ISTYPE(m, S_IFCHR)

/** Test for a directory. */
#define S_ISDIR(m)  S_ISTYPE(m, S_IFDIR)

/** Test for a pipe or FIFO special file. */
#define S_ISFIFO(m) S_ISTYPE(m, S_IFIFO)

/** Test for a regular file. */
#define S_ISREG(m)  S_ISTYPE(m, S_IFREG)

/** Test for a symbolic link. */
#define S_ISLNK(m)  S_ISTYPE(m, S_IFLNK)

/** Test for a socket. */
#define S_ISSOCK(m) S_ISTYPE(m, S_IFSOCK)

/**
 * @}
 */

/**
 * The <sys/stat.h> header shall define the stat structure.
 */
struct stat
{
#ifdef CPP
    /**
     * Instantiates the structure given an FileStat object.
     * @param stat FileStat pointer to copy from.
     */
    void fromFileStat(FileStat *stat)
    {
        this->st_mode  = stat->access;
        this->st_mode |= stat->type << FILEMODE_BITS;
        this->st_size  = stat->size;
        this->st_uid   = stat->userID;
        this->st_gid   = stat->groupID;
        this->st_dev   = stat->deviceID;
    }
#endif /* CPP */

    /** Device ID of device containing file. */
    dev_t st_dev;

    /** File serial number. */
    ino_t st_ino;

    /** Mode of file. */
    mode_t st_mode;

    /** Number of hard links to the file. */
    nlink_t st_nlink;

    /** User ID of file. */
    uid_t st_uid;

    /** Group ID of file. */
    gid_t st_gid;

    /** Device ID (if file is character or block special). */
    dev_t st_rdev;

    /**
     * For regular files, the file size in bytes.
     * For symbolic links, the length in bytes of the
     * pathname contained in the symbolic link.
     * For a shared memory object, the length in bytes.
     * For a typed memory object, the length in bytes.
     * For other file types, the use of this field is
     * unspecified.
     */
    off_t st_size;

    /** Last data access timestamp. */
    struct timespec st_atim;

    /** Last data modification timestamp. */
    struct timespec st_mtim;

    /** Last file status change timestamp. */
    struct timespec st_ctim;

    /**
     * A file system-specific preferred I/O block size
     * for this object. In some file system types, this
     * may vary from file to file.
     */
    blksize_t st_blksize;

    /** Number of blocks allocated for this object. */
    blkcnt_t st_blocks;
};

/**
 * Get file status.
 *
 * @param path The path argument points to a pathname naming a file.
 *             Read, write, or execute permission of the named file
 *             is not required. An implementation that provides additional
 *             or alternate file access control mechanisms may, under
 *             implementation-defined conditions, cause stat() to fail.
 *             In particular, the system may deny the existence of the
 *             file specified by path.
 * @param buf The buf argument is a pointer to a stat structure,
 *            as defined in the <sys/stat.h> header, into which
 *            information is placed concerning the file.
 *
 * @return Upon successful completion, these functions shall return 0.
 *         Otherwise, these functions shall return -1 and set errno to
 *         indicate the error.
 */
extern C int stat(const char *path, struct stat *buf);

/**
 * Make directory, special file, or regular file
 *
 * @param path The mknod() function shall create a new file
 *             named by the pathname to which the argument path points.
 * @param mode The file type for path is OR'ed into the mode argument.
 *
 * @return Upon successful completion, these functions shall return 0.
 *         Otherwise, these functions shall return -1 and set errno to
 *         indicate the error. If -1 is returned, the new file shall
 *         not be created.
 */
extern C int mknod(const char *path, mode_t mode, dev_t dev);

/**
 * @brief Create a new directory.
 *
 * The mkdir() function shall create a new directory with name path.
 * The file permission bits of the new directory shall be initialized
 * from mode. These file permission bits of the mode argument shall be
 * modified by the process' file creation mask.
 *
 * @param path Full path to the directory to create.
 * @param mode Initial access permissions on the directory.
 *
 * @return Upon successful completion, these functions shall return 0.
 *         Otherwise, these functions shall return -1 and set errno to
 *         indicate the error. If -1 is returned, no directory shall
 *         be created.
 */
extern C int mkdir(const char *path, mode_t mode);

/**
 * @brief Create a new file or rewrite an existing one
 *
 * The creat() function is redundant. Its services are also provided by
 * the open() function. It has been included primarily for historical
 * purposes since many existing applications depend on it. It is best
 * considered a part of the C binding rather than a function that should
 * be provided in other languages.
 *
 * @param path Full path to the file to create
 * @param mode Initial access permissions for the file.
 *
 * @return Upon successful completion, these functions shall return 0.
 *         Otherwise, these functions shall return -1 and set errno to
 *         indicate the error. If -1 is returned, no file shall
 *         be created.
 */
extern C int creat(const char *path, mode_t mode);

/**
 * @}
 * @}
 */

#endif /* __LIBPOSIX_STAT_H */
