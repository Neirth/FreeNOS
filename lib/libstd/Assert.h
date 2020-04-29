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

#ifndef __ASSERT_H
#define __ASSERT_H

#if defined(__ASSERT__) && !defined(__HOST__)

#include "Macros.h"
#include "Types.h"

#ifdef __SERVER__
#include <LogMessage.h>
#include <stdlib.h>
#endif /* __SERVER__ */

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * @name Runtime assertion
 * @{
 */

/**
 * Function which prints a message, and then terminates itself.
 *
 * @param fmt Formatted message.
 * @param ... Argument list.
 */
extern C void __assertFailure(const char *fmt, ...);

/**
 * Verify that the given address is readable.
 *
 * @param addr Address to verify.
 */
extern C int __assertRead(Address addr);

/**
 * Verify that the given address is writeable.
 *
 * @param addr Address to verify.
 */
extern C int __assertWrite(Address addr);

/**
 * @}
 */

/**
 * @name Preprocessor assertion
 * @{
 */

/**
 * Invokes __assertFailure for applications, and log() + exit() for servers.
 *
 * @param fmt Formatted string.
 * @param ... Argument list.
 *
 * @return Never.
 */
#ifdef __SERVER__
#define raiseFailure(fmt, ...) \
    log(fmt, ##__VA_ARGS__); \
    exit(1);
#else
#define raiseFailure(fmt, ...) \
    __assertFailure(fmt, ##__VA_ARGS__);
#endif

/**
 * Verify that a given expression evaluates to true.
 *
 * @param exp Boolean expression.
 */
#define assert(exp) \
    if (!(exp)) \
    { \
        raiseFailure("[%s:%d]: *** Assertion `%s' failed ***\n", \
                     __FILE__, __LINE__, #exp); \
    }

/**
 * Presume that the given address is readable.
 *
 * @param addr Address to check.
 */
#define assertRead(addr) \
    assert(__assertRead((Address)addr))

/**
 * Presume that the given address is writeable.
 *
 * @param addr Address to check.
 */
#define assertWrite(addr) \
    assert(__assertWrite((Address)addr))

/**
 * @}
 */

#else

/**
 * @name Dummy preprocessor macros
 * @{
 */

/**
 * Dummy assertion function which does not perform any check.
 *
 * @param exp Expression is ignored.
 */
#define assert(exp)

/**
 * Dummy assertion function for checking read access on an address.
 *
 * @param addr Address is ignored.
 */
#define assertRead(addr)

/**
 * Dummy assertion function for checking read access on an address.
 *
 * @param addr Address is ignored.
 */
#define assertWrite(addr)

/**
 * @}
 */

/**
 * @}
 * @}
 */

#endif /* defined(__ASSERT__) && !defined(__HOST__) */
#endif /* __ASSERT_H */
