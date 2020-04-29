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

#ifndef __LIBSTD_HASHTABLE_H
#define __LIBSTD_HASHTABLE_H

#include "Types.h"
#include "Macros.h"
#include "Vector.h"
#include "List.h"
#include "ListIterator.h"
#include "HashFunction.h"
#include "Associative.h"
#include "Assert.h"

/** Default size of the HashTable internal table. */
#define HASHTABLE_DEFAULT_SIZE    64

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Efficient key -> value lookups.
 */
template <class K, class V> class HashTable : public Associative<K,V>
{
  public:

    /**
     * Describes a bucket in the HashTable, for collision avoidance.
     */
    class Bucket
    {
      public:

        /**
         * Default constructor.
         */
        Bucket()
        {
        }

        /**
         * Constructor.
         *
         * @param k K to use.
         * @param v V of the bucket.
         */
        Bucket(K k, V v)
            : key(k), value(v)
        {
        }

        /**
         * Copy constructor.
         */
        Bucket(const Bucket & b)
            : key(b.key), value(b.value)
        {
        }

        /**
         * Comparision operator.
         *
         * @param b HashBucket instance to compare us with.
         */
        bool operator == (const Bucket & b) const
        {
            return key == b.key && value == b.value;
        }

        /**
         * Inequality operator.
         */
        bool operator != (const Bucket & b) const
        {
            return !(key == b.key && value == b.value);
        }

        /** Key for this item. */
        K key;

        /** Value of the item. */
        V value;
    };

    /**
     * Class constructor.
     *
     * @param size Initial size of the internal table.
     */
    HashTable(Size size = HASHTABLE_DEFAULT_SIZE)
        : m_table(size)
    {
        assert(size > 0);

        m_count = ZERO;

        // Fill the Vector with empty Bucket Lists.
        for (Size i = 0; i < m_table.size(); i++)
            m_table.insert(List<Bucket>());
    }

    /**
     * Inserts the given item to the Assocation.
     *
     * If an item exists for the given key, its value will be replaced.
     *
     * @param position The position to insert the item.
     * @param item The item to insert
     *
     * @return bool Whether inserting the item at the given position succeeded.
     */
    virtual bool insert(const K & key, const V & value)
    {
        assertRead(key);
        assertRead(value);

        Size idx = hash(key, m_table.size());

        // See if the given key exists. Overwrite if so.
        for (ListIterator<Bucket> i(m_table[idx]); i.hasCurrent(); i++)
        {
            if (i.current().key == key)
            {
                i.current().value = value;
                return true;
            }
        }

        // Key does not exist. Append it.
        m_table[idx].append(Bucket(key, value));
        m_count++;
        return true;
    }

    /**
     * Append a new item.
     *
     * @param k Associated key.
     * @param v New item to append.
     *
     * @return True if append successfull, false otherwise.
     */
    virtual bool append(const K & key, const V & value)
    {
        assertRead(key);
        assertRead(value);

        // Always append
        m_table[hash(key, m_table.size())].append(Bucket(key, value));
        m_count++;
        return true;
    }

    /**
     * Remove value(s) for the given key.
     *
     * @param k Associated key.
     *
     * @return Number of values removed.
     */
    virtual int remove(const K & key)
    {
        int removed = 0;

        for (ListIterator<Bucket> i(m_table[hash(key, m_table.size())]); i.hasCurrent(); )
        {
            if (i.current().key == key)
            {
                i.remove();
                m_count--;
                removed++;
            }
            else
                i++;
        }
        return removed;
    }

    /**
     * Get the size of the HashTable.
     *
     * @return Size of the internal array.
     */
    virtual Size size() const
    {
        return m_table.size();
    }

    /**
     * Get the number of values stored in the HashTable.
     *
     * @return Number of items in the HashTable.
     */
    virtual Size count() const
    {
        return m_count;
    }

    /**
     * Retrieve all keys inside the Association.
     *
     * @return A List of keys.
     */
    virtual List<K> keys() const
    {
        List<K> lst;

        for (Size i = 0; i < m_table.count(); i++)
            for (ListIterator<Bucket> j(m_table[i]); j.hasCurrent(); j++)
                if (!lst.contains(j.current().key))
                    lst << j.current().key;

        return lst;
    }

    /**
     * Retrieve list of Keys for the given value.
     */
    virtual List<K> keys(const V & value) const
    {
        List<K> lst;

        for (Size i = 0; i < m_table.count(); i++)
            for (ListIterator<Bucket> j(m_table[i]); j.hasCurrent(); j++)
                if (j.current().value == value && !lst.contains(j.current().key))
                    lst << j.current().key;

        return lst;
    }

    /**
     * Retrieve all values inside the Association.
     *
     * @return A List of values.
     */
    virtual List<V> values() const
    {
        List<V> lst;

        for (Size i = 0; i < m_table.count(); i++)
            for (ListIterator<Bucket> j(m_table[i]); j.hasCurrent(); j++)
                lst << j.current().value;

        return lst;
    }

    /**
     * Retrieve values for the given key inside the Association.
     *
     * @return A List of values.
     */
    virtual List<V> values(const K & key) const
    {
        List<V> lst;

        for (ListIterator<Bucket> i(m_table[hash(key, m_table.size())]); i.hasCurrent(); i++)
            if (i.current().key == key)
                lst << i.current().value;

        return lst;
    }

    /**
     * Returns the first value for the given key.
     *
     * @param key Key to find.
     *
     * @return Pointer to the first value for the given key or ZERO if not found.
     */
    virtual const V * get(const K & key) const
    {
        const List<Bucket> & lst = m_table[hash(key, m_table.size())];

        for (ListIterator<Bucket> i(lst); i.hasCurrent(); i++)
            if (i.current().key == key)
                return &i.current().value;

        return ZERO;
    }

    /**
     * Returns a reference to the first value for the given key.
     *
     * @param key Key to find.
     *
     * @return Reference to the first value for the key.
     *
     * @note This function assumes the key exists.
     */
    virtual const V & at(const K & key) const
    {
        const List<Bucket> & lst = m_table[hash(key, m_table.size())];

        for (ListIterator<Bucket> i(lst); i.hasCurrent(); i++)
            if (i.current().key == key)
                return i.current().value;

        return m_table[0].head()->data.value;
    }

    /**
     * Return the first value for the given key.
     *
     * If the key is not found, the default value is returned.
     *
     * @return First value for the given key, or the defaultValue.
     */
    virtual const V value(const K & key, const V defaultValue = V()) const
    {
        const List<Bucket> & lst = m_table[hash(key, m_table.size())];

        for (ListIterator<Bucket> i(lst); i.hasCurrent(); i++)
            if (i.current().key == key)
                return i.current().value;

        return defaultValue;
    }

    /**
     * Get the internal Vector with Buckets.
     *
     * @return Reference to the Vector with Buckets.
     */
    Vector<List<Bucket> > & table()
    {
        return m_table;
    }

    /**
     * Modifiable index operator.
     */
    V & operator[](const K & key)
    {
        return (V &) at(key);
    }

    /**
     * Constant index operator.
     */
    const V & operator[](const K & key) const
    {
        return (const V &) at(key);
    }

  private:

    /** Internal table. */
    Vector<List<Bucket> > m_table;

    /** Number of values in the buckets. */
    Size m_count;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_HASHTABLE_H */
