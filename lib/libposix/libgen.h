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

#ifndef __LIBPOSIX_LIBGEN_H
#define __LIBPOSIX_LIBGEN_H

#include <Macros.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/**
 * @brief Return the last component of a pathname.
 *
 * The basename() function shall take the pathname
 * pointed to by path and return a pointer to the
 * final component of the pathname, deleting any
 * trailing '/' characters.
 *
 * @param path Filesystem path.
 *
 * @return String pointer to the last component.
 */
extern C char * basename(char *path);

/**
 * @brief Return the directory portion of a pathname.
 *
 * @param path Filesystem path
 *
 * @return String pointer to the directory of pathname.
 */
extern C char * dirname(char *path);

/**
 * @}
 * @}
 */

#endif /* __LIBPOSIX_LIBGEN_H */
