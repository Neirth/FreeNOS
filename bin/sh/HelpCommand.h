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

#ifndef __SH_HELPCOMMAND
#define __SH_HELPCOMMAND

#include <Types.h>
#include "ShellCommand.h"

class Shell;

/**
 * @addtogroup bin
 * @{
 *
 * @addtogroup sh
 * @{
 */

/**
 * Prints the help info for all known ShellCommands.
 */
class HelpCommand : public ShellCommand
{
  public:

    /**
     * Constructor.
     */
    HelpCommand(Shell *shell);

    /**
     * Executes the command.
     *
     * @param nparams Number of parameters given.
     * @param params Array of parameters.
     * @return Error code or zero on success.
     */
    virtual int execute(Size nparams, char **params);

  private:

    /** Shell object */
    Shell *m_shell;
};

/**
 * @}
 * @}
 */

#endif /* __SH_HELPCOMMAND */
