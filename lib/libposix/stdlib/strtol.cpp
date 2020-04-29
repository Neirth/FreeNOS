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

#include <String.h>
#include <Types.h>
#include <Character.h>
#include "stdlib.h"

extern C long strtol(const char *nptr, char **endptr, int base)
{
    String s = nptr;
    Number::Base b = Number::Dec;

    switch (base)
    {
        case 10: b = Number::Dec; break;
        case 16: b = Number::Hex; break;
    }

    if (*nptr && *nptr == '-')
        nptr++;

    if (*nptr && *nptr == '0' && *(nptr+1) && *(nptr+1) == 'x')
        nptr += 2;

    while (Character::isDigit(*nptr))
        nptr++;

    *endptr = (char *) nptr;

    return s.toLong(b);
}
