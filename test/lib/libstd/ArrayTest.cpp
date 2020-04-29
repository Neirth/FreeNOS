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

#include <TestCase.h>
#include <TestRunner.h>
#include <TestInt.h>
#include <TestMain.h>
#include <Array.h>
#include <String.h>

TestCase(ArrayConstruct)
{
    Array<int, 64> a;

    // Check the array has the correct size
    testAssert(a.size() == 64);
    return OK;
}

TestCase(ArrayOfStrings)
{
    Array<String, 64> a;

    // Fill the array with a String value
    a.fill(String("test"));

    // The array should be filled with the test value
    for (Size i = 0; i < 64; i++)
    {
        testAssert(a[i].equals("test"));
    }

    // Check administration
    testAssert(a.size() == 64);
    testAssert(a.count() == 64);
    return OK;
}

TestCase(ArrayFill)
{
    TestInt<int> ints(INT_MIN, INT_MAX);
    Array<int, 64> a;

    // Fill the array with a random value
    a.fill(ints.random());

    // Check the array is completely filled with the value
    for (Size i = 0; i < 64; i++)
    {
        testAssert(a[i] == ints[0]);
    }
    return OK;
}

TestCase(ArrayOverflow)
{
    Array<int, 64> a;

    // Try to overflow the array
    testAssert(!a.insert(64, 0));
    testAssert(!a.insert(65, 0));

    return OK;
}

TestCase(ArrayOverwrite)
{
    TestInt<int> ints(INT_MIN, INT_MAX);
    Array<int, 64> a;

    // Write first item
    a.insert(0, ints.random());
    testAssert(a[0] == ints[0]);

    // Overwrite first item many times
    for (Size i = 0; i < 64; i++)
    {
        a.insert(0, ints.random());
        testAssert(a[0] == ints[i+1]);
    }
    return OK;
}

TestCase(ArrayPutSeq)
{
    TestInt<int> ints(INT_MIN, INT_MAX);
    Array<int, 256> a;

    // Clear the array
    a.fill(0);

    // Fill the array sequentially
    for (Size i = 0; i < 256; i++)
    {
        // Generate value and insert it at the current index
        a[i] = ints.random();

        // Assert that all previously inserted values are still there
        // and all other places are ZERO.
        for (Size j = 0; j < 256; j++)
        {
            if (j <= i)
            {
                testAssert(a[j] == ints[j]);
            }
            else
            {
                testAssert(a[j] == ZERO);
            }
        }
    }
    return OK;
}

TestCase(ArrayPutRandom)
{
    TestInt<int> ints(INT_MIN, INT_MAX);
    TestInt<uint> sizes(32, 128);
    TestInt<uint> indexes(0, 255);
    Array<int, 256> a;
    uint count = sizes.random();

    // Retrieve random indexes
    indexes.unique(256);

    // Clear the array
    a.fill(0);

    // Fill the array randomly
    for (Size i = 0; i < count; i++)
    {
        // Generate value and insert it at a random index
        a.insert(indexes[i], ints.random());

        // Assert that all previously inserted values are still there
        // and that empty places are ZERO.
        for (Size j = 0; j < count; j++)
        {
            if (j <= i)
            {
                testAssert(a[indexes[j]] == ints[j]);
            }
            else
            {
                testAssert(a[indexes[j]] == ZERO);
            }
        }
    }
    return OK;
}

TestCase(ArrayCompare)
{
    TestInt<int> ints(INT_MIN, INT_MAX);
    Array<int, 64> a1, a2;

    // Fill the arrays completely
    for (Size i = 0; i < 64; i++)
    {
        // Generate value and insert it at the current index
        a1.insert(i, ints.random());
        a2.insert(i, ints[i]);
    }

    // Both arrays must be equal
    testAssert(a1.compareTo(a2) == 0);
    testAssert(a1.equals(a2));
    testAssert(a1.size() == a2.size());
    testAssert(a1.count() == a2.count());

    // Change one item. Arrays cannot be equal
    a1.insert(0, ~(a1[0]));
    testAssert(a1.compareTo(a2) != 0);
    testAssert(!a1.equals(a2));
    testAssert(a1.size() == a2.size());
    testAssert(a1.count() == a2.count());
    return OK;
}
