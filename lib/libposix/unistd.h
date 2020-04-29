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

#ifndef __LIBPOSIX_UNISTD_H
#define __LIBPOSIX_UNISTD_H

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
 * Integer value indicating version of this standard (C-language binding)
 * to which the implementation conforms. For implementations conforming to
 * POSIX.1-2008, the value shall be 200809L.
 */
#define _POSIX_VERSION 200809L

/**
 * Integer value indicating version of the Shell and Utilities volume of
 * POSIX.1 to which the implementation conforms. For implementations
 * conforming to POSIX.1-2008, the value shall be 200809L.
 */
#define _POSIX2_VERSION 200809L

/**
 * Get the process ID.
 *
 * The getpid() function shall return the process ID of the calling process.
 *
 * @return The getpid() function shall always be successful and no return
 *         value is reserved to indicate an error.
 */
extern C pid_t getpid();

/**
 * Get parent process ID.
 *
 * @return The parent process ID.
 */
extern C pid_t getppid();

/**
 * Read from a file
 *
 * @param fildes The read() function shall attempt to read nbyte bytes from the file
 *               associated with the open file descriptor, fildes, into the buffer
 *               pointed to by buf.
 * @param buf The read() function shall attempt to read nbyte bytes from the file
 *            associated with the open file descriptor, fildes, into the buffer
 *            pointed to by buf.
 * @param nbyte If the value of nbyte is greater than {SSIZE_MAX}, the result is implementation-defined.
 *
 * @return Upon successful completion, these functions shall return a non-negative integer
 *         indicating the number of bytes actually read. Otherwise, the functions shall
 *         return -1 and set errno to indicate the error.
 */
extern C ssize_t read(int fildes, void *buf, size_t nbyte);

/**
 * @brief Write on a file.
 *
 * The write() function shall attempt to write nbyte bytes from
 * the buffer pointed to by buf to the file associated with the
 * open file descriptor, fildes.
 *
 * @param fildes File descriptor.
 * @param buf Input buffer.
 * @param nbyte Maximum number of bytes to write.
 *
 * @return Upon successful completion, these functions shall return the number
 *         of bytes actually written to the file associated with fildes. This
 *         number shall never be greater than nbyte. Otherwise, -1 shall be
 *         returned and errno set to indicate the error.
 */
extern C ssize_t write(int fildes, const void *buf, size_t nbyte);

/**
 * Close a file descriptor
 *
 * @param fildes The close() function shall deallocate the file descriptor indicated by fildes.
 *
 * @return Upon successful completion, 0 shall be returned; otherwise, -1 shall
 *         be returned and errno set to indicate the error.
 *
 */
extern C int close(int fildes);

/**
 * @brief Move the read/write file offset.
 *
 * The lseek() function shall set the file offset for the open file
 * description associated with the file descriptor fildes, as follows:
 *
 * If whence is SEEK_SET, the file offset shall be set to offset bytes.
 * If whence is SEEK_CUR, the file offset shall be set to its current location plus offset.
 * If whence is SEEK_END, the file offset shall be set to the size of the file plus offset.
 *
 * @param fildes File descriptor.
 * @param offset New file offset.
 * @param whence Determines how to modify the file offset pointer.
 *
 * @return Upon successful completion, the resulting offset, as measured in bytes
 *         from the beginning of the file, shall be returned. Otherwise, (off_t)-1
 *         shall be returned, errno shall be set to indicate the error, and the file
 *         offset shall remain unchanged.
 */
extern C off_t lseek(int fildes, off_t offset, int whence);

/**
 * @brief Create a new process and execute program.
 *
 * @param path File to execute.
 * @param argv Argument list pointer.
 *
 * @return New process ID on success and -1 on failure.
 * @note  Errno is set with the appropriate error code on failure.
 */
extern C int forkexec(const char *path, const char *argv[]);

/**
 * @brief Create a new process using in-memory image.
 *
 * @param program In-memory executable to run
 * @param programSize number of bytes of the executable
 * @param command Command line to pass to the program
 *
 * @return New process ID on success and -1 on failure.
 * @note  Errno is set with the appropriate error code on failure.
 */
extern C int spawn(Address program, Size programSize, const char *command);

/**
 * @brief Get name of current host.
 *
 * The gethostname() function shall return the standard host name for the
 * current machine. The namelen argument shall specify the size of the
 * array pointed to by the name argument. The returned name shall be
 * null-terminated, except that if namelen is an insufficient length to
 * hold the host name, then the returned name shall be truncated and it
 * is unspecified whether the returned name is null-terminated.
 * Host names are limited to {HOST_NAME_MAX} bytes.
 *
 * @param name Output buffer.
 * @param namelen Size of the output buffer.
 *
 * @return Upon successful completion, 0 shall be returned; otherwise,
 *         -1 shall be returned.
 */
extern C int gethostname(char *name, size_t namelen);

/**
 * @brief Get the pathname of the current working directory.
 *
 * The getcwd() function shall place an absolute pathname of the
 * current working directory in the array pointed to by buf, and
 * return buf. The pathname shall contain no components that are
 * dot or dot-dot, or are symbolic links.
 *
 * @param buf Output buffer, to store the current directory.
 * @param size Size of the output buffer.
 *
 * @return Upon successful completion, getcwd() shall return
 *         the buf argument. Otherwise, getcwd() shall return
 *         a null pointer and set errno to indicate the error.
 *         The contents of the array pointed to by buf are then undefined.
 */
extern C char *getcwd(char *buf, size_t size);

/**
 * @brief Change working directory.
 *
 * The chdir() function shall cause the directory named by the
 * pathname pointed to by the path argument to become the current
 * working directory; that is, the starting point for path searches
 * for pathnames not beginning with '/' .
 *
 * @param path Path to the directory to use as working directory.
 *
 * @return Upon successful completion, 0 shall be returned. Otherwise,
 *         -1 shall be returned, the current working directory shall
 *         remain unchanged, and errno shall be set to indicate the error.
 */
extern C int chdir(const char *path);

/**
 * @brief Remove a file from the filesystem.
 * @param path Parh to the file to remove.
 * @return Zero on success or -1 on failure with errno set.
 */
extern C int unlink(const char *path);

/**
 * Sleep for the specified number of seconds.
 *
 * @param seconds Number of seconds to sleep
 *
 * @return Zero on success or number of seconds left when interrupted.
 */
extern C unsigned int sleep(unsigned int seconds);

/**
 * @}
 * @}
 */

#endif /* __LIBPOSIX_UNISTD_H */
