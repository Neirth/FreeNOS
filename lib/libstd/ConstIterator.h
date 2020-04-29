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

#ifndef __LIBSTD_CONSTITERATOR_H
#define __LIBSTD_CONSTITERATOR_H

#include "Types.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Abstracts an iteration process for a constant.
 */
template <class T> class ConstIterator
{
  public:

    /**
     * Destructor.
     */
    virtual ~ConstIterator() {}

    /**
     * Restart iteration from the beginning.
     */
    virtual void reset() = 0;

    /**
     * Check if there is more to iterate.
     *
     * @return true if more items, false if not.
     */
    virtual bool hasNext() const = 0;

    /**
     * Check if there is a current item.
     *
     * @return True if the iterator has a current item, false otherwise.
     */
    virtual bool hasCurrent() const = 0;

    /**
     * Get the current item read-only.
     *
     * @return Reference to the next item.
     */
    virtual const T & current() const = 0;

    /**
     * Fetch the next item.
     *
     * This function first fetches the next item
     * and then updates the current item pointer to that item.
     *
     * @return Reference to the next item.
     */
    virtual const T & next() = 0;

    /**
     * Increment operator.
     *
     * This function first increment the current item
     * and then updates the next item pointer.
     *
     * @param num Ignored
     */
    virtual void operator ++(int num) = 0;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_CONSTITERATOR_H */
