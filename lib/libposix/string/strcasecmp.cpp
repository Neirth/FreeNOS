/*
 * Copyright (C) 2009 Coen Bijlsma
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

#include "string.h"
#include <ctype.h>

int strcasecmp(const char *dest, const char *src )
{
    while ( *dest && *src && (tolower((int)*dest) == tolower((int)*src)) )
    {
        dest++;
        src++;
    }
    return (tolower((int)*dest) - tolower((int)*src));
}
