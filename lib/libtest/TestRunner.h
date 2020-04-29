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

#ifndef __LIBTEST_TESTRUNNER_H
#define __LIBTEST_TESTRUNNER_H

#include "TestCase.h"

class TestReporter;

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libtest
 * @{
 */

/**
 * Reponsible for discovering and running tests
 */
class TestRunner
{
  public:

    /**
     * Class constructor
     *
     * @param argc Program argument count
     * @param argv Program argument values
     */
    TestRunner(int argc, char **argv);

    /**
     * Destructor
     */
    virtual ~TestRunner();

    /**
     * Get test reporter
     *
     * @return TestReporter pointer
     */
    TestReporter * getReporter();

    /**
     * Run all discovered tests
     *
     * @return Number of failed tests. Zero if success.
     */
    int run(void);

  protected:

    /** Program argument count */
    int m_argc;

    /** Program argument values */
    char **m_argv;

    /** Reports test results */
    TestReporter *m_reporter;
};

/**
 * @}
 * @}
 */

#endif /* __LIBTEST_TESTRUNNER_H */
