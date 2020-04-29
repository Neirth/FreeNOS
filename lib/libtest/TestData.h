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

#ifndef __LIBTEST_TESTDATA_H
#define __LIBTEST_TESTDATA_H

#include <Types.h>
#include <Macros.h>
#include <Vector.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __HOST__
#include <sys/time.h>
#else
#include <FreeNOS/System.h>
#endif /* __HOST__ */

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libtest
 * @{
 */

/**
 * Generate test data for a certain data type.
 */
template <class T> class TestData
{
  public:

    /**
     * Constructor.
     */
    TestData()
    {
        seed();
    }

    /**
     * Destructor.
     */
    virtual ~TestData()
    {
    }

    /**
     * Initialize the random number generator.
     */
    void seed()
    {
        // Collect seed from process id.
        pid_t pid = getpid();
        unsigned int seed = pid;

        // Collect seed from the current time
#ifdef __HOST__
        struct timeval tv;
        gettimeofday(&tv, (struct timezone *)NULL);
        seed += tv.tv_sec + tv.tv_usec;
#else
        seed += timestamp();
#endif
        // Seed the random generator
        srandom(seed);
    }

    /**
     * The number of generated values.
     */
    Size count() const
    {
        return m_values.count();
    }

    /**
     * Retrieve previously random generated test data by index.
     *
     * @param index Index of the value to retrieve.
     *
     * @return T value reference.
     */
    T & get(Size index)
    {
        return m_values[index];
    }

    /**
     * Retrieve previously random generated test data by index.
     */
    T & operator[](Size index)
    {
        return m_values[index];
    }

    /**
     * Get random test value(s).
     *
     * @param count Number or values to generate.
     *
     * @return The last generated T value.
     */
    virtual T random(Size count = 1) = 0;

    /**
     * Get unique random test value(s).
     *
     * @param count Number of unique values to generate.
     *
     * @return The last generated T value.
     */
    virtual T unique(Size count = 1) = 0;

  protected:

    /** Vector with generated values. */
    Vector<T> m_values;
};

/**
 * @}
 * @}
 */

#endif /* __LIBTEST_TESTDATA_H */
