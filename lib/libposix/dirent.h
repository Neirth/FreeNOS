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

#ifndef __LIBPOSIX_DIRENT_H
#define __LIBPOSIX_DIRENT_H

#include <Macros.h>
#include <Types.h>
#include "sys/types.h"
#include "string.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/** The file type is unknown. */
#define DT_UNKNOWN       0

/** This is a named pipe (FIFO). */
#define DT_FIFO          1

/** This is a character device. */
#define DT_CHR           2

/** This is a directory. */
#define DT_DIR           4

/** This is a block device. */
#define DT_BLK           6

/** This is a regular file. */
#define DT_REG           8

/** This is a symbolic link. */
#define DT_LNK          10

/** This is a Unix domain socket. */
#define DT_SOCK         12

/** Maximum length of a directory entry name. */
#define DIRLEN          64

/**
 * Represents a directory entry.
 */
struct dirent
{
    /** Name of entry. */
    char d_name[DIRLEN];

    /** Type of file. */
    u8 d_type;

#ifdef CPP

    /**
     * Comparison operator.
     *
     * @param d Dirent pointer.
     *
     * @return True if equal, false otherwise.
     */
    bool operator == (struct dirent *d)
    {
        return strcmp(d->d_name, d_name) == 0 && d->d_type == d_type;
    }

#endif /* CPP */
};

/**
 * A type representing a directory stream.
 *
 * @note The DIR type may be an incomplete type.
 */
typedef struct DIR
{
    /** File descriptor returned by opendir(). */
    int fd;

    /** Input buffer. */
    struct dirent *buffer;

    /** Index of the current dirent. */
    Size current;

    /** Number of direct structures in the buffer. */
    Size count;

    /** End-of-file reached? */
    bool eof;
}
DIR;

/**
 * Open directory associated with file descriptor.
 *
 * @param dirname The opendir() function shall open a directory stream
 *                corresponding to the directory named by the dirname argument.
 *
 * @return Upon successful completion, these functions shall return a
 *         pointer to an object of type DIR. Otherwise, these functions
 *         shall return a null pointer and set errno to indicate the error.
 */
extern C DIR * opendir(const char *dirname);

/**
 * Read a directory.
 *
 * @param dirp The readdir() function shall return a pointer to a structure representing
 *             the directory entry at the current position in the directory stream
 *             specified by the argument dirp, and position the directory stream
 *             at the next entry.
 *
 * @return Upon successful completion, readdir() shall return a pointer to an object
 *         of type struct dirent. When an error is encountered, a null pointer shall
 *         be returned and errno shall be set to indicate the error. When the end of
 *         the directory is encountered, a null pointer shall be returned and errno is not changed.
 */
extern C struct dirent * readdir(DIR *dirp);

/**
 * Close a directory stream.
 *
 * @param dirp The closedir() function shall close the directory stream referred
 *             to by the argument dirp.
 *
 * @return Upon successful completion, closedir() shall return 0; otherwise,
 *         -1 shall be returned and errno set to indicate the error.
 */
extern C int closedir(DIR *dirp);

/**
 * @}
 * @}
 */

#endif /* __LIBPOSIX_DIRENT_H */
