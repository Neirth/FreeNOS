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

#ifndef __LIBSTD_CHARACTER_H
#define __LIBSTD_CHARACTER_H

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

namespace Character
{
    /**
     * Test for a decimal digit.
     *
     * @param c Input character.
     *
     * @return Boolean
     */
    inline bool isDigit(char c)
    {
        return c >= '0' && c <= '9';
    }

    /**
     * Test for a wildcard character.
     *
     * @param c Input character.
     *
     * @return True if wildcard, false otherwise.
     */
    inline bool isWildcard(char c)
    {
        return c == '*';
    }

    /**
     * Test for a lowercase letter.
     *
     * @param c Input character.
     *
     * @return Boolean
     */
    inline bool isLower(char c)
    {
        return (c) >= 'a' && (c) <= 'z';
    }

    /**
     * Test for an uppercase letter.
     *
     * @param c Input character.
     *
     * @return Boolean
     */
    inline bool isUpper(char c)
    {
        return (c) >= 'A' && (c) <= 'Z';
    }

    /**
     * Test for an alphabetic character.
     *
     * @param c Input character.
     *
     * @return Boolean
     */
    inline bool isAlpha(char c)
    {
        return isUpper(c) || isLower(c);
    }

    /**
     * Test for an alphanumeric character.
     *
     * @param c Input character.
     *
     * @return Boolean
     */
    inline bool isAlnum(char c)
    {
        return isAlpha(c) || isDigit(c);
    }

    /**
     * Test for a blank character.
     *
     * @param c Input character.
     *
     * @return Boolean
     */
    inline bool isBlank(char c)
    {
        return (c) == ' ' || (c) == '\t';
    }

    /**
     * Test for a white-space character.
     *
     * @param c Input character.
     *
     * @return Boolean
     */
    inline bool isWhitespace(char c)
    {
        return (c) == '\v' || (c) == '\f' ||
               (c) == '\r' || (c) == '\n' || isBlank(c);
    }

    /**
     * Converts the letter c to lowercase.
     *
     * @param c The letter to convert.
     *
     * @return The converted letter, or c if conversion was not possible.
     */
    inline char lower(char c)
    {
        return (c >= 'A' && c <= 'Z') ? (c + 32) : (c);
    }

    /**
     * Converts the letter c to uppercase.
     *
     * @param c The letter to convert.
     *
     * @return The converted letter, or c if conversion was not possible.
     */
    inline char upper(char c)
    {
        return (c >= 'a' && c <= 'z') ? (c - 32) : (c);
    }
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_CHARACTER_H */
