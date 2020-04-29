/*
 * Copyright (C) 2019 Niek Linnenbank
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
#include "XMLReporter.h"

XMLReporter::XMLReporter(int argc, char **argv)
    : TestReporter(argc, argv)
{
}

void XMLReporter::reportBegin(List<TestInstance *> & tests)
{
    if (m_multiline)
    {
        printf("<!-- Init %s -->\r\n",
                m_argv[0]);
    }
    else
    {
        printf("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r\n"
               "<testsuites id=\"%s\" name=\"%s\">\r\n"
               "<testsuite id=\"%s\" name=\"%s\" tests=\"%d\">\r\n",
                m_argv[0], m_argv[0],
                m_argv[0], m_argv[0], tests.count());
    }

#ifdef __HOST__
    fflush(stdout);
#endif /* __HOST__ */
}

void XMLReporter::reportBefore(TestInstance & test)
{
    if (m_multiline)
    {
        printf("<!-- Start %s -->\r\n", *test.m_name);
    }
    else
    {
        printf("   <testcase id=\"%s.%s\" name=\"%s.%s\">\r\n",
                m_argv[0], *test.m_name,
                m_argv[0], *test.m_name);
    }

#ifdef __HOST__
    fflush(stdout);
#endif /* __HOST__ */
}

void XMLReporter::reportAfter(TestInstance & test, TestResult & result)
{
    if (m_multiline)
    {
        switch (result.getResult())
        {
            case TestResult::Success: printf("<!-- Finish %s OK -->\r\n", *test.m_name); break;
            case TestResult::Failure: printf("<!-- Finish %s FAIL -->\r\n", *test.m_name); break;
            case TestResult::Skipped: printf("<!-- Finish %s SKIP -->\r\n", *test.m_name); break;
        }
    }
    else
    {
        switch (result.getResult())
        {
            case TestResult::Success:
                printf("      <!-- OK -->\r\n");
                break;

            case TestResult::Failure:
                printf("      <failure message=\"%s\" type=\"error\" />\r\n",
                        *result.getDescription());
                break;

            case TestResult::Skipped:
                printf("      <skipped message=\"Skipped: %s\" type=\"unittest.case.SkipTest\">Skipped: %s</skipped>\r\n",
                        *test.m_name, *test.m_name);
                break;
        }
        printf("   </testcase>\r\n");
    }

#ifdef __HOST__
    fflush(stdout);
#endif /* __HOST__ */
}

void XMLReporter::reportFinish(List<TestInstance *> & tests)
{
    if (m_multiline)
        printf("</testsuites>\r\n"
               "<!-- Completed ");
    else
        printf("</testsuite>\r\n"
               "</testsuites>\r\n"
               "<!-- Result ");

    printf("%s (%d passed %d failed %d skipped %d total) -->\r\n",
            m_fail == 0 ? "OK" : "FAIL", m_ok, m_fail, m_skip, (m_ok + m_fail + m_skip));

#ifdef __HOST__
    fflush(stdout);
#endif /* __HOST__ */
}
