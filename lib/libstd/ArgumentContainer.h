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

#ifndef __LIBSTD_ARGUMENTCONTAINER_H
#define __LIBSTD_ARGUMENTCONTAINER_H

#include "Types.h"
#include "Macros.h"
#include "String.h"
#include "HashTable.h"
#include "HashIterator.h"
#include "Vector.h"
#include "Argument.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Generic command-line argument parser.
 */
class ArgumentContainer
{
  public:

    /**
     * Result codes.
     */
    enum Result
    {
        Success,
        InvalidArgument,
        NotFound
    };

  public:

    /**
     * Constructor
     */
    ArgumentContainer();

    /**
     * Destructor
     */
    virtual ~ArgumentContainer();

    /**
     * Get argument by name
     *
     * @param name Name of the argument
     *
     * @return String object pointer if found or ZERO if not found
     */
    const char * get(const char *name) const;

    /**
     * Get positional arguments.
     *
     * @return Vector object reference
     */
    const Vector<Argument *> & getPositionals() const;

    /**
     * Get flag arguments.
     *
     * @return HashTable object reference
     */
    const HashTable<String, Argument *> & getFlags() const;

    /**
     * Add positional argument.
     *
     * @param arg Positional argument
     *
     * @return Result code
     */
    Result addPositional(Argument *arg);

    /**
     * Add flag argument.
     *
     * @param arg Flag argument
     *
     * @return Result code
     */
    Result addFlag(Argument *arg);

  private:

    /** Contains all flag arguments. */
    HashTable<String, Argument *> m_flags;

    /** Contains all positional arguments. */
    Vector<Argument *> m_positionals;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_ARGUMENTCONTAINER_H */
