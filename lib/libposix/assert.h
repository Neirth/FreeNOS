/*
 * Copyright (C) 2010 Niek Linnenbank
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

#ifndef __LIBC_ASSERT_H
#define __LIBC_ASSERT_H

#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/**
 * @brief Insert program diagnostics.
 *
 * The assert() macro shall insert diagnostics into programs; it shall
 * expand to a void expression. When it is executed, if expression
 * (which shall have a scalar type) is false (that is, compares equal to 0),
 * assert() shall write information about the particular call that failed on
 * stderr and shall call abort().
 *
 * The information written about the call that failed shall include the text
 * of the argument, the name of the source file, the source file line number,
 * and the name of the enclosing function; the latter are, respectively, the
 * values of the preprocessing macros __FILE__ and __LINE__ and of the
 * identifier __func__.
 *
 * Forcing a definition of the name NDEBUG, either from the compiler command
 * line or with the preprocessor control statement #define NDEBUG ahead of
 * the #include <assert.h> statement, shall stop assertions from being
 * compiled into the program.
 *
 * @param exp Boolean expression.
 *
 * @return The assert() macro shall not return a value.
 */
#ifdef NDEBUG
#define assert(ignore) ((void) 0)
#else
#define assert(exp) \
    if (!(exp)) \
    { \
        printf("[%s:%d]: *** Assertion `%s' failed ***\n", \
                __FILE__, __LINE__, #exp); \
        exit(1); \
    }
#endif

/**
 * @}
 * @}
 */

#endif /* __LIBC_ASSERT_H */
