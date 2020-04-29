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

#ifndef __LIBTEST_TESTREPORTER_H
#define __LIBTEST_TESTREPORTER_H

#include <Types.h>
#include "TestCase.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libtest
 * @{
 */

/**
 * Responsible for outputting test results.
 */
class TestReporter
{
  public:

    /**
     * Constructor.
     */
    TestReporter(int argc, char **argv);

    /**
     * Destructor.
     */
    virtual ~TestReporter();

    /**
     * Get OK count.
     */
    uint getOk() const;

    /**
     * Get fail count.
     */
    uint getFailed() const;

    /**
     * Get skip count.
     */
    uint getSkipped() const;

    /**
     * Set reporting on/off.
     */
    void setReport(bool value);

    /**
     * Set final statistics on/off.
     */
    void setStatistics(bool value);

    /**
     * Set multine mode on/off.
     */
    void setMultiline(bool value);

    /**
     * Prepare for next test.
     */
    virtual void prepare(TestInstance & test);

    /**
     * Collect test statistics.
     */
    virtual void collect(TestInstance & test, TestResult & result);

    /**
     * Begin testing.
     */
    virtual void begin(List<TestInstance *> & tests);

    /**
     * Finish testing.
     */
    virtual void finish(List<TestInstance *> & tests);

  protected:

    /**
     * Report start of testing.
     */
    virtual void reportBegin(List<TestInstance *> & tests) = 0;

    /**
     * Report start of a test.
     */
    virtual void reportBefore(TestInstance & test) = 0;

    /**
     * Report finish of a test.
     */
    virtual void reportAfter(TestInstance & test, TestResult & result) = 0;

    /**
     * Report completion of all tests.
     */
    virtual void reportFinish(List<TestInstance *> & tests) = 0;

  protected:

    /** Argument count */
    int m_argc;

    /** Argument values */
    char ** m_argv;

    /** Report on/off */
    bool m_report;

    /** Final statistics on/off */
    bool m_statistics;

    /** Multi line output */
    bool m_multiline;

    /** Test statistics */
    uint m_ok, m_fail, m_skip;
};

/**
 * @}
 * @}
 */

#endif /* __LIBTEST_TESTREPORTER_H */
