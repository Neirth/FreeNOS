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
#include <String.h>
#include <HashTable.h>

TestCase(HashTableConstruct)
{
    HashTable<String, int> h;

    // The table should be empty
    testAssert(h.count() == 0);
    testAssert(h.size() == HASHTABLE_DEFAULT_SIZE);
    testAssert(h.keys().count() == 0);
    testAssert(h.values().count() == 0);
    return OK;
}

TestCase(HashTableConstructInts)
{
    HashTable<int, int> h;

    // The table should be empty
    testAssert(h.count() == 0);
    testAssert(h.size() == HASHTABLE_DEFAULT_SIZE);
    testAssert(h.keys().count() == 0);
    testAssert(h.values().count() == 0);
    return OK;
}

TestCase(HashTableInsert)
{
    HashTable<String, int> h;

    // Insert one value
    testAssert(h.insert("key1", 1234));

    // Check the value inserted correctly.
    testAssert(h.count() == 1);
    testAssert(h.size() == HASHTABLE_DEFAULT_SIZE);
    testAssert(h.keys().contains("key1"));
    testAssert(h.values().contains(1234));
    testAssert(h["key1"] == 1234);
    return OK;
}

TestCase(HashTableInsertOverwrite)
{
    HashTable<String, int> h;

    // Insert and overwrite one value.
    testAssert(h.insert("key1", 1234));
    testAssert(h.insert("key1", 4567));

    // Check the value inserted correctly.
    testAssert(h.count() == 1);
    testAssert(h.size() == HASHTABLE_DEFAULT_SIZE);
    testAssert(h.keys().contains("key1"));
    testAssert(h.values().contains(4567));
    testAssert(h["key1"] == 4567);
    return OK;
}

TestCase(HashTableInsertRandom)
{
    HashTable<String, int> h;
    TestChar<char *> strings(8, 32);    
    TestInt<int> ints(INT_MIN, INT_MAX);
    TestInt<Size> sizes(32, 128);
    Size size = 20U;

    // Generate unique keys
    strings.unique(size);

    // Insert random values
    for (Size i = 0; i < size; i++)
        testAssert(h.insert(strings.get(i), ints.random()));

    // Check all key/value pairs are correctly inserted
    for (Size i = 0; i < size; i++)
    {
        // Check key lookup
        testAssert(h[strings.get(i)] == ints.get(i));
        testAssert(h.get(strings.get(i)) != ZERO);
        testAssert(*h.get(strings.get(i)) == ints.get(i));
        testAssert(h.at(strings.get(i)) == ints.get(i));
        testAssert(h.value(strings.get(i)) == ints.get(i));

        // Check key/value lists
        testAssert(h.keys().contains(strings.get(i)));
        testAssert(h.values().contains(ints.get(i)));
        testAssert(h.keys(ints.get(i)).contains(strings.get(i)));
        testAssert(h.values(strings.get(i)).contains(ints.get(i)));
    }
    // Check administration
    testAssert(h.count() == size);
    testAssert(h.size() == HASHTABLE_DEFAULT_SIZE);
    testAssert(h.keys().count() == size);
    testAssert(h.values().count() == size);
    return OK;
}

TestCase(HashTableAppend)
{
    HashTable<String, int> h;
    TestChar<char *> strings(8, 32);    
    TestInt<int> ints(INT_MIN, INT_MAX);
    TestInt<Size> sizes(32, 128);
    Size size = sizes.random();

    // Generate one key.
    strings.random();

    // Append random values.
    for (Size i = 0; i < size; i++)
        testAssert(h.append(strings.get(0), ints.random()));

    // Check all values are appended to the same key.
    for (Size i = 0; i < size; i++)
    {
        // Check direct key lookup matches first value
        testAssert(h[strings.get(0)] == ints.get(0));
        testAssert(h.get(strings.get(0)) != ZERO);
        testAssert(*h.get(strings.get(0)) == ints.get(0));
        testAssert(h.at(strings.get(0)) == ints.get(0));
        testAssert(h.value(strings.get(0)) == ints.get(0));

        // Check key/value lists. It should contain all the values.
        testAssert(h.keys().contains(strings.get(0)));
        testAssert(h.values().contains(ints.get(i)));
        testAssert(h.keys(ints.get(i)).contains(strings.get(0)));
        testAssert(h.values(strings.get(0)).contains(ints.get(i)));
    }
    // Check administration
    testAssert(h.count() == size);
    testAssert(h.size() == HASHTABLE_DEFAULT_SIZE);
    return OK;
}

TestCase(HashTableRemove)
{
    HashTable<String, int> h;
    TestChar<char *> strings(8, 32);    
    TestInt<int> ints(INT_MIN, INT_MAX);
    TestInt<Size> sizes(32, 64);
    Size size = sizes.random();

    // Generate unique keys
    strings.unique(size);

    // Insert random values
    for (Size i = 0; i < size; i++)
        testAssert(h.insert(strings.get(i), ints.random()));

    // Remove one key/value.
    testAssert(h.remove(strings.get(0)) == 1);

    // Check all other key/value pairs still exist
    for (Size i = 1; i < size; i++)
    {
        // Check key lookup
        testAssert(h[strings.get(i)] == ints.get(i));
        testAssert(h.get(strings.get(i)) != ZERO);
        testAssert(*h.get(strings.get(i)) == ints.get(i));
        testAssert(h.at(strings.get(i)) == ints.get(i));
        testAssert(h.value(strings.get(i)) == ints.get(i));

        // Check key/value lists
        testAssert(h.keys().contains(strings.get(i)));
        testAssert(h.values().contains(ints.get(i)));
        testAssert(h.keys(ints.get(i)).contains(strings.get(i)));
        testAssert(h.values(strings.get(i)).contains(ints.get(i)));
    }

    // Check administration
    testAssert(h.count() == size - 1);
    testAssert(h.size() == HASHTABLE_DEFAULT_SIZE);
    testAssert(!h.keys().contains(strings.get(0)));
    testAssert(h.get(strings.get(0)) == ZERO);
    return OK;
}

TestCase(HashTableRemoveMultiple)
{
    HashTable<String, int> h;
    TestChar<char *> strings(8, 32);    
    TestInt<int> ints(INT_MIN, INT_MAX);
    TestInt<Size> sizes(32, 64);
    Size size = sizes.random();

    // Generate unique keys
    strings.unique(size);

    // Insert random values
    for (Size i = 0; i < size; i++)
        testAssert(h.insert(strings.get(i), ints.random()));

    // Append first value multiple times
    for (Size i = 0; i < 10; i++)
        testAssert(h.append(strings.get(0), ints.get(0)));

    // Remove multiple values.
    testAssert(h.remove(strings.get(0)) == 11);

    // Check all other key/value pairs still exist
    for (Size i = 1; i < size; i++)
    {
        // Check key lookup
        testAssert(h[strings.get(i)] == ints.get(i));
        testAssert(h.get(strings.get(i)) != ZERO);
        testAssert(*h.get(strings.get(i)) == ints.get(i));
        testAssert(h.at(strings.get(i)) == ints.get(i));
        testAssert(h.value(strings.get(i)) == ints.get(i));

        // Check key/value lists
        testAssert(h.keys().contains(strings.get(i)));
        testAssert(h.values().contains(ints.get(i)));
        testAssert(h.keys(ints.get(i)).contains(strings.get(i)));
        testAssert(h.values(strings.get(i)).contains(ints.get(i)));
    }

    // Check administration
    testAssert(h.count() == size - 1);
    testAssert(h.size() == HASHTABLE_DEFAULT_SIZE);
    testAssert(!h.keys().contains(strings.get(0)));
    testAssert(h.get(strings.get(0)) == ZERO);
    return OK;
}

TestCase(HashTableGet)
{
    HashTable<String, int> h;
    TestChar<char *> strings(8, 32);    
    TestInt<int> ints(INT_MIN, INT_MAX);
    TestInt<Size> sizes(32, 64);
    Size size = sizes.random();

    // Generate unique keys
    strings.unique(size);

    // Insert random values
    for (Size i = 0; i < size; i++)
        testAssert(h.insert(strings.get(i), ints.random()));

    // Get a non-existing key
    testAssert(h.get("test") == ZERO);

    // Get a non-existing key with a default
    testAssert(h.value("test", 123456) == 123456);
    return OK;
}

TestCase(HashTableIterate)
{
    HashTable<String, int> h;
    TestChar<char *> strings(8, 32);    
    TestInt<int> ints(INT_MIN, INT_MAX);
    TestInt<Size> sizes(32, 64);
    Size size = sizes.random();

    // Generate unique keys
    strings.unique(size);

    // Insert random values
    for (Size i = 0; i < size; i++)
        testAssert(h.insert(strings.get(i), ints.random()));

    // Iterate over all keys
    List<String> keys = h.keys();

    for (ListIterator<String> i(keys); i.hasCurrent(); i++)
    {
        testAssert(h.contains(i.current()));
    }
    return OK;
}
