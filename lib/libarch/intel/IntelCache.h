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

#ifndef __LIBARCH_INTEL_INTELCACHE_H
#define __LIBARCH_INTEL_INTELCACHE_H

#include <Types.h>
#include <Macros.h>
#include <Cache.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_intel
 * @{
 */

/**
 * Intel cache management implementation.
 */
class IntelCache : public Cache
{
  public:

    /**
     * Invalidate the entire cache.
     *
     * @param type Cache type to invalidate.
     *
     * @return Result code
     */
    virtual Result invalidate(Type type);

    /**
     * Clean and invalidate entire cache.
     *
     * This operation will clean and invalidate the
     * entire given cache.
     *
     * @param type Cache type to clean and invalidate.
     *
     * @return Result code
     */
    virtual Result cleanInvalidate(Type type);

    /**
     * Clean and invalidate one memory page.
     *
     * @param type Cache type to clean and invalidate.
     * @param addr Virtual memory address of the page to clean and invalidate
     *
     * @return Result code
     */
    virtual Result cleanInvalidateAddress(Type type, Address addr);

    /**
     * Clean one memory page.
     *
     * @param type Cache type to clean
     * @param addr Virtual memory address of the page to clean and invalidate
     *
     * @return Result code
     */
    virtual Result cleanAddress(Type type, Address addr);
};

namespace Arch
{
    typedef IntelCache Cache;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_INTEL_INTELCACHE_H */
