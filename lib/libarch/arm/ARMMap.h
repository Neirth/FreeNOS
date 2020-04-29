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

#ifndef __LIBARCH_ARMMAP_H
#define __LIBARCH_ARMMAP_H

#include <MemoryMap.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_arm
 * @{
 */

/**
 * Memory mapping for the kernel and user processes on the ARM architecture
 */
class ARMMap : public MemoryMap
{
  public:

    /**
     * Constructor.
     */
    ARMMap();
};

namespace Arch
{
    typedef ARMMap MemoryMap;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_ARMMAP_H */
