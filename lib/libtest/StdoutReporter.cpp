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
#include <libgen.h>
#include <ListIterator.h>
#include <TerminalCodes.h>
#include "TestCase.h"
#include "StdoutReporter.h"

StdoutReporter::StdoutReporter(int argc, char **argv)
    : TestReporter(argc, argv)
{
}

void StdoutReporter::reportBegin(List<TestInstance *> & tests)
{
    if (m_multiline)
        printf("%s%s: running %d tests\r\n", WHITE, basename(m_argv[0]), tests.count());

#ifdef __HOST__
    fflush(stdout);
#endif /* __HOST__ */
}

void StdoutReporter::reportBefore(TestInstance & test)
{
    printf("%s%s: %s", WHITE, basename(m_argv[0]), *test.m_name);

    if (m_multiline)
        printf("\r\n");
    else
        printf(" .. ");

#ifdef __HOST__
    fflush(stdout);
#endif /* __HOST__ */
}

void StdoutReporter::reportAfter(TestInstance & test, TestResult & result)
{
    if (m_multiline)
        printf("%s%s: %s .. ", WHITE, basename(m_argv[0]), *test.m_name);

    switch (result.getResult())
    {
        case TestResult::Success: printf("%sOK\r\n", GREEN); break;
        case TestResult::Failure: printf("%sFAIL\r\n%s\r\n", RED, *result.getDescription()); break;
        case TestResult::Skipped: printf("%sSKIP\r\n", YELLOW); break;
    }
    printf("%s", WHITE);

    if (m_multiline)
        printf("\r\n");

#ifdef __HOST__
    fflush(stdout);
#endif /* __HOST__ */
}

void StdoutReporter::reportFinish(List<TestInstance *> & tests)
{
    if (m_fail)
        printf("%s: %sFAIL%s   ", basename(m_argv[0]), RED, WHITE);
    else
        printf("%s: %sOK%s   ", basename(m_argv[0]), GREEN, WHITE);

    printf("(%d passed %d failed %d skipped %d total)\r\n",
            m_ok, m_fail, m_skip, (m_ok + m_fail + m_skip));

#ifdef __HOST__
    fflush(stdout);
#endif /* __HOST__ */
}
