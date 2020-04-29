/**
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

#include <time.h>

unsigned long mktime(const unsigned int year0, const unsigned int month0,
                     const unsigned int day, const unsigned int hour,
                     const unsigned int min, const unsigned int sec)
{
    unsigned int month = month0, year = year0;

    // 1..12 -> 11,12,1..10
    if( 0 >= (int) (month -= 2))
    {
        month += 12; // Puts Feb last since it has leap day
        year -= 1;
    }

    return ((((unsigned long)
                (year/4 - year/100 + year/400 + 367*month/12 + day) +
                year*365 - 719499
            )*24 + hour // now have hours
        )*60 + min // now have minutes
    )*60 + sec; // finally seconds
}
