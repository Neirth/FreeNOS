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

#ifndef __LIBSTD_HASH_FUNCTION_H
#define __LIBSTD_HASH_FUNCTION_H

#include "Types.h"
#include "String.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/** Prime number used by FNV hashing. */
#define FNV_PRIME 16777619

/** Initial value of the FNV internal state. */
#define FNV_INIT  0x811c9dc5

/**
 * Compute a hash using the FNV algorithm.
 *
 * @param key Key string to hash.
 * @param mod Modulo value.
 *
 * @return Computed hash.
 */
Size hash(const String & key, Size mod);

/**
 * Compute a hash using the FNV algorithm.
 *
 * @param key Integer key to hash.
 * @param mod Modulo value.
 *
 * @return Computed hash.
 */
Size hash(int key, Size mod);

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_HASH_FUNCTION_H */
