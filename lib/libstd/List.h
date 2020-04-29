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

#ifndef __LIBSTD_LIST_H
#define __LIBSTD_LIST_H

#include "Macros.h"
#include "Assert.h"
#include "Sequence.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Simple linked list template class.
 */
template <class T> class List : public Sequence<T>
{
  public:

    /**
     * Represents an item on the List.
     */
    class Node
    {
      public:

        /**
         * Constructor.
         */
        Node(T t) : data(t)
        {
            prev = ZERO;
            next = ZERO;
        }

        /** Item of this node */
        T data;

        /** Previous node */
        Node *prev;

        /** Next node */
        Node *next;
    };

    /**
     * Class constructor.
     */
    List()
    {
        m_head  = ZERO;
        m_tail  = ZERO;
        m_count = 0;
    }

    /**
     * Copy constructor.
     *
     * @param lst List instance to copy from
     */
    List(const List<T> & lst)
    {
        m_head  = ZERO;
        m_tail  = ZERO;
        m_count = 0;

        for (Node *node = lst.m_head; node; node = node->next)
            append(node->data);
    }

    /**
     * Class destructor.
     */
    virtual ~List()
    {
        Node *node = m_head;

        while (node)
        {
            Node *tmp = node;
            node = node->next;
            delete tmp;
        }
    }

    /**
     * Insert an item at the start of the list.
     *
     * @param t Data item to te inserted.
     */
    void prepend(T t)
    {
        assertRead(t);

        // Create a new node with the item.
        Node *node = new Node(t);

        // Connect the item to the list head, if set
        if (m_head)
        {
            m_head->prev = node;
            node->next = m_head;
        }
        // Make the new node head of the list.
        m_head = node;

        // Also make it the tail, if not yet set
        if (!m_tail)
            m_tail = node;

        // Update node count
        m_count++;
    }

    /**
     * Insert an item at the end of the list.
     *
     * @param t Item to insert.
     */
    void append(T t)
    {
        assertRead(t);

        Node *node = new Node(t);
        node->prev = m_tail;

        // Connect the item with the tail, if any.
        if (m_tail)
            m_tail->next = node;

        // Make the new Node the tail of the list.
        m_tail = node;

        // Also make the item the head, if none.
        if (!m_head)
            m_head = node;

        // Update node count.
        m_count++;
    }

    /**
     * Remove all items which are equal to the given item.
     *
     * @param t Item to remove.
     * @param single True to remove only one item, false to remove all matching items.
     *
     * @return Number of items removed from the list.
     */
    virtual int remove(T t)
    {
        assertRead(t);

        Node *node = m_head;
        Node *next;
        int num = 0;

        while (node)
        {
            next = node->next;

            if (node->data == t)
            {
                remove(node);
                num++;
            }
            node = next;
        }
        return num;
    }

    /**
     * Removes the given node from the list.
     *
     * @param node The Node to remove.
     *
     * @return True on success, false otherwise
     */
    virtual int remove(Node *node)
    {
        if (node->prev)
            node->prev->next = node->next;

        if (node == m_head)
            m_head = node->next;

        if (node->next)
            node->next->prev = node->prev;

        if (node == m_tail)
            m_tail = node->prev;

        m_count--;
        delete node;
        return true;
    }

    /**
     * Check whether an element is on the List.
     *
     * @param t The element to find.
     *
     * @return true if the element is on the List, false otherwise.
     */
    virtual bool contains(const T t) const
    {
        assertRead(t);

        for (Node *i = m_head; i; i = i->next)
            if (i->data == t)
                return true;

        return false;
    }

    /**
     * Clears the entire List.
     */
    virtual void clear()
    {
        Node *node = m_head, *next = ZERO;

        // Delete all Node and optionally items too.
        while (node)
        {
            next = node->next;
            delete node;
            node = next;
        }
        // Clear administration
        m_head  = ZERO;
        m_tail  = ZERO;
        m_count = 0;
    }

    /**
     * Get the first Node on the list.
     *
     * @return First Node on the List.
     */
    Node * head()
    {
        return m_head;
    }

    /**
     * Get the first Node on the List (read-only).
     */
    const Node * head() const
    {
        return m_head;
    }

    /**
     * Get the last Node on the list.
     *
     * @return Last Node on the list.
     */
    Node * tail()
    {
        return m_tail;
    }

    /**
     * Get the last Node on the List (read-only).
     */
    const Node * tail() const
    {
        return m_tail;
    }

    /**
     * Get the first value in the list.
     *
     * @return First value on the list.
     *
     * @note Assumes that the list is not empty.
     */
    T first()
    {
        return m_head->data;
    }

    /**
     * Get the first value as constant.
     *
     * @return First value on the list.
     *
     * @note Assumes that the list is not empty.
     */
    const T first() const
    {
        return m_head->data;
    }

    /**
     * Get the last value on the list.
     *
     * @return Last value on the list.
     *
     * @note Assumes that the list is not empty.
     */
    T last()
    {
        return m_tail->data;
    }

    /**
     * Get the last value on the list as constant.
     *
     * @return Last value on the list.
     *
     * @note Assumes that the list is not empty.
     */
    const T last() const
    {
        return m_tail->data;
    }

    /**
     * Get a pointer to the item at the given position.
     *
     * @param position Index in the list
     *
     * @return Pointer to the item or ZERO if not available.
     */
    virtual const T * get(Size position) const
    {
        Size num = 0;
        Node *node = m_head;

        // Is the index within bounds of the list?
        if (position >= m_count)
            return ZERO;

        // Find the item and return it.
        while (num++ < position)
            node = node->next;

        return &node->data;
    }

    /**
     * Get a reference to the item at the given position.
     *
     * @param position Index in the list.
     *
     * @return Reference to the item at the given position.
     *
     * @note Assumes that the item is available.
     */
    virtual const T & at(Size position) const
    {
        Size num = 0;
        Node *node = m_head;

        // Find the item and return it.
        while (num++ < position)
            node = node->next;

        return node->data;
    }

    /**
     * Check if the List is empty.
     *
     * @return true if empty, false if not.
     */
    virtual bool isEmpty() const
    {
        return !m_head;
    }

    /**
     * Get the size of the list.
     *
     * @return Size of the list.
     */
    Size size() const
    {
        return m_count;
    }

    /**
     * Get the number of items on the list.
     *
     * @return The number of items on the List.
     */
    Size count() const
    {
        return m_count;
    }

    /**
     * Append operator.
     */
    List & operator << (T t)
    {
        append(t);
        return (*this);
    }

    /**
     * Comparison operator.
     */
    bool operator == (const List<T> & lst) const
    {
        if (lst.count() != m_count)
            return false;

        for (Node *n = m_head; n; n = n->next)
            if (!lst.contains(n->data))
                return false;

        return true;
    }

    /**
     * Inequality operator.
     */
    bool operator != (const List<T> & lst) const
    {
        if (lst.count() != m_count)
            return true;

        for (Node *n = m_head; n; n = n->next)
            if (!lst.contains(n->data))
                return true;

        return false;
    }

  private:

    /** Head of the List. */
    Node *m_head;

    /** Tail of the list. */
    Node *m_tail;

    /** Number of items currently in the List. */
    Size m_count;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_LIST_H */
