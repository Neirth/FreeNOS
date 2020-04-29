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

#ifndef __SH_CHANGE_DIR_COMMAND
#define __SH_CHANGE_DIR_COMMAND

#include <Types.h>
#include "ShellCommand.h"

/**
 * @addtogroup bin
 * @{
 *
 * @addtogroup sh
 * @{
 */

/**
 * Change the current working directory.
 */
class ChangeDirCommand : public ShellCommand
{
  public:

    /**
     * Constructor function.
     */
    ChangeDirCommand();

    /**
     * Executes the command.
     * @param nparams Number of parameters given.
     * @param params Array of parameters.
     * @return Error code or zero on success.
     */
    virtual int execute(Size nparams, char **params);
};

/**
 * @}
 */

#endif /* __SH_CHANGE_DIR_COMMAND */
