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

#ifndef __FILESYSTEM_BOOTIMAGESTORAGE_H
#define __FILESYSTEM_BOOTIMAGESTORAGE_H

#include <Types.h>
#include <BootImage.h>
#include "Storage.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libfs
 * @{
 */

/**
 * Uses a BootImage symbol entry as filesystem storage provider (aka RamFS).
 */
class BootImageStorage : public Storage
{
  public:

    /**
     * Constructor function.
     *
     * @param name Name of the BootSymbol entry to use.
     */
    BootImageStorage(const char *name);

    /**
     * Loads the boot module into virtual memory.
     *
     * @return True on success, false otherwise.
     */
    bool load();

    /**
     * Reads data from the boot module.
     *
     * @param offset Offset to start reading from.
     * @param buffer Output buffer.
     * @param size Number of bytes to copied.
     *
     * @return Error code
     */
    virtual Error read(u64 offset, void *buffer, Size size);

    /**
     * Retrieve maximum storage capacity.
     *
     * @return Storage capacity.
     */
    virtual u64 capacity() const;

  private:

    /** Name of the BootSymbol */
    const char *m_name;

    /** Data pointer */
    u8 *m_data;

    /** Size of the BootSymbol. */
    Size m_size;
};

/**
 * @}
 * @}
 */

#endif /* __FILESYSTEM_BOOTIMAGESTORAGE_H */
