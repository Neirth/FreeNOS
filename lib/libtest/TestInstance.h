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

#ifndef __LIBTEST_TESTINSTANCE_H
#define __LIBTEST_TESTINSTANCE_H

#include <String.h>
#include "TestResult.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libtest
 * @{
 */

/**
 * Represents a test instance
 */
class TestInstance
{
  public:

    /**
     * Class constructor
     *
     * @param name Name of the test
     */
    TestInstance(const char *name);

    /**
     * Retrieve test instance name
     *
     * @return Test instance name
     */
    const String & getName() const;

    /**
     * Run the test instance
     *
     * @return TestResult
     */
    virtual TestResult run() = 0;

  protected:

    /** Name of the test instance */
    String m_name;
};

/**
 * @}
 * @}
 */

#endif /* __LIBTEST_TESTINSTANCE_H */
