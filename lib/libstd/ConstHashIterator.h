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

#ifndef __LIBSTD_CONSTHASHITERATOR_H
#define __LIBSTD_CONSTHASHITERATOR_H

#include "Macros.h"
#include "Types.h"
#include "ConstIterator.h"
#include "ListIterator.h"
#include "HashTable.h"
#include "Assert.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Iterate through a constant (read-only) HashTable.
 */
template <class K, class V> class ConstHashIterator : public ConstIterator<V>
{
  public:

    /**
     * Class constructor.
     *
     * @param hash Reference to the HashTable to iterate.
     */
    ConstHashIterator(const HashTable<K, V> & hash)
        : m_hash(hash), m_keys(hash.keys()), m_iter(m_keys)
    {
        assertRead(hash);
    }

    /**
     * Destructor.
     */
    virtual ~ConstHashIterator()
    {
    }

    /**
     * Reset the iterator.
     */
    virtual void reset()
    {
        m_iter.reset();
    }

    /**
     * Check if there is more to iterate.
     * @return true if more items, false if not.
     */
    virtual bool hasNext() const
    {
        return m_iter.hasNext();
    }

    /**
     * Check if there is a current item.
     *
     * @return True if the iterator has a current item, false otherwise.
     */
    virtual bool hasCurrent() const
    {
        return m_iter.hasCurrent();
    }

    /**
     * Get the current value (read-only).
     *
     * @return Reference to the current read-only value.
     */
    virtual const V & current() const
    {
        return m_hash[m_iter.current()];
    }

    /**
     * Get the current key.
     *
     * @return Reference to the current key.
     */
    virtual const K & key() const
    {
        return m_iter.current();
    }

    /**
     * Fetch the next item.
     *
     * This function first fetches the next item
     * and then updates the current item pointer to that item.
     *
     * @return Reference to the next item.
     */
    virtual const V & next()
    {
        return m_hash[m_iter.next()];
    }

    /**
     * Increment operator.
     *
     * This function first increment the current item
     * and then updates the next item pointer.
     *
     * @param num Ignored
     */
    virtual void operator ++(int num)
    {
        m_iter++;
    }

  private:

    /** Points to the HashTable to iterate. */
    const HashTable<K, V> & m_hash;

    /** List of keys to iterate. */
    List<K> m_keys;

    /** Iterator of keys. */
    ListIterator<K> m_iter;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_HASHITERATOR_H */
