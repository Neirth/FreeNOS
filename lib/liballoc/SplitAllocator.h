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

#ifndef __LIBALLOC_SPLITALLOCATOR_H
#define __LIBALLOC_SPLITALLOCATOR_H

#include <Types.h>
#include "Allocator.h"
#include "BitAllocator.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup liballoc
 * @{
 */

/**
 * Allocator which separates kernel mapped low-memory and higher memory.
 */
class SplitAllocator : public Allocator
{
  public:

    /**
     * Class constructor.
     *
     * @param low Lower physical memory.
     * @param high Higher physical memory.
     */
    SplitAllocator(Memory::Range low, Memory::Range high);

    /**
     * Class destructor.
     */
    virtual ~SplitAllocator();

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
     * Allocate address.
     *
     * @param addr Allocate a specific address.
     *
     * @return Result value.
     */
    Result allocate(Address addr);

    /**
     * Allocate from lower memory.
     *
     * @param args Allocator arguments containing the requested size, address and alignment.
     *
     * @return Result code
     */
    Result allocateLow(Arguments & args);

    /**
     * Allocate from high memory.
     *
     * @param args Allocator arguments containing the requested size, address and alignment.
     *
     * @return Result code
     */
    Result allocateHigh(Arguments & args);

    /**
     * Release memory.
     *
     * @param addr Points to memory previously returned by allocate().
     *
     * @return Result value.
     *
     * @see allocate
     */
    virtual Result release(Address addr);

    /**
     * Convert the given physical address to lower virtual accessible address.
     */
    void * toVirtual(Address phys) const;

    /**
     * Convert lower virtual address back to system level physical address.
     */
    void * toPhysical(Address virt) const;

  private:

    /** Physical memory allocator. */
    BitAllocator *m_alloc;

    /** Low memory */
    Memory::Range m_low;

    /** High memory */
    Memory::Range m_high;
};

/**
 * @}
 * @}
 */

#endif /* __LIBALLOC_SPLITALLOCATOR_H */
