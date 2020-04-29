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

#ifndef __LIBALLOC_PAGEALLOCATOR_H
#define __LIBALLOC_PAGEALLOCATOR_H

#include <FreeNOS/System.h>
#include <Types.h>
#include "Allocator.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup liballoc
 * @{
 */

/**
 * Allocates virtual memory using the memory server.
 */
class PageAllocator : public Allocator
{
  public:

    /**
     * Class constructor.
     *
     * @param base Starting address to allocate.
     * @param size Maximum size in bytes.
     */
    PageAllocator(Address start, Size size);

    /**
     * Get base address.
     *
     * Returns the first address of the allocated memory region.
     *
     * @return Start address.
     */
    Address base() const;

    /**
     * Get memory size.
     *
     * @return Size of memory owned by the Allocator.
     */
    virtual Size size() const;

    /**
     * Get memory available.
     *
     * @return Size of memory available by the Allocator.
     */
    virtual Size available() const;

    /**
     * Allocate memory.
     *
     * @param args Allocator arguments containing the requested size, address and alignment.
     *
     * @return Result value.
     */
    virtual Result allocate(Arguments & args);

    /**
     * Release memory.
     *
     * @param addr Points to memory previously returned by allocate().
     * @return Result value.
     *
     * @see allocate
     */
    virtual Result release(Address addr);

  private:

    /** Start of the allocated memory region. */
    Address m_base;

    /** Maximum size to allocate */
    Size m_size;

    /** Total number of bytes allocated. */
    Size m_allocated;
};

/**
 * @}
 * @}
 */

#endif /* __LIBALLOC_PAGEALLOCATOR_H */
