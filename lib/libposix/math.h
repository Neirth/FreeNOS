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

#ifndef __LIBPOSIX_MATH_H
#define __LIBPOSIX_MATH_H

#include <Macros.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/**
 * Compute the absolute value of a number.
 *
 * The absolute value of a number turns it to positive
 * in case it is negative. Basically it just removes the sign (-) from a number.
 * For example, -3 returns 3, and 8 returns 8.
 *
 * @param number The number to make absolute.
 *
 * @return Absolute value of the given number.
 */
extern C int abs(int number);

/**
 * Compute the square root of a number.
 *
 * @param number The number to use for square root.
 *
 * @return Square root of the given number.
 */
extern C u32 sqrt(u32 number);

/**
 * @}
 * @}
 */

#endif /* __LIBPOSIX_MATH_H */
