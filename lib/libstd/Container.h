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

#ifndef __LIBSTD_CONTAINER_H
#define __LIBSTD_CONTAINER_H

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
 * Containers provide access to stored items.
 */
class Container
{
  public:

    /**
     * Constructor.
     */
    Container();

    /**
     * Destructor.
     */
    virtual ~Container();

    /**
     * Returns the maximum size of this Container.
     *
     * @return size The maximum size of this Container.
     */
    virtual Size size() const = 0;

    /**
     * Returns the number of items inside the Container.
     *
     * @return Number of items inside the Container.
     */
    virtual Size count() const = 0;

    /**
     * Removes all items from the Container.
     */
    virtual void clear() = 0;

    /**
     * Check if the Container is empty.
     *
     * @return True if empty, false otherwise.
     */
    virtual bool isEmpty() const;

    /**
     * Ensure that at least the given size is available.
     *
     * @param size Size to reserve in the Container
     *
     * @return True if the given size is reserved, false otherwise.
     */
    virtual bool reserve(Size size);

    /**
     * Shrink the container size by the given amount of items.
     *
     * @param size Number of items to shrink.
     *
     * @return True if shrink succeeded, false otherwise.
     */
    virtual bool shrink(Size size);

    /**
     * Try to minimize the memory required in the Container.
     *
     * @return New size of the Container.
     */
    virtual Size squeeze();

    /**
     * Change the size of the Container.
     *
     * @param size New size of the container.
     *
     * @return True if the resize succeeded, false otherwise.
     */
    virtual bool resize(Size size);
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_CONTAINER_H */
