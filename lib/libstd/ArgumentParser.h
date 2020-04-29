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

#ifndef __LIBSTD_ARGUMENTPARSER_H
#define __LIBSTD_ARGUMENTPARSER_H

#include "Types.h"
#include "Macros.h"
#include "String.h"
#include "HashTable.h"
#include "Vector.h"
#include "Argument.h"
#include "ArgumentContainer.h"

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
class ArgumentParser
{
  public:

    /**
     * Result codes.
     */
    enum Result
    {
        Success,
        InvalidArgument,
        NotFound,
        AlreadyExists
    };

  public:

    /**
     * Constructor
     */
    ArgumentParser();

    /**
     * Destructor
     */
    virtual ~ArgumentParser();

    /**
     * Get program usage.
     *
     * @return Program usage string reference.
     */
    String getUsage() const;

    /**
     * Retrieve program name.
     *
     * @return Program name
     */
    const String & name() const;

    /**
     * Set program name.
     *
     * @param name Program name string.
     */
    void setName(const char *name);

    /**
     * Set program description.
     *
     * @param desc Program description string.
     */
    void setDescription(const String & desciption);

    /**
     * Register a flag Argument
     *
     * @param arg Argument identifier
     * @param name Argument name
     * @param description Argument single line description
     *
     * @return Result code.
     */
    Result registerFlag(char arg,
                        const char *name,
                        const char *description);

    /**
     * Register a positional argument.
     *
     * @param name Argument name
     * @param description Argument single line description
     * @param count Maximum number to set for this positional argument
     *
     * @return Result code.
     */
    Result registerPositional(const char *name,
                              const char *description,
                              Size count = 1);

    /**
     * Parse input arguments.
     *
     * This function parses the given arguments and
     * fills the argument container with parsed arguments.
     *
     * @param argc Input argument count.
     * @param argv Input argument values.
     * @param output Filled with parsed arguments on output.
     *
     * @return Result code.
     */
    Result parse(int argc,
                 char **argv,
                 ArgumentContainer & output);

  private:

    /** Contains all registered flag arguments by name. */
    HashTable<String, Argument *> m_flags;

    /** Contains all registered flag arguments by single character identifier. */
    HashTable<String, Argument *> m_flagsId;

    /** Contains all registered positional arguments. */
    Vector<Argument *> m_positionals;

    /** Program name */
    String m_name;

    /** Program description */
    String m_description;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_ARGUMENTPARSER_H */
