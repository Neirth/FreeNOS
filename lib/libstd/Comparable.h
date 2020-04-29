/*
 * Copyright (C) 2009 Niek Linnenbank
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

#ifndef __LIBSTD_COMPARABLE_H
#define __LIBSTD_COMPARABLE_H

#include "Types.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Objects which can be compared to each other.
 */
template <class T> class Comparable
{
  public:

    /**
     * Class destructor.
     */
    virtual ~Comparable() {}

    /**
     * Test if an object is equal to an other object.
     *
     * @param t Object instance.
     *
     * @return True if equal, false otherwise.
     */
    virtual bool equals(const T &t) const = 0;

    /**
     * Compares this Comparable to the given Comparable.
     *
     * This function checks whether this Comparable is equal to,
     * less, or greater then the given Comparable.
     *
     * @param c The Comparable to compare us to.
     *
     * @return an int < 0, 0, > 0 if we are respectively less then,
     *         equal to or greater then the given Comparable.
     */
    virtual int compareTo(const T &t) const = 0;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_COMPARABLE_H */
