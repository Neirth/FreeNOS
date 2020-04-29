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

#ifndef __LIBTEST_LOCALTEST_H
#define __LIBTEST_LOCALTEST_H

#include "TestInstance.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libtest
 * @{
 */

typedef TestResult TestFunction(void);

/**
 * Represents test inside the same process
 */
class LocalTest : public TestInstance
{
  public:

    /**
     * Class constructor
     *
     * @param name Test name
     * @param func Test function to run
     */
    LocalTest(const char *name, TestFunction func);

    /**
     * Run the test function
     *
     * @return TestResult
     */
    virtual TestResult run();

  private:

    /** Contains the test to run */
    TestFunction *m_func;
};

/**
 * @}
 * @}
 */

#endif /* __LIBTEST_LOCALTEST_H */
