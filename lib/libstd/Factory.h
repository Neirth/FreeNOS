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

#ifndef __LIBSTD_FACTORY_H
#define __LIBSTD_FACTORY_H

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Abstract Factory design pattern.
 */
template <class T> class Factory
{
  public:

    /**
     * Create an instance of T.
     *
     * @return Pointer to T.
     */
    static T * create()
    {
        return new T();
    }
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_FACTORY_H */
