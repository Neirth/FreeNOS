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

#ifndef __LIBSTD_LISTITERATOR_H
#define __LIBSTD_LISTITERATOR_H

#include "Macros.h"
#include "Iterator.h"
#include "List.h"
#include "Assert.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Iterate through a List.
 */
template <class T> class ListIterator : public Iterator<T>
{
  public:

    /**
     * Class constructor.
     *
     * @param list Reference to the List to iterate.
     */
    ListIterator(List<T> *list)
        : m_list(*list)
    {
        assertRead(list);

        m_current = ZERO;
        reset();
    }

    /**
     * Class constructor.
     *
     * @param list Reference to the List to iterate.
     */
    ListIterator(List<T> & list)
        : m_list(list)
    {
        assertRead(list);

        m_current = ZERO;
        reset();
    }

    /**
     * Constant class constructor.
     *
     * @param list Reference to the List to iterate.
     */
    ListIterator(const List<T> & list)
        : m_list((List<T> &) list)
    {
        assertRead(list);

        m_current = ZERO;
        reset();
    }

    /**
     * Reset the iterator.
     */
    virtual void reset()
    {
        assertRead(m_list);
        m_current = m_list.head();
        m_next = m_current;
    }

    /**
     * Check if there is more on the List to iterate.
     *
     * @return true if more items, false if not.
     */
    virtual bool hasNext() const
    {
        return m_next != ZERO;
    }

    /**
     * Check if there is a current item on the List.
     *
     * @return True if current item available, false otherwise.
     */
    virtual bool hasCurrent() const
    {
        return m_current != ZERO;
    }

    /**
     * Get current item in the List.
     *
     * @return Current item.
     */
    virtual const T & current() const
    {
        return m_current->data;
    }

    /**
     * Get current item in the List.
     *
     * @return Current item.
     */
    virtual T & current()
    {
        return m_current->data;
    }

    /**
     * Fetch the next item.
     8
     * This function first fetches the next item
     * and then updates the current item pointer to that item.
     * It assumes the iterator has a next item.
     *
     * @return The next item.
     *
     * @see hasNext
     */
    virtual T & next()
    {
        assertRead(m_current);
        assertRead(m_current->next);

        m_current = m_next;
        m_next = m_current->next;
        return m_current->data;
    }

    /**
     * Remove the current item from the List.
     *
     * @return True if successfull, false otherwise.
     */
    virtual bool remove()
    {
        // Do we have a current item?
        if (!m_current)
            return false;

        // Update iterator administration
        class List<T>::Node *node = m_current;
        m_current = m_current->next;
        m_next = m_current;

        // Delete the node on the List
        m_list.remove(node);
        return true;
    }

    /**
     * Increment operator.
     *
     * This function first increment the current item
     * and then updates the next item pointer.
     */
    virtual void operator++(int num)
    {
        if (m_current)
        {
            m_current = m_current->next;

            if (m_current)
                m_next = m_current->next;
            else
                m_next = ZERO;
        }
    }

  private:

    /** Points to the List to iterate. */
    List<T> & m_list;

    /** Current node */
    class List<T>::Node *m_current;

    /** Next node */
    class List<T>::Node *m_next;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_LISTITERATOR_H */
