/*
 * Copyright (C) 2014 Niek Linnenbank
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

#ifndef __BIN_TOUCH_CREATEFILE_H
#define __BIN_TOUCH_CREATEFILE_H

#include <POSIXApplication.h>

/**
 * @addtogroup bin
 * @{
 */

/**
 * Create new files on the filesystem
 */
class CreateFile : public POSIXApplication
{
  public:

    /**
     * Constructor
     *
     * @param argc Argument count
     * @param argv Argument values
     */
    CreateFile(int argc, char **argv);

    /**
     * Destructor
     */
    virtual ~CreateFile();

    /**
     * Execute the application.
     *
     * @return Result code
     */
    virtual Result exec();

  private:

    /**
     * Create a new empty file
     *
     * @param file Name of the file to create
     *
     * @return Result code
     */
    Result createFile(const String & file) const;
};

/**
 * @}
 */

#endif /* __BIN_TOUCH_CREATEFILE_H */
