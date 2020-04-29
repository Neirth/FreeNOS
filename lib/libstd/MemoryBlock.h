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

#ifndef __MEMORYBLOCK_H
#define __MEMORYBLOCK_H

#include "Types.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Memory block operations class
 */
class MemoryBlock
{
  public:

    /**
     * Fill memory with a constant byte.
     *
     * @param dest Memory to write to.
     * @param ch Constant byte.
     *
     * @return Pointer to dest.
     */
    static void * set(void *dest, int ch, unsigned count);

    /**
     * Copy memory from one place to another.
     *
     * @param dest Destination address.
     * @param src Source address.
     * @param count Number of bytes to copy.
     *
     * @return Number of bytes copied.
     */
    static Size copy(void *dest, const void *src, Size count);

    /**
     * Copy a character string.
     *
     * @param dest Destination string.
     * @param src Source string.
     * @param count Number of bytes to copy.
     *
     * @return Number of bytes copied
     */
    static Size copy(char *dest, char *src, Size count);

    /**
     * Compare memory.
     *
     * @param dest Destination address
     * @param src Source address
     * @param count Number of bytes to compare
     *
     * @return Zero if equal or non-zero if not equal
     */
    static bool compare(void *dest, void *src, Size count);

    /**
     * Compare memory.
     *
     * @param p1 Memory pointer one.
     * @param p2 Memory pointer two.
     * @param count Number of bytes to compare or zero to continue until a ZERO byte.
     *
     * @return True if equal, false otherwise.
     */
    static bool compare(const char *p1, const char *p2, Size count = 0);
};

/**
 * @}
 * @}
 */

#endif /* __MEMORYBLOCK_H */
