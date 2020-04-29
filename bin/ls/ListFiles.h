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

#ifndef __BIN_LS_LS_H
#define __BIN_LS_LS_H

#include <POSIXApplication.h>

/**
 * @addtogroup bin
 * @{
 */

/**
 * List files on the filesystem.
 */
class ListFiles : public POSIXApplication
{
  public:

    /**
     * Constructor
     *
     * @param argc Argument count
     * @param argv Argument values
     */
    ListFiles(int argc, char **argv);

    /**
     * Destructor
     */
    virtual ~ListFiles();

    /**
     * Execute the application.
     *
     * @return Result code
     */
    virtual Result exec();

  private:

    /**
     * List files on the filesystem
     *
     * @param path Path to the file(s) to list
     *
     * @return Result code
     */
    Result printFiles(const String & path) const;

    /**
     * List single file on the filesystem
     *
     * @param path Path to the file to list
     * @param out String to write the output to
     *
     * @return Result code
     */
    Result printSingleFile(const String & path, String & out) const;
};

/**
 * @}
 */

#endif /* __BIN_LS_LS_H */
