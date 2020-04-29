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

#ifndef __LIBARCH_MEMORYMAP_H
#define __LIBARCH_MEMORYMAP_H

#include "Memory.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 */

#define MEMORYMAP_MAX_REGIONS 8

/**
 * Describes virtual memory map layout
 */
class MemoryMap
{
  public:

    /**
     * Memory regions.
     *
     * Various memory regions which are available on the system.
     * Memory regions are labels which refer to predefined virtual memory ranges.
     * The virtual memory address range of each region depends on the
     * architecture implementation and can be obtained with region().
     *
     * @see region
     */
    typedef enum Region
    {
        KernelData,    /**<< Kernel program data from libexec, e.g. code, bss, (ro)data */
        KernelPrivate, /**<< Kernel dynamic memory mappings */
        UserData,      /**<< User program data from libexec, e.g. code, bss, (ro)data */
        UserHeap,      /**<< User heap */
        UserStack,     /**<< User stack */
        UserPrivate,   /**<< User private dynamic memory mappings */
        UserShare,     /**<< User shared dynamic memory mappings */
        UserArgs       /**<< Used for copying program arguments and file descriptors */
    }
    Region;

    /**
     * Constructor.
     */
    MemoryMap();

    /**
     * Copy Constructor.
     */
    MemoryMap(const MemoryMap &map);

    /**
     * Get memory range for the given region.
     *
     * @param region Memory region.
     *
     * @return Memory::Range object
     */
    Memory::Range range(Region region) const;

    /**
     * Set memory range for the given region.
     *
     * @param region Identifier of the memory region to set
     * @param range New memory range value for the memory region
     */
    void setRange(Region region, Memory::Range range);

  protected:

    /** Memory ranges. */
    Memory::Range m_regions[MEMORYMAP_MAX_REGIONS];
};

/**
 * @}
 * @}
 */

#endif /* __LIBARCH_MEMORYMAP_H */
