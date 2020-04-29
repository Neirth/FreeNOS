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

#include <string.h>
#include <ListIterator.h>
#include "TestCase.h"
#include "TestSuite.h"
#include "TestRunner.h"
#include "StdoutReporter.h"
#include "TAPReporter.h"
#include "XMLReporter.h"

TestRunner::TestRunner(int argc, char **argv)
{
    // Set member default values.
    m_argc = argc;
    m_argv = argv;
    m_reporter = new StdoutReporter(argc, argv);

    // Check for command-line specified arguments.
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--tap") == 0)
        {
            if (m_reporter)
                delete m_reporter;

            m_reporter = new TAPReporter(argc, argv);
            break;
        }
        else if (strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--xml") == 0)
        {
            if (m_reporter)
                delete m_reporter;

            m_reporter = new XMLReporter(argc, argv);
            break;
        }
        else if (strcmp(argv[i], "-n") == 0)
        {
            m_reporter->setStatistics(false);
        }
    }
}

TestRunner::~TestRunner()
{
    delete m_reporter;
}

TestReporter * TestRunner::getReporter()
{
    return m_reporter;
}

int TestRunner::run(void)
{
    // Prepare for testing.
    List<TestInstance *> *tests = TestSuite::instance->getTests();
    m_reporter->begin(*tests);

    // Execute tests. Report per-test stats.
    for (ListIterator<TestInstance *> i(tests); i.hasCurrent(); i++)
    {
        TestInstance *test = i.current();
        if (!test)
            break;

        m_reporter->prepare(*test);
        TestResult result = test->run();
        m_reporter->collect(*test, result);
    }
    // Finish testing. Report final stats.
    m_reporter->finish(*tests);
    return m_reporter->getFailed();
}
