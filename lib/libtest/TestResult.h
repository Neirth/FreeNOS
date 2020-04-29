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

#ifndef __LIBTEST_TESTRESULT_H
#define __LIBTEST_TESTRESULT_H

#include <String.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libtest
 * @{
 */

/**
 * @name TestResult macros
 * @{
 */

#define OK   TestResult( TestResult::Success )
#define FAIL TestResult( TestResult::Failure )
#define SKIP TestResult( TestResult::Skipped )

/**
 * @}
 */

/**
 * Represents a Test result created by a TestInstance.
 */
class TestResult
{
  public:

    /**
     * Result codes.
     */
    enum Result
    {
        Success,
        Failure,
        Skipped
    };

  public:

    /**
     * Constructor
     */
    TestResult(Result result, const char *description = "");

    /**
     * Check if the test passed.
     */
    bool isOK() const;

    /**
     * Check if the test failed.
     */
    bool isFailed() const;

    /**
     * Check if the test is skipped.
     */
    bool isSkipped() const;

    /**
     * Get result code.
     */
    const Result getResult() const;

    /**
     * Get result description.
     */
    String & getDescription();

  private:

    /** The result code for this test. */
    const Result m_result;

    /** Text describing the result. */
    String m_description;
};

/**
 * @}
 * @}
 */

#endif /* __LIBTEST_TESTRESULT_H */
