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

#ifndef __LIBTEST_TESTINT_H
#define __LIBTEST_TESTINT_H

#include "TestData.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libtest
 * @{
 */

/**
 * Generate test data for integers.
 */
template <class T> class TestInt : public TestData<T>
{
  public:

    /**
     * Constructor.
     */
    TestInt(T min, T max) : TestData<T>()
    {
        m_min = min;
        m_max = max;
    }

    /**
     * Get random test value(s).
     *
     * @param count Number or values to generate.
     *
     * @return The last generated T value.
     */
    virtual T random(Size count = 1);

    /**
     * Get unique random test value(s).
     *
     * @param count Number of unique values to generate.
     *
     * @return The last generated T value.
     */
    virtual T unique(Size count = 1);

  private:

    /** Maximum value */
    T m_max;

    /** Minimum value */
    T m_min;
};

template<> int TestInt<int>::random(Size count)
{
    int value = 0;

    while (count--)
    {
        value = (::random() % m_max);

        if (value < m_min)
            value = m_min;

        m_values.insert(value);
        break;
    }
    return value;
}

template<> int TestInt<int>::unique(Size count)
{
    // Save current count
    int offset = (int) m_values.count();

    // First put values sequentially
    for (int i = 0; i < (int)count; i++)
        m_values.insert(i + m_min);

    // Randomize values by swapping
    for (int i = offset; i < ((int)count + offset); i++)
    {
        int tmp = m_values[i];

        int idx = (::random() % (int)count - 1);
        if (idx < offset)
            idx = offset;

        m_values[i]   = m_values[idx];
        m_values[idx] = tmp;
    }
    // Success
    return m_values[offset + (int)count - 1];
}

template<> uint TestInt<uint>::random(Size count)
{
    uint value = 0;

    while (count--)
    {
        value = (((unsigned)::random() + (unsigned)::random()) % m_max);

        if (value < m_min)
            value = m_min;

        m_values.insert(value);
        break;
    }
    return value;
}

template<> uint TestInt<uint>::unique(Size count)
{
    // Save current count
    Size offset = m_values.count();

    // First put values sequentially
    for (Size i = 0; i < count; i++)
        m_values.insert(i + m_min);

    // Randomize values by swapping
    for (Size i = offset; i < (count + offset); i++)
    {
        Size tmp = m_values[i];

        Size idx = (::random() % (count - 1));
        if (idx < offset)
            idx = offset;

        m_values[i]   = m_values[idx];
        m_values[idx] = tmp;
    }
    // Success
    return m_values[offset + count - 1];
}

/**
 * @}
 * @}
 */

#endif /* __LIBTEST_TESTINT_H */
