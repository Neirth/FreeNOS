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

#ifndef __LIBTEST_TESTCASE_H
#define __LIBTEST_TESTCASE_H

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libtest
 * @{
 */

#ifndef private

/**
 * Redefine private as public for accessing class members in testcases
 *
 * Testcases sometimes need to access private members
 * of a class. This define makes sure that a TestCase
 * can access the private members.
 */
#define private public
#endif /* private */

/* Same for protected */
#ifndef protected
#define protected public
#endif

#include <MemoryBlock.h>
#include <stdio.h>
#include <List.h>
#include <Macros.h>
#include "LocalTest.h"

#define TestCase(name) \
    TestResult name (void); \
    LocalTest instance_##name (QUOTE(name), name); \
    TestResult name (void)

/**
 * Test if the given expression is true and return NOK otherwise.
 *
 * @param expression The expression to test against
 *
 * @return NOK only if the expression evaluates to false
 */
#define testAssert(expression) \
    if(!(expression)) \
    { \
        char msg[256]; \
        snprintf(msg, sizeof(msg), "%s:%d:%s testAssert failed: `%s' .. ", __FILE__, __LINE__,  __FUNCTION__, QUOTE(expression)); \
        return TestResult(TestResult::Failure, msg); \
    }

/**
 * Test if the given character strings are equal.
 */
#define testString(s1, s2) \
    testAssert(MemoryBlock::compare((s1), (s2)))

/**
 * @}
 * @}
 */

#endif /* __LIBTEST_TESTCASE_H */
