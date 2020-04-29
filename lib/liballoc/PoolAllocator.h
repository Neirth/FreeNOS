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

#ifndef __LIBALLOC_POOLALLOCATOR_H
#define __LIBALLOC_POOLALLOCATOR_H

#include <Types.h>
#include <Macros.h>
#include "Allocator.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup liballoc
 * @{
 */

/** Minimum power of two for a pool size. */
#define POOL_MIN_POWER 2

/** Maximum power of two size a pool can be. */
#define POOL_MAX_POWER 32

/**
 * Allocate this many blocks per default for the given size.
 *
 * @param size Size of each block.
 */
#define POOL_MIN_COUNT(size) \
    (64 / (((size) / 1024 ) + 1)) > 0 ? \
    (64 / (((size) / 1024 ) + 1)) : 1

/**
 * @brief Calculates the number of bytes needed in a bitmap,
 *        to hold the specified number of elements.
 * @param count Number of elements to hold in the bitmap.
 */
#define BITMAP_NUM_BYTES(count) \
    ((count / 8) + 1)

/**
 * Memory pool contains pre-allocated blocks of a certain size (power of two).
 */
typedef struct MemoryPool
{
    /**
     * Marks the appropriate bits in the free and used block bitmap.
     *
     * @return Pointer to the next available block, if any.
     */
    Address allocate()
    {
        Address *ptr;
        Size num = count / sizeof(Address);

        // At least one
        if (!num) num++;

        // Scan bitmap as fast as possible
        for (Size i = 0; i < num; i++)
        {
            // Point to the correct offset
            ptr = (Address *) (&blocks) + i;

            // Any blocks free?
            if (*ptr != (Address) ~ZERO)
            {
                // Find the first free bit
                for (Size bit = 0; bit < sizeof(Address) * 8; bit++)
                {
                    if (!(*ptr & 1 << bit))
                    {
                        *ptr |= (1 << bit);
                        free--;
                        return addr + (((i * sizeof(Address) * 8) + bit) * size);
                    }
                }
            }
        }
        // Out of memory
        return ZERO;
    }

    /**
     * Unmarks the appropriate bit for the given address.
     *
     * @param a Address to unmark.
     */
    void release(Address a)
    {
        Size index = (a - addr) / size / 8;
        Size bit   = (a - addr) / size % 8;

        free++;
        blocks[index] &= ~(1 << bit);
    }

    /** Points to the next pool of this size (if any). */
    MemoryPool *next;

    /** Memory address allocated to this pool. */
    Address addr;

    /** Size of each object in the pool. */
    Size size;

    /** Number of blocks in the pool. */
    Size count;

    /** Free blocks left. */
    Size free;

    /** Bitmap which represents free and used blocks. */
    u8 blocks[];
}
MemoryPool;

/**
 * Memory allocator which uses pools.
 *
 * Allocates memory from pools the size of a power of two.
 * Each pool is pre-allocated and has a bitmap representing free blocks.
 */
class PoolAllocator : public Allocator
{
  public:

    /**
     * Constructor
     */
    PoolAllocator();

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
     *
     * @return Result value.
     *
     * @see allocate
     */
    virtual Result release(Address addr);

  private:

    /**
     * Creates a new MemoryPool instance.
     *
     * @param index Index in the pools array.
     * @param cnt Allocate for this many of blocks from our parent.
     *
     * @return Pointer to a MemoryPool object on success, ZERO on failure.
     */
    MemoryPool * newPool(Size index, Size cnt);

  private:

    /** Array of memory pools. Index represents the power of two. */
    MemoryPool *m_pools[POOL_MAX_POWER];
};

/**
 * @}
 * @}
 */

#endif /* __LIBALLOC_POOLALLOCATOR_H */
