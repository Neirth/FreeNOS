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

#include "Assert.h"
#include "HashFunction.h"

Size hash(const String & key, Size mod)
{
    Size ret  = FNV_INIT;
    Size size = key.length();

    assertRead(key);
    assert(mod > 0);

    for (Size i = 0; i < size; i++)
    {
        ret *= FNV_PRIME;
        ret ^= key[i];
    }
    return (ret % mod);
}

Size hash(int key, Size mod)
{
    Size ret = FNV_INIT;

    assert(mod > 0);

    for (Size i = 0; i < 4; i++)
    {
        ret *= FNV_PRIME;
        ret ^= (((uint) key) >> (i*8)) & 0xff;
    }
    return (ret % mod);
}
