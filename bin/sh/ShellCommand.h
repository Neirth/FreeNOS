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

#ifndef __SH_SHELLCOMMAND
#define __SH_SHELLCOMMAND

#include <String.h>
#include <HashTable.h>
#include <Types.h>

/**
 * @addtogroup bin
 * @{
 *
 * @addtogroup sh
 * @{
 */

/**
 * Builtin command for the Shell.
 *
 * @see Shell
 */
class ShellCommand
{
  public:

    /**
     * Constructor.
     *
     * @param name Unique name of the command.
     * @param minParams Minimum number of parameters required by this command.
     */
    ShellCommand(const char *name, Size minParams = 0);

    /**
     * Destructor.
     */
    virtual ~ShellCommand();

    /**
     * Get command name.
     *
     * @return Command name
     */
    const char * getName() const;

    /**
     * Get command help.
     *
     * @return Command help
     */
    const char * getHelp() const;

    /**
     * Get the minimum number of parameters required.
     *
     * @return Minimum number of parameters required.
     */
    Size getMinimumParams() const;

    /**
     * Executes the command.
     *
     * @param nparams Number of parameters given.
     * @param params Array of parameters.
     * @return Error code or zero on success.
     */
    virtual int execute(Size nparams, char **params) = 0;

  protected:

    /** Unique name of the command. */
    const char *m_name;

    /** Command help text. */
    const char *m_help;

    /** Minimum number of parameters required. */
    Size m_minParams;
};

/**
 * @}
 * @}
 */

#endif /* __SH_SHELLCOMMAND */
