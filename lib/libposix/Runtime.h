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

#ifndef __LIBPOSIX_RUNTIME_H
#define __LIBPOSIX_RUNTIME_H

#include <Macros.h>
#include <Types.h>
#include <Array.h>
#include <String.h>
#include <ChannelClient.h>
#include "FileSystemMount.h"
#include "FileDescriptor.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/** Maximum size of each argument. */
#define ARGV_SIZE  128

/** Number of arguments at maximum. */
#define ARGV_COUNT (PAGESIZE / ARGV_SIZE)

/**
 * Program entry point.
 *
 * @param argc Argument count.
 * @param argv Argument values.
 *
 * @return Exit status.
 */
int main(int argc, char **argv);

/**
 * Retrieve the ProcessID of the FileSystemMount for the given path.
 *
 * @param path Path to lookup.
 *
 * @return ProcessID of the FileSystemMount on success and ZERO otherwise.
 */
ProcessID findMount(const char *path);

/**
 * Lookup the ProcessID of a FileSystemMount by a filedescriptor number.
 *
 * @param fildes FileDescriptor number.
 *
 * @return ProcessID of the FileSystemMount on success and ZERO otherwise.
 */
ProcessID findMount(int fildes);

/**
 * Refresh mounted filesystems
 *
 * @param path Input path to refresh or NULL to refresh all mountpoints
 */
void refreshMounts(const char *path);

/**
 * Blocking wait for a mounted filesystem
 *
 * @param path Full path of the mounted filesystem
 *
 * @note Blocks until a filesystem is mounted on the exact given input path
 */
void waitMount(const char *path);

/**
 * Get File Descriptors table.
 *
 * @return FileDescriptor array pointer
 */
FileDescriptor * getFiles();

/**
 * Get mounts table.
 *
 * @return FileSystemMount array pointer
 */
FileSystemMount * getMounts();

/**
 * Get current directory String.
 *
 * @return String pointer containing current directory
 */
String * getCurrentDirectory();

/**
 * @}
 * @}
 */

#endif /* __LIBPOSIX_RUNTIME_H */
