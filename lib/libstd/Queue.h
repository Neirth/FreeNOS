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

#ifndef __LIBSTD_QUEUE_H
#define __LIBSTD_QUEUE_H

#include "Types.h"
#include "Macros.h"
#include "Container.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Array of items as a First-In-First-Out (FIFO) datastructure.
 */
template <class T, Size N> class Queue : public Container
{
  public:

    /**
     * Default constructor
     */
    Queue()
    {
        clear();
    }

    /**
     * Add item to the head of the Queue.
     *
     * @param item The item to add
     *
     * @return True if successful, false otherwise
     */
    bool push(const T & item)
    {
        if (m_count >= N)
        {
            return false;
        }

        m_array[m_head] = item;
        m_head = (m_head + 1) % N;
        m_count++;

        return true;
    }

    /**
     * Remove item from the tail of the Queue.
     *
     * @return Item T
     *
     * @note Do not call this function if the Queue is empty
     */
    T & pop()
    {
        uint idx = m_tail;
        m_tail = (m_tail + 1) % N;
        m_count--;

        return m_array[idx];
    }

    /**
     * Look if an item exists on the Queue
     *
     * @param item Item reference
     *
     * @return True if the item exists, false otherwise
     */
    bool contains(const T & item) const
    {
        for (Size i = 0; i < m_count; i++)
        {
            if (m_array[(m_tail + i) % N] == item)
                return true;
        }

        return false;
    }

    /**
     * Removes all items from the Queue.
     */
    virtual void clear()
    {
        m_head = 0;
        m_tail = 0;
        m_count = 0;
    }

    /**
     * Returns the maximum size of this Queue.
     *
     * @return size The maximum size of the Queue.
     */
    virtual Size size() const
    {
        return N;
    }

    /**
     * Returns the number of items in the Queue.
     *
     * @return Number of items in the Queue.
     */
    virtual Size count() const
    {
        return m_count;
    }

  private:

    /** The actual array where the data is stored. */
    T m_array[N];

    /** Head of the queue */
    uint m_head;

    /** Tail of the queue */
    uint m_tail;

    /** Number of items in the queue */
    uint m_count;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_QUEUE_H */
