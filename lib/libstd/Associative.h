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

#ifndef __LIBSTD_ASSOCIATIVE_H
#define __LIBSTD_ASSOCIATIVE_H

#include "Container.h"
#include "Comparable.h"
#include "Types.h"
#include "Macros.h"
#include "List.h"
#include "ListIterator.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Associatives are containers that provide a mapping of keys to values.
 */
template <class K, class V> class Associative : public Container, public Comparable<Associative<K,V> >
{
  public:

    /**
     * Inserts the given item to the Assocation.
     *
     * If an item exists for the given key, its value will be replaced.
     *
     * @param key The key of the item to insert
     * @param item The item to insert
     *
     * @return bool Whether inserting the item at the given position succeeded.
     */
    virtual bool insert(const K & key, const V & item)
    {
        return false;
    }

    /**
     * Append the given item to the Association.
     *
     * @param key The key of the item to insert
     * @param item The item to insert
     *
     * @return True on success and false otherwise
     */
    virtual bool append(const K & key, const V & item)
    {
        return false;
    }

    /**
     * Removes all items associated with the given key.
     *
     * @param key The key to remove items for.
     *
     * @return Number of items removed associated with the key.
     */
    virtual int remove(const K & key)
    {
        return 0;
    }

    /**
     * Removes all items from the Association
     */
    virtual void clear()
    {
        List<K> k = keys();

        for (ListIterator<K> i(k); i.hasNext(); i++)
            remove(i.current());
    }

    /**
     * Retrieve all keys inside the Association.
     *
     * @return A List of keys.
     */
    virtual List<K> keys() const = 0;

    /**
     * Retrieve list of Keys for the given value.
     */
    virtual List<K> keys(const V & value) const = 0;

    /**
     * Check if the given key exists.
     *
     * @return True if exists, false otherwise.
     */
    virtual bool contains(const K & key) const
    {
        return values(key).count() > 0;
    }

    /**
     * Retrieve all values inside the Association.
     *
     * @return A List of values.
     */
    virtual List<V> values() const = 0;

    /**
     * Retrieve values for the given key inside the Association.
     *
     * @return A List of values.
     */
    virtual List<V> values(const K & key) const = 0;

    /**
     * Returns the first value for the given key.
     *
     * @param key Key to find.
     *
     * @return Pointer to the first value for the given key or ZERO if not found.
     */
    virtual const V * get(const K & key) const = 0;

    /**
     * Returns a reference to the first value for the given key.
     *
     * This function assumes the key exists.
     *
     * @param key Key to find.
     *
     * @return Reference to the first value for the key.
     */
    virtual const V & at(const K & key) const = 0;

    /**
     * Return the first value for the given key.
     *
     * If the key is not found, the default value is returned.
     *
     * @param key Key to find value for
     * @param defaultValue Return this default value if not found
     *
     * @return First value for the given key, or the defaultValue.
     */
    virtual const V value(const K & key, const V defaultValue = V()) const = 0;

    /**
     * Compare this instance to another instance
     *
     * @param a Other instance to compare to
     *
     * @return Zero if equal, non-zero otherwise
     */
    virtual int compareTo(const Associative<K,V> &a) const
    {
        Size sz = size();
        Size cnt = count();
        List<V> vals = a.values();

        // Size must be equal
        if (a.size() != sz)
            return a.size() - sz;

        // Count must be equal
        if (a.count() != cnt)
            return a.count() - cnt;

        // All elements must be equal
        for (ListIterator<V> i(values()); i.hasCurrent(); i++)
            if (!vals.contains(i.current()))
                return 1;

        return 0;
    }

    /**
     * Test if an Associative is equal to an other Associative.
     *
     * @param a Associative instance.
     *
     * @return True if equal, false otherwise.
     */
    virtual bool equals(const Associative<K,V> &a) const
    {
        return compareTo(a) == 0;
    }

    /**
     * Returns the first value for the given key.
     *
     * If there are multiple values for the key, the first
     * value is returned.
     *
     * @param key The key of the item to return.
     *
     * @return The first value for the key.
     */
    const V & operator [] (K key) const
    {
        return at(key);
    }
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_ASSOCIATIVE_H */
