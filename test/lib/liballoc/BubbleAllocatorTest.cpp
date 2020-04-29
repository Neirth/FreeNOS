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

#include <FreeNOS/System.h>
#include <TestCase.h>
#include <TestRunner.h>
#include <TestInt.h>
#include <TestMain.h>
#include <BubbleAllocator.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

TestCase(BubbleConstruct)
{
    TestInt<uint> addresses(UINT_MIN, UINT_MAX);
    TestInt<uint> sizes(PAGESIZE, PAGESIZE * 16);

    BubbleAllocator ba(addresses.random(),
                       sizes.random());

    testAssert(ba.m_start == (u8 *) addresses[0]);
    testAssert(ba.m_size == sizes[0]);

    return OK;
}
