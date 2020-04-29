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

#ifndef __LIBTEST_TESTSUITE_H
#define __LIBTEST_TESTSUITE_H

#include <Singleton.h>
#include <List.h>

class TestInstance;

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libtest
 * @{
 */

/**
 * Maintains a list of test instances
 */
class TestSuite : public Singleton<TestSuite>
{
  public:

    /**
     * Class constructor
     */
    TestSuite();

    /**
     * Add a test
     *
     * @param test TestInstance to add
     */
    void addTest(TestInstance *test);

    /**
     * Retrieve a list of all tests
     *
     * @return List of TestInstances
     */
    List<TestInstance *> * getTests();

  private:

    /** List of TestInstances in the suite */
    List<TestInstance *> m_tests;
};

/**
 * @}
 * @}
 */

#endif /* __LIBTEST_TESTSUITE_H */
