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

#ifndef __BIN_LOGIN_LOGIN_H
#define __BIN_LOGIN_LOGIN_H

#include <POSIXApplication.h>

/**
 * @addtogroup bin
 * @{
 */

/**
 * Login program which starts the user shell.
 */
class Login : public POSIXApplication
{
  public:

    /**
     * Constructor
     *
     * @param argc Argument count
     * @param argv Argument values
     */
    Login(int argc, char **argv);

    /**
     * Destructor
     */
    virtual ~Login();

    /**
     * Execute the application.
     *
     * @return Result code
     */
    virtual Result exec();

  private:

    /**
     * Read username from standard input
     *
     * @return Username of the user.
     */
    const char * getUsername() const;

    /**
     * Print the login program prompt
     */
    void printPrompt() const;
};

/**
 * @}
 */

#endif /* __BIN_LOGIN_LOGIN_H */
