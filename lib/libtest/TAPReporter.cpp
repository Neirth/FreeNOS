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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TAPReporter.h"

TAPReporter::TAPReporter(int argc, char **argv)
    : TestReporter(argc, argv)
{
    m_count = 1;
}

void TAPReporter::reportBegin(List<TestInstance *> & tests)
{
    if (!m_multiline)
        printf("1..%d # Start %s\r\n", tests.count(), m_argv[0]);

#ifdef __HOST__
    fflush(stdout);
#endif /* __HOST__ */
}

void TAPReporter::reportBefore(TestInstance & test)
{
}

void TAPReporter::reportAfter(TestInstance & test, TestResult & result)
{
    if (!m_multiline)
    {
        switch (result.getResult())
        {
            case TestResult::Success: printf("ok %d %s\r\n", m_count, *test.getName()); break;
            case TestResult::Failure: printf("not ok %d %s %s\r\n", m_count, *test.getName(), *result.getDescription()); break;
            case TestResult::Skipped: printf("ok %d %s # SKIP\r\n", m_count, *test.getName()); break;
        }
        m_count++;
    }
    else
    {
        switch (result.getResult())
        {
            case TestResult::Success: printf("# Finish %s OK\r\n", *test.getName()); break;
            case TestResult::Failure: printf("# Finish %s FAIL\r\n", *test.getName()); break;
            case TestResult::Skipped: printf("# Finish %s SKIP\r\n", *test.getName()); break;
        }
    }

#ifdef __HOST__
    fflush(stdout);
#endif /* __HOST__ */
}

void TAPReporter::reportFinish(List<TestInstance *> & tests)
{
    if (m_multiline)
    {
        printf("# Completed ");

        if (m_fail)
            printf("FAIL ");
        else
            printf("OK ");

        printf("(%d passed %d failed %d skipped %d total)\r\n",
                m_ok, m_fail, m_skip, (m_ok + m_fail + m_skip));

#ifdef __HOST__
        fflush(stdout);
#endif /* __HOST__ */
    }
}
