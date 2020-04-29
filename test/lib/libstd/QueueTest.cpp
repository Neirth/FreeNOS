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

#include <TestCase.h>
#include <TestRunner.h>
#include <TestInt.h>
#include <TestMain.h>
#include <Queue.h>
#include <String.h>

TestCase(QueueConstruct)
{
    Queue<int, 64> q;

    // Check the Queue has the correct size and count
    testAssert(q.size() == 64);
    testAssert(q.count() == 0);
    return OK;
}

TestCase(QueuePushPopString)
{
    Queue<String, 64> q;

    // Append a string
    q.push(String("test"));
    testAssert(q.count() == 1);
    testAssert(q.size() == 64);

    // Remove the string
    String & str = q.pop();
    testAssert(q.count() == 0);
    testAssert(q.size() == 64);
    testAssert(str.equals(String("test")));

    return OK;
}

TestCase(QueuePushPopInt)
{
    Queue<int, 64> q;
    TestInt<int> ints(INT_MIN, INT_MAX);

    // Append a number
    q.push(ints.random());
    testAssert(q.count() == 1);
    testAssert(q.size() == 64);

    // Remove the number
    int n = q.pop();
    testAssert(q.count() == 0);
    testAssert(q.size() == 64);
    testAssert(n == ints[0]);

    return OK;
}

TestCase(QueueCycle)
{
    Queue<int, 64> q;
    TestInt<int> ints(INT_MIN, INT_MAX);

    // Fill the queue halfway, ten times
    for (Size j = 0; j < 10; j++)
    {
        for (Size i = 0; i < 32; i++)
        {
            q.push(ints.random());
            testAssert(q.count() == (i + 1));
            testAssert(q.size() == 64);
        }

        // Empty the queue
        for (Size i = 0; i < 32; i++)
        {
            int n = q.pop();

            testAssert(n == ints[i + (j * 32)]);
            testAssert(q.count() == (32 - i - 1));
            testAssert(q.size() == 64);
        }
    }

    // Queue is empty again
    testAssert(q.count() == 0);
    testAssert(q.size() == 64);

    return OK;
}


TestCase(QueueFull)
{
    Queue<int, 64> q;
    TestInt<int> ints(INT_MIN, INT_MAX);

    // Fill the queue
    for (Size i = 0; i < 64; i++)
    {
        q.push(ints.random());
        testAssert(q.count() == (i + 1));
        testAssert(q.size() == 64);
    }

    // Now the queue is full, so a push must fail
    testAssert(!q.push(ints.random()));

    // Empty the queue
    for (Size i = 0; i < 64; i++)
    {
        int n = q.pop();

        testAssert(n == ints[i]);
        testAssert(q.count() == (64 - i - 1));
        testAssert(q.size() == 64);
    }

    // Queue is empty again
    testAssert(q.count() == 0);
    testAssert(q.size() == 64);

    return OK;
}

TestCase(QueueClear)
{
    Queue<int, 64> q;
    TestInt<int> ints(INT_MIN, INT_MAX);

    // Fill the queue
    for (Size i = 0; i < 64; i++)
    {
        q.push(ints.random());
        testAssert(q.count() == (i + 1));
        testAssert(q.size() == 64);
    }

    // Clear it
    q.clear();
    testAssert(q.count() == 0);
    testAssert(q.size() == 64);

    return OK;
}

TestCase(QueueContains)
{
    Queue<int, 64> q;
    TestInt<int> ints(0, 1024);

    // Fill the queue
    for (Size i = 0; i < 64; i++)
    {
        q.push(ints.random());
        testAssert(q.count() == (i + 1));
        testAssert(q.size() == 64);
    }

    // Verify that all items exist
    for (Size i = 0; i < 64; i++)
        testAssert(q.contains(ints[i]));

    // Check that other integers do not exist
    testAssert(!q.contains(1025));
    testAssert(!q.contains(-1));

    // Clear the queue
    q.clear();
    testAssert(q.count() == 0);
    testAssert(q.size() == 64);

    // Verify that none of the items exist
    for (Size i = 0; i < 64; i++)
        testAssert(!q.contains(ints[i]));

    // Check that other integers also still do not exist
    testAssert(!q.contains(1025));
    testAssert(!q.contains(-1));

    return OK;
}
