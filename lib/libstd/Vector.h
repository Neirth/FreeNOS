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

#ifndef __LIBSTD_VECTOR_H
#define __LIBSTD_VECTOR_H

#include "Assert.h"
#include "Sequence.h"
#include "Types.h"
#include "Macros.h"
#include "MemoryBlock.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/** Default size of an Vector */
#define VECTOR_DEFAULT_SIZE  64

/**
 * Vectors are dynamically resizeable Arrays.
 */
template <class T> class Vector : public Sequence<T>
{
  public:

    /**
     * Constructor.
     *
     * Initializes the Vector with the given Size.
     *
     * @param size The maximum size of the array
     */
    Vector(Size size = VECTOR_DEFAULT_SIZE)
    {
        assert(size > 0);

        m_size  = size;
        m_count = 0;
        m_array = new T[m_size];
    }

    /**
     * Copy constructor.
     *
     * @param a Vector reference to copy from.
     */
    Vector(const Vector<T> & a)
    {
        assert(a.m_size > 0);

        m_size  = a.m_size;
        m_count = a.m_count;
        m_array = new T[m_size];

        for (Size i = 0; i < m_size; i++)
            m_array[i] = a.m_array[i];
    }

    /**
     * Destructor.
     */
    virtual ~Vector()
    {
        delete[] m_array;
    }

    /**
     * Adds the given item to the Vector, if possible.
     *
     * @param item The item to add to the Vector.
     *
     * @return Position of the item in the Vector or -1 on failure.
     */
    virtual int insert(const T & item)
    {
        if (m_count == m_size)
            if (!resize(m_size*2))
                return -1;

        m_array[m_count++] = item;
        return m_count-1;
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
        // Resize the vector if needed
        if (position >= m_size)
        {
            Size increase = position > (m_size * 2) ? position : m_size * 2;

            if (!resize(m_size+increase))
                return false;
        }
        // Update the item count if needed
        if (position >= m_count)
            m_count += (position+1) - m_count;

        // Assign the item
        m_array[position] = item;
        return true;
    }

    /**
     * Returns the item at the given position.
     *
     * @param position The position of the item to get.
     *
     * @return Pointer to the item at the given position.
     */
    virtual const T * get(Size position) const
    {
        if (position >= m_count)
        {
            return ZERO;
        }
        return &m_array[position];
    }

    /**
     * Return item at the given position as a reference.
     *
     * @param position Position of the item to get.
     */
    virtual const T & at(Size position) const
    {
        return m_array[position];
    }

    /**
     * Remove all items from the vector.
     */
    virtual void clear()
    {
        m_count = 0;
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
        if (position >= m_count)
        {
            return false;
        }

        // Move all consequetive items
        for (Size i = position; i < m_count-1; i++)
        {
            m_array[i] = m_array[i+1];
        }
        m_count--;
        return true;
    }

    /**
     * Returns the maximum size of this Vector.
     *
     * @return size The maximum size of this Vector.
     */
    virtual Size size() const
    {
        return m_size;
    }

    /**
     * Returns the number of items inside the Vector.
     *
     * @return Number of items inside the Vector.
     */
    virtual Size count() const
    {
        return m_count;
    }

    /**
     * Get Vector data pointer.
     *
     * @return Pointer to Vector data.
     */
    virtual const T * vector() const
    {
        return m_array;
    }

    /**
     * Resize the Vector.
     *
     * @param increase Size to add to the Vector.
     */
    virtual bool resize(Size size)
    {
        assert(size > 0);

        T *arr = new T[size];
        if (!arr)
            return false;

        // Copy the old array in the new one
        for (Size i = 0; i < m_size; i++)
        {
            arr[i] = m_array[i];
        }
        // Clean up the old array and set the new one
        delete[] m_array;
        m_array = arr;
        m_size  = size;
        return true;
    }

  private:

    /** The actual array where the data is stored. */
    T* m_array;

    /** The maximum size of the array. */
    Size m_size;

    /** Number of used items in the array. */
    Size m_count;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_VECTOR_H */
