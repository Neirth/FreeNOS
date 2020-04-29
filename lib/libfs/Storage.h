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

#ifndef __FILESYSTEM_STORAGE_H
#define __FILESYSTEM_STORAGE_H

#include <Types.h>
#include <errno.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/**
 * Provides a storage device to build filesystems on top.
 */
class Storage
{
  public:

    /**
     * Constructor function.
     */
    Storage();

    /**
     * Destructor function.
     */
    virtual ~Storage();

    /**
     * Read a contiguous set of data.
     *
     * @param offset Offset to start reading from.
     * @param buffer Output buffer.
     * @param size Number of bytes to copied.
     *
     * @return Result code
     */
    virtual Error read(u64 offset, void *buffer, Size size);

    /**
     * Write a contiguous set of data.
     *
     * @param offset Offset to start writing to.
     * @param buffer Input buffer.
     * @param size Number of bytes to written.
     *
     * @return Result code
     */
    virtual Error write(u64 offset, void *buffer, Size size);

    /**
     * Retrieve maximum storage capacity.
     *
     * @return Storage capacity.
     */
    virtual u64 capacity() const = 0;
};

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_STORAGE_H */
