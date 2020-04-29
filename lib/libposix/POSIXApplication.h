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

#ifndef __LIBPOSIX_POSIXAPPLICATION_H
#define __LIBPOSIX_POSIXAPPLICATION_H

#include <Application.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/**
 * POSIX-compatible application
 */
class POSIXApplication : public Application
{
  public:

    /**
     * Class constructor.
     *
     * @param argc Argument count
     * @param argv Argument values
     */
    POSIXApplication(int argc, char **argv);

    /**
     * Class destructor.
     */
    virtual ~POSIXApplication();

  protected:

    /**
     * Print text to output.
     *
     * @param string Text to print to program output.
     *
     * @return Result code.
     */
    virtual Result output(const char *string) const;
};

/**
 * @}
 * @}
 */

#endif /* __LIBPOSIX_POSIXAPPLICATION_H */
