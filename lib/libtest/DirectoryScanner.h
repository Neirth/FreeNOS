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

#ifndef __LIBTEST_DIRECTORYSCANNER_H
#define __LIBTEST_DIRECTORYSCANNER_H

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libtest
 * @{
 */

/**
 * Scan directory for automated tests
 */
class DirectoryScanner
{
  public:

    /**
     * Constructor
     *
     * @param argc Program argument count
     * @param argv Program argument values
     */
    DirectoryScanner(int argc, char **argv);

    /**
     * Scan filesystem path for tests
     *
     * @param path Input filesystem path
     *
     * @return Zero on success, non-zero otherwise
     */
    int scan(const char *path);

  private:

    /** Program argument count */
    int m_argc;

    /** Program argument values */
    char **m_argv;
};

/**
 * @}
 * @}
 */

#endif /* __LIBTEST_DIRECTORYSCANNER_H */
