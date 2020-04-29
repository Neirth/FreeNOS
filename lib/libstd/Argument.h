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

#ifndef __LIBSTD_ARGUMENT_H
#define __LIBSTD_ARGUMENT_H

#include "Types.h"
#include "String.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Represents program command line argument
 */
class Argument
{
  public:

    /**
     * Class constructor
     *
     * @param name Input argument name
     */
    Argument(const char *name);

    /**
     * Class constructor
     *
     * @param name Input argument name
     */
    Argument(const String & name);

    /**
     * Get single character identifier
     *
     * @return Single character identifier
     */
    char getIdentifier() const;

    /**
     * Retrieve argument name
     *
     * @return Argument name
     */
    const String & getName() const;

    /**
     * Retrieve single line argument description
     *
     * @return Argument description
     */
    const String & getDescription() const;

    /**
     * Retrieve argument option value (if any)
     *
     * @return Argument value
     */
    const String & getValue() const;

    /**
     * Retrieve maximum argument count (if set)
     *
     * @return Maximum argument count
     */
    Size getCount() const;

    /**
     * Set argument identifier
     *
     * @param id Argument identifier
     */
    void setIdentifier(char id);

    /**
     * Set argument name
     *
     * @param name New argument name
     */
    void setName(const char *name);

    /**
     * Set argument single line description
     *
     * @param description New argument description
     */
    void setDescription(const char *description);

    /**
     * Set argument option value
     *
     * @param value New argument option value
     */
    void setValue(const char *value);

    /**
     * Set argument maximum count
     *
     * @param count New argument maximum count
     */
    void setCount(Size count);

    /**
     * Equality operator
     *
     * @param arg Argument reference
     *
     * @return Boolean
     */
    bool operator == (const Argument & arg) const;

    /**
     * Non-equality operator
     *
     * @param arg Argument reference
     *
     * @return Boolean
     */
    bool operator != (const Argument & arg) const;

  private:

    /** Argument identifier */
    char m_id;

    /** Argument name */
    String m_name;

    /** Argument description in a single line */
    String m_description;

    /** Optional argument value */
    String m_value;

    /** Maximum argument count */
    Size m_count;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_ARGUMENT_H */
