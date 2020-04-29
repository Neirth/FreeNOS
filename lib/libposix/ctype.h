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

#ifndef __LIBC_CTYPE_H
#define __LIBC_CTYPE_H

#include <Character.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/**
 * Test for a decimal digit.
 *
 * @param c Input character.
 */
#define isdigit(c) Character::isDigit(c)

/**
 * Test for a lowercase letter.
 *
 * @param c Input character.
 *
 * @return Boolean
 */
#define islower(c) Character::isLower(c)

/**
 * Test for an uppercase letter.
 *
 * @param c Input character.
 *
 * @return Boolean
 */
#define isupper(c) Character::isUpper(c)

/**
 * Test for an alphabetic character.
 *
 * @param c Input character.
 *
 * @return Boolean
 */
#define isalpha(c) Character::isAlpha(c)

/**
 * Test for an alphanumeric character.
 *
 * @param c Input character.
 *
 * @return Boolean
 */
#define isalnum(c) Character::isAlnum(c)

/**
 * Test for a blank character.
 *
 * @param c Input character.
 *
 * @return Boolean
 */
#define isblank(c) Character::isBlank(c)

/**
 * Test for a white-space character.
 *
 * @param c Input character.
 *
 * @return Boolean
 */
#define isspace(c) Character::isSpace(c)

/**
 * Converts the letter c to lowercase.
 *
 * @param c The letter to convert.
 *
 * @return The converted letter, or c if conversion was not possible.
 */
#define tolower(c) Character::lower(c)

/**
 * Converts the letter c to uppercase.
 *
 * @param c The letter to convert.
 *
 * @return The converted letter, or c if conversion was not possible.
 */
#define toupper(c) Character::upper(c)

/**
 * @}
 * @}
 */

#endif /* __LIBC_CTYPE_H */
