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

#ifndef __LIB_LIBFS_FILECACHE_H
#define __LIB_LIBFS_FILECACHE_H

#include <HashTable.h>
#include "File.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/**
 * Cached in-memory file.
 */
typedef struct FileCache
{
    /**
     * @brief Constructor function.
     *
     * @param f File to insert into the cache.
     * @param name Entry name of the File in the parent, if any.
     * @param p Our parent. ZERO if we have no parent.
     */
    FileCache(File *f, const char *n, FileCache *p)
            : file(f), valid(true), parent(p)
    {
        name = n;

        if (p && p != this)
        {
            p->entries.insert(name, this);
        }
    }

    /**
     * Comparision operator.
     *
     * @param fc Instance to compare us with.
     *
     * @return True if equal, false otherwise.
     */
    bool operator == (FileCache *fc)
    {
        return file == fc->file;
    }

    /** File pointer. */
    File *file;

    /** Our name */
    String name;

    /** Contains childs. */
    HashTable<String, FileCache *> entries;

    /** Is this entry still valid?. */
    bool valid;

    /** Parent */
    FileCache *parent;
}
FileCache;

/**
 * @}
 * @}
 */

#endif /* __LIB_LIBFS_FILECACHE_H */
