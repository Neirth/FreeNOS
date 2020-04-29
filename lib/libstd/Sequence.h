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

#ifndef __LIBSTD_SEQUENCE_H
#define __LIBSTD_SEQUENCE_H

#include "Container.h"
#include "Comparable.h"
#include "Types.h"
#include "Macros.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Sequences are containers that provide indexed based storage of items.
 */
template <class T> class Sequence : public Container, public Comparable<Sequence<T> >
{
  public:

    /**
     * Adds the given item to the Sequence, if possible.
     *
     * @param item The item to add to the Sequence.
     *
     * @return Position of the item in the Sequence or -1 on failure.
     */
    virtual int insert(const T & item)
    {
        return -1;
    }

    /**
     * Inserts the given item at the given position.
     *
     * If an item exists at the given position, it will be replaced by the given item.
     *
     * @param position The position to insert the item.
     * @param item The item to insert
     *
     * @return bool Whether inserting the item at the given position succeeded.
     */
    virtual bool insert(Size position, const T & item)
    {
        return false;
    }

    /**
     * Fill the Sequence with the given value.
     *
     * @param value New value to fill the Sequence.
     */
    virtual void fill(T value)
    {
        Size s = this->size();

        for (Size i = 0; i < s; i++)
            insert(i, value);
    }

    /**
     * Remove all items with the given value.
     *
     * @param value Value to remove.
     *
     * @return Number of items removed.
     */
    virtual int remove(T value)
    {
        return 0;
    }

    /**
     * Removes the item at the given position.
     *
     * @param position The position of the item to remove.
     *
     * @return bool Whether removing the item succeeded.
     */
    virtual bool removeAt(Size position)
    {
        return false;
    }

    /**
     * Removes all items from the Sequence.
     */
    virtual void clear()
    {
        Size s = this->size();

        for (Size i = 0; i < s; i++)
            removeAt(i);
    }

    /**
     * Returns the item at the given position.
     *
     * @param position The position of the item to get.
     *
     * @return Pointer to the item at the given position or ZERO if no item available.
     */
    virtual const T * get(Size position) const = 0;

    /**
     * Returns a reference to the item at the given position.
     *
     * @param position Valid index inside this array.
     *
     * @return Reference to the item at the given position
     *
     * @note Position must be a valid index.
     */
    virtual const T & at(Size position) const = 0;

    /**
     * Check if the given item is stored in this Sequence.
     */
    virtual bool contains(const T value) const
    {
        Size sz = this->size();

        for (Size i = 0; i < sz; i++)
            if (at(i) == value)
                return true;

        return false;
    }

    /**
     * Compare this Sequence to another Sequence.
     */
    virtual int compareTo(const Sequence<T> &s) const
    {
        Size sz  = this->size();
        Size cnt = this->count();

        // Size must be equal
        if (s.size() != sz)
            return s.size() - sz;

        // Count must be equal
        if (s.count() != cnt)
            return s.count() - cnt;

        // All elements must be equal
        for (Size i = 0; i < cnt; i++)
        {
            if (at(i) != s.at(i))
            {
                return i + 1;
            }
        }
        return 0;
    }

    /**
     * Test if this Sequence is equal to an other Sequence.
     *
     * @param s Sequence instance.
     *
     * @return True if equal, false otherwise.
     */
    virtual bool equals(const Sequence<T> &s) const
    {
        return compareTo(s) == 0;
    }

    /**
     * Returns the item at the given position in the Sequence.
     *
     * @param i The index of the item to return.
     *
     * @return the Item at position i.
     */
    const T & operator [] (int i) const
    {
        return at(i);
    }

    /**
     * Returns the item at the given position in the Sequence.
     *
     * @param i The index of the item to return.
     *
     * @return the Item at position i.
     */
    const T & operator [] (Size i) const
    {
        return at(i);
    }

    /**
     * Returns the item at the given position in the Sequence.
     *
     * @param i The index of the item to return.
     *
     * @return the Item at position i.
     */
    T & operator [] (int i)
    {
        return (T &) at(i);
    }

    /**
     * Returns the item at the given position in the Sequence.
     *
     * @param i The index of the item to return.
     *
     * @return the Item at position i.
     */
    T & operator [] (Size i)
    {
        return (T &) at(i);
    }
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_SEQUENCE_H */
