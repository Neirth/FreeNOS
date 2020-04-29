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

#ifndef __LIBSTD_INDEX_H
#define __LIBSTD_INDEX_H

#include "Assert.h"
#include "Types.h"
#include "Macros.h"
#include "Sequence.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/** Default size of an Index */
#define INDEX_DEFAULT_SIZE  64

/**
 * Index is a resizable array of pointers to items.
 */
template <class T> class Index : public Sequence<T>
{
  using Sequence<T>::remove;
  using Sequence<T>::contains;
  using Sequence<T>::compareTo;

  public:

    /**
     * Constructor.
     */
    Index(Size size = INDEX_DEFAULT_SIZE)
    {
        m_size  = size;
        m_count = 0;
        m_array = new T*[size];

        for (Size i = 0; i < size; i++)
            m_array[i] = ZERO;
    }

    /**
     * Destructor.
     */
    virtual ~Index()
    {
        delete[] m_array;
    }

    /**
     * Adds the given item to the Sequence, if possible.
     *
     * @param item The item to add to the Sequence.
     *
     * @return Position of the item in the Sequence or -1 on failure.
     */
    virtual int insert(const T & item)
    {
        if (m_count == m_size)
            this->resize(m_size * 2);

        for (Size i = 0; i < m_size; i++)
        {
            if (!m_array[i])
            {
                m_array[i] = (T *) &item;
                m_count++;
                return (int) i;
            }
        }
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
        if (position <= m_size)
            this->resize(position * 2);

        if (!m_array[position])
            m_count++;

        m_array[position] = (T *) &item;
        return true;
    }

    /**
     * Fill the Sequence with the given value.
     *
     * @param value New value to fill the Sequence.
     */
    virtual void fill(T value)
    {
        return;
    }

    /**
     * Removes the item at the given position.
     *
     * @param position The position of the item to remove.
     *
     * @return bool Whether removing the item succeeded.
     */
    virtual bool remove(Size position)
    {
        if (position >= m_size)
            return false;

        if (!m_array[position])
            return false;

        m_array[position] = ZERO;
        m_count--;
        return true;
    }

    /**
     * Returns the item at the given position.
     *
     * @param position The position of the item to get.
     *
     * @return Pointer to the item at the given position or ZERO if no item available.
     */
    virtual const T * get(Size position) const
    {
        if (position >= m_size)
            return ZERO;

        return m_array[position];
    }

    /**
     * Returns a reference to the item at the given position.
     *
     * @param position Valid index inside this array.
     *
     * @return Reference to the item at the given position
     *
     * @note This function does not perform bounds checking. Position must be a valid index.
     */
    virtual const T & at(Size position) const
    {
        return (*m_array[position]);
    }

    /**
     * Check if the given item is stored in this Sequence.
     */
    virtual bool contains(T value)
    {
        for (Size i = 0; i < m_size; i++)
            if (m_array[i] && (*m_array[i]) == value)
                return true;

        return false;
    }

    /**
     * Compare this Sequence to another Sequence.
     */
    virtual int compareTo(const Index<T> &idx) const
    {
        Size sz  = this->size();
        Size cnt = this->count();

        // Size must be equal
        if (idx.size() != sz)
            return idx.size() - sz;

        // Count must be equal
        if (idx.count() != cnt)
            return idx.count() - cnt;

        // All elements must be equal
        for (Size i = 0; i < cnt; i++)
        {
            if (!(idx.m_array[i] && m_array[i] && (*idx.m_array[i]) == (*m_array[i])))
            {
                return i + 1;
            }
        }
        return 0;
    }

    /**
     * Size of the index.
     */
    virtual Size size() const
    {
        return m_size;
    }

    /**
     * Item count of the index.
     */
    virtual Size count() const
    {
        return m_count;
    }

  private:

    /** Array of pointers to items. */
    T** m_array;

    /** Size of the pointer array. */
    Size m_size;

    /** Amount of valid pointers in the array. */
    Size m_count;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_INDEX_H */
