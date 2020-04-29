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

#ifndef __LIBTEST_EXTERNALTEST_H
#define __LIBTEST_EXTERNALTEST_H

#include "TestInstance.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libtest
 * @{
 */

/**
 * Represents external test program
 */
class ExternalTest : public TestInstance
{
  public:

    /**
     * Class constructor
     *
     * @param name Test progran name
     * @param argc Program argument count
     * @param argv Program argument values
     */
    ExternalTest(const char *name, int argc, char **argv);

    /**
     * Run the external test
     *
     * @return TestResult
     */
    virtual TestResult run();

  private:

    /** Program argument count */
    int m_argc;

    /** Program argument values */
    char ** m_argv;
};

/**
 * @}
 * @}
 */

#endif /* __LIBTEST_EXTERNALTEST_H */
