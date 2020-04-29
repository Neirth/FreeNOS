/*
 * Copyright (C) 2015 Niek Linnenbank
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

#ifndef __LIBFS_DIRECTORY_H
#define __LIBFS_DIRECTORY_H

#include <FreeNOS/System.h>
#include <List.h>
#include "File.h"
#include "FileSystemPath.h"
#include <stdio.h>
#include <string.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/** @brief Maximum length of a filename. */
#define DIRENT_LEN      64

/**
 * @brief Describes an entry inside a Directory.
 */
typedef struct Dirent
{
    /** Name of the file. */
    char name[DIRENT_LEN];

    /** Type of file. */
    FileType type;

    /**
     * Compares this Dirent with another Dirent instance.
     * @param dir Instance to compare with.
     * @return True if equal, false otherwise.
     */
    bool operator == (struct Dirent *dir)
    {
        return strcmp(name, dir->name) == 0;
    }
}
Dirent;

/**
 * @brief Directory File functionality.
 *
 * A Directory is a File which may have several child File's. Each
 * child File which also is a Directory, may in turn have child File's.
 * This way, a FileSystem can form a tree of File's and Directories.
 *
 * @see FileSystem
 * @see File
 */
class Directory : public File
{
  public:

    /**
     * Constructor
     */
    Directory();

    /**
     * Destructor
     */
    virtual ~Directory();

    /**
     * Read directory entries.
     *
     * This default read implementation reads the private
     * List of Dirent entries from memory. It can be usefull
     * for pseudo filesystems which don't have any real data
     * on Storage. Filesystem that do have date on Storage should
     * implement their version of read().
     *
     * @param buffer Input/Output buffer to output bytes to.
     * @param size Number of bytes to read, at maximum.
     * @param offset Offset inside the file to start reading.
     *
     * @return Number of bytes read on success, Error on failure.
     *
     * @see FileSystem
     * @see Storage
     * @see Dirent
     */
    virtual Error read(IOBuffer & buffer, Size size, Size offset);

    /**
     * Retrieve a File from storage.
     *
     * This virtual function is responsible for reading a
     * directory entry from storage which is not already in
     * the FileCache. It creates a File object for the directory entry
     * and returns a pointer to it, if found.
     *
     * @param name Name of the directory entry to lookup.
     *
     * @return Pointer to a File on success, ZERO otherwise.
     *
     * @see File
     * @see Dirent
     * @see Storage
     */
    virtual File * lookup(const char *name);

    /**
     * Insert a new directory entry.
     *
     * FileSystem implementations should implement this function
     * to insert a new implementation defined directory entry into
     * the underlying Storage.
     *
     * @param type File type.
     * @param name Formatted name of the entry to add.
     * @param ... Argument list.
     *
     * @note Entry names must be unique within the same Directory.
     * @see FileSystem
     * @see Storage
     */
    void insert(FileType type, const char *name, ...);

    /**
     * Remove a directory entry.
     *
     * A FileSystem implemenatation should lookup the internal
     * entry for the given name, and update the underlying Storage
     * to remove any references for it inside this Directory.
     *
     * @param name Name of the entry to remove.
     *
     * @see FileSystem
     * @see Storage
     */
    void remove(const char *name);

    /**
     * Clears the internal list of entries.
     */
    void clear();

  private:

    /**
     * Retrieve a directory entry by it's name.
     *
     * @param name Name of the entry to get.
     *
     * @return Direct pointer on success, ZERO otherwise.
     */
    Dirent * get(const char *name);

  private:

    /**
     * List of directory entries.
     *
     * This List should only be used for pseudo filesystems,
     * which use the default implementations of read(), insert() and remove().
     *
     * @see List
     * @see Directory::read
     * @see Directory::insert
     * @see Directory::remove
     */
    List<Dirent *> entries;
};

/**
 * @}
 * @}
 */

#endif /* __LIBFS_DIRECTORY_H */
