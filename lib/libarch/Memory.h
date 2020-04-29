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

#ifndef __LIBARCH_MEMORY_H
#define __LIBARCH_MEMORY_H

#include <Types.h>
#include <BitOperations.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 */

namespace Memory
{
    /**
     * Memory access flags.
     */
    typedef enum Access
    {
        None        = 0,
        Readable    = 1 << 0,
        Writable    = 1 << 1,
        Executable  = 1 << 2,
        User        = 1 << 3,
        Uncached    = 1 << 4,
        InnerCached = 1 << 5,
        OuterCached = 1 << 6,
        Device      = 1 << 7
    }
    Access;

    /**
     * Memory range.
     */
    typedef struct Range
    {
        Address virt;   /**< Virtual address. */
        Address phys;   /**< Physical address. */
        Size    size;   /**< Size in number of bytes. */
        Access  access; /**< Page access flags. */
    }
    Range;
};

/**
 * @}
 * @}
 */

#endif /* __LIBARCH_MEMORY_H */
