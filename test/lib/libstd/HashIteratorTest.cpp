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
#include <TestChar.h>
#include <TestMain.h>
#include <HashTable.h>
#include <HashIterator.h>
#include <List.h>

TestCase(HashIteratorConstruct)
{
    HashTable<String, int> h;
    HashIterator<String, int> i(h);

    // The iterator should be empty
    testAssert(!i.hasNext());
    testAssert(!i.hasCurrent());
    return OK;
}

TestCase(HashIteratorCurrent)
{
    HashTable<String, int> h;
    TestChar<char *> strings(8, 32);
    TestInt<int> ints(INT_MIN, INT_MAX);
    TestInt<Size> sizes(32, 128);
    Size size = sizes.random();
    List<String> keys;

    // Generate unique keys
    strings.unique(size);

    // Insert random values
    for (Size i = 0; i < size; i++)
    {
        testAssert(!h.contains(strings.get(i)));
        testAssert(h.insert(strings.get(i), ints.random()));
    }

    // Check that all key/values have been iterated
    List<String> hashKeys = h.keys();

    // Iterate the hash via hasCurrent() and current()
    HashIterator<String, int> it(h);
    for (Size i = 0; i < size; i++, it++)
    {
        // Key must match value in the HashTable
        testAssert(it.hasCurrent());
        testAssert(h[it.key()] == it.current());
        keys << it.key();
    }
    // Iteration should be stopped now
    testAssert(!it.hasCurrent());
    testAssert(!it.hasNext());

    // Check that all key/values have been iterated
    List<String> hashKeys2 = h.keys();
    testAssert(hashKeys2.count() == size);
    testAssert(hashKeys == hashKeys2);
    testAssert(hashKeys == keys);
    testAssert(keys.count() == hashKeys.count());

    for (ListIterator<String> i(hashKeys); i.hasCurrent(); i++)
        testAssert(keys.contains(i.current()));

    return OK;
}

TestCase(HashIteratorNext)
{
    HashTable<String, int> h;
    TestChar<char *> strings(8, 32);
    TestInt<int> ints(INT_MIN, INT_MAX);
    TestInt<Size> sizes(32, 128);
    Size size = sizes.random();
    List<String> keys;
    
    // Generate unique keys
    strings.unique(size);

    // Insert random values
    for (Size i = 0; i < size; i++)
        testAssert(h.insert(strings.get(i), ints.random()));

    // Iterate the hash via hasNext() and next()
    HashIterator<String, int> it(h);

    for (Size i = 0; i < size; i++)
    {
        testAssert(it.hasNext());

        int val = it.next();

        testAssert(h[it.key()] == val);
        testAssert(it.current() == val);
        testAssert(h.keys().contains(it.key()));
        keys << it.key();
    }
    // Iteration should be stopped now
    testAssert(!it.hasNext());

    // Check that all key/values have been iterated
    List<String> hashKeys = h.keys();
    for (ListIterator<String> i(hashKeys); i.hasCurrent(); i++)
        testAssert(keys.contains(i.current()));

    return OK;
}

TestCase(HashIteratorRemoveFirst)
{
    HashTable<String, int> h;
    TestChar<char *> strings(8, 32);
    TestInt<int> ints(INT_MIN, INT_MAX);
    TestInt<Size> sizes(32, 128);
    Size size = sizes.random();
    List<String> keys;
    
    // Generate unique keys
    strings.unique(size);

    // Insert random values
    for (Size i = 0; i < size; i++)
        testAssert(h.insert(strings.get(i), ints.random()));

    // Start iterating on the hash.
    HashIterator<String, int> it(h);

    // Remove the first item.
    String s = it.key();

    testAssert(it.remove());
    testAssert(!h.contains(s));
    testAssert(h.count() == size-1);

    // Check that the iterator can iterate the rest of the hash.
    for (Size i = 1; i < size; i++)
    {
        testAssert(it.hasNext());

        int val = it.next();

        testAssert(h[it.key()] == val);
        testAssert(it.current() == val);
        testAssert(h.keys().contains(it.key()));
        keys << it.key();
    }
    // Iteration should be stopped now
    testAssert(!it.hasNext());

    // Check that all key/values have been iterated
    List<String> hashKeys = h.keys();
    for (ListIterator<String> i(hashKeys); i.hasCurrent(); i++)
        testAssert(keys.contains(i.current()));

    return OK;
}

TestCase(HashIteratorRemoveLast)
{
    HashTable<String, int> h;
    TestChar<char *> strings(8, 32);
    TestInt<int> ints(INT_MIN, INT_MAX);
    TestInt<Size> sizes(32, 128);
    Size size = sizes.random();
    List<String> keys;
    
    // Generate unique keys
    strings.unique(size);

    // Insert random values
    for (Size i = 0; i < size; i++)
        testAssert(h.insert(strings.get(i), ints.random()));

    // Move iterator to the last item.
    HashIterator<String, int> it(h);
    for (Size i = 0; i < size - 1; i++)
        it++;

    // Remove the last item.
    String s = it.key();
    testAssert(it.remove());
    testAssert(it.m_iter.m_next == ZERO);
    testAssert(it.m_iter.m_current == ZERO);
    testAssert(!h.contains(s));
    testAssert(h.count() == size-1);

    // The iterator should stop now.
    testAssert(!it.hasNext());
    testAssert(!it.hasCurrent());

    // Check that the iterator can iterate again.
    it.reset();
    for (Size i = 0; i < size-1; i++)
    {
        testAssert(it.hasNext());

        int val = it.next();

        testAssert(h[it.key()] == val);
        testAssert(it.current() == val);
        testAssert(h.keys().contains(it.key()));
        keys << it.key();
    }
    // Iteration should be stopped now
    testAssert(!it.hasNext());

    // Check that all key/values have been iterated
    List<String> hashKeys = h.keys();
    for (ListIterator<String> i(hashKeys); i.hasCurrent(); i++)
        testAssert(keys.contains(i.current()));

    return OK;
}

TestCase(HashIteratorRemoveSingle)
{
    HashTable<String, int> h;
    TestChar<char *> strings(8, 32);
    TestInt<int> ints(INT_MIN, INT_MAX);
    TestInt<Size> sizes(32, 128);
    Size size = sizes.random();
    Size position = size/2;
    
    // Generate unique keys
    strings.unique(size);

    // Insert random values
    for (Size i = 0; i < size; i++)
        testAssert(h.insert(strings.get(i), ints.random()));

    // Start iterating.
    HashIterator<String, int> it(h);
    for (Size i = 0; i < size - 1; i++, it++)
    {
        if (i == position)
        {
            // Remove one item.
            String s = it.key();
            testAssert(it.remove());
            testAssert(!h.contains(s));
            testAssert(h.count() == size-1);
        }
        else
        {
            testAssert(h[it.key()] == it.current());
            testAssert(h.values().contains(it.current()));
        }
    }
    // The iterator should stop now.
    testAssert(!it.hasNext());
    testAssert(!it.hasCurrent());

    // Check that the iterator can iterate again.
    it.reset();
    for (Size i = 0; i < size-1; i++)
    {
        // Skip the removed item.
        if (i != position)
        {
            testAssert(it.hasCurrent());
            testAssert(h[it.key()] == it.current());
            testAssert(h.values().contains(it.current()));
            it++;
        }
    }
    return OK;
}

TestCase(HashIteratorRemoveAll)
{
    HashTable<String, int> h;
    TestChar<char *> strings(8, 32);
    TestInt<int> ints(INT_MIN, INT_MAX);
    TestInt<Size> sizes(32, 128);
    Size size = sizes.random();
    
    // Generate unique keys
    strings.unique(size);

    // Insert random values
    for (Size i = 0; i < size; i++)
        testAssert(h.insert(strings.get(i), ints.random()));

    // Remove all items by iteration.
    HashIterator<String, int> it(h);
    for (; it.hasCurrent();)
        it.remove();

    // The iterator should be empty.
    testAssert(!it.hasNext());
    testAssert(!it.hasCurrent());

    // The list should be empty.
    testAssert(h.isEmpty());
    testAssert(h.count() == 0);
    testAssert(h.size() == HASHTABLE_DEFAULT_SIZE);
    testAssert(h.keys().count() == 0);
    testAssert(h.values().count() == 0);
    return OK;
}

TestCase(HashIteratorReset)
{
    HashTable<String, int> h;
    TestChar<char *> strings(8, 32);
    TestInt<int> ints(INT_MIN, INT_MAX);
    TestInt<Size> sizes(32, 128);
    Size size = sizes.random();
    Size rounds = sizes.random();

    // Generate unique keys
    strings.unique(size);

    // Insert random values
    for (Size i = 0; i < size; i++)
        testAssert(h.insert(strings.get(i), ints.random()));

    // Perform a number of iteration rounds
    for (Size i = 0; i < rounds; i++)
    {
        // Iterate the list via hasCurrent() and current()
        HashIterator<String, int> it(h);

        // At a random position while iterating, perform a reset.
        Size position = sizes.random();

        for (Size j = 0; j < size; j++, it++)
        {
            testAssert(it.hasCurrent());
            testAssert(h[it.key()] == it.current());
            testAssert(h.values().contains(it.current()));

            // Perform a reset?
            if (j == position)
                break;
        }
        // Reset the iterator.
        it.reset();

        // Now iterate again from the start to the end.
        for (Size j = 0; j < size; j++, it++)
        {
            testAssert(it.hasCurrent());
            testAssert(h[it.key()] == it.current());
            testAssert(h.values().contains(it.current()));
        }
    }
    return OK;
}
