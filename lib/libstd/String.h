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

#ifndef __LIBSTD_STRING_H
#define __LIBSTD_STRING_H

#include "Types.h"
#include "Macros.h"
#include "Assert.h"
#include "Sequence.h"
#include "List.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/** Default maximum length of a String's value. */
#define STRING_DEFAULT_SIZE 64

/**
 * Abstraction of strings.
 */
class String : public Sequence<char>
{
  using Sequence<char>::compareTo;
  using Sequence<char>::equals;

  public:

    /**
     * Default constructor.
     *
     * Constructs an empty string with the default size.
     */
    String();

    /**
     * Copy constructor.
     *
     * @param s String reference.
     */
    String(const String & str);

    /**
     * Constructor.
     *
     * @param s Initial value of the String.
     * @param copy If true allocate and copy the input buffer.
     */
    String(char *s, bool copy = true);

    /**
     * Constant constructor.
     *
     * @param s Initial value of the String.
     * @param copy If true allocate and copy the input buffer.
     */
    String(const char *s, bool copy = false);

    /**
     * Signed integer constructor.
     *
     * Creates a String with the textual representation
     * of the given signed integer.
     *
     * @param number Integer number of the new String.
     */
    String(int number);

    /**
     * Destructor
     */
    virtual ~String();

    /**
     * Calculates the length of the String.
     *
     * @return Length of the current String value.
     */
    virtual Size size() const;

    /**
     * Number of characters in the string.
     *
     * @return Number of characters
     */
    virtual Size count() const;

    /**
     * Same as count().
     *
     * @see count
     */
    Size length() const;

    /**
     * Get the length of the given character string.
     *
     * @param str Input string
     *
     * @return String length in bytes
     */
    static Size length(char *str);

    /**
     * Get the length of a constant character string.
     *
     * @param str Input string
     *
     * @return String length
     */
    static Size length(const char *str);

    /**
     * Change the size of the String buffer.
     *
     * @param size New size of the String.
     *
     * @return True if resized successfully, false otherwise.
     */
    virtual bool resize(Size size);

    /**
     * Make sure at least given number of bytes available.
     *
     * @param count The number of free bytes to guarantee.
     *
     * @return True if success, false otherwise.
     */
    virtual bool reserve(Size count);

    /**
     * Returns the item at the given position.
     *
     * @param position The position of the item to get.
     *
     * @return Pointer to the item at the given position or ZERO if no item available.
     */
    virtual const char * get(Size position) const;

    /**
     * Returns a reference to the item at the given position.
     *
     * @param position Valid index inside this array.
     *
     * @return Reference to the item at the given position
     *
     * @note Position must be a valid index.
     */
    virtual const char & at(Size position) const;

    /**
     * Return value at the given position.
     *
     * If position is not within bounds of this array,
     * this function will return a default constructed T.
     *
     * @param position Index inside this array.
     *
     * @return T at the given position or default constructed T.
     */
    virtual const char value(Size position) const;

    /**
     * Check if the given character occurs in the String.
     *
     * @param character The character to search for.
     *
     * @return True if found, false otherwise.
     */
    virtual bool contains(char character) const;

    /**
     * Tests if this String starts with the specified prefix.
     *
     * @param prefix String prefix.
     *
     * @return True if matched, false otherwise.
     */
    bool startsWith(String & prefix) const;

    /**
     * Tests if this String starts with the specified prefix.
     *
     * @param prefix String prefix.
     *
     * @return True if matched, false otherwise.
     */
    bool startsWith(const char * prefix) const;

    /**
     * Tests if this String ends with the specified suffix.
     *
     * @param suffix The suffix String.
     *
     * @return True if matched, false otherwise.
     */
    bool endsWith(String & suffix) const;

    /**
     * Tests if this String ends with the specified suffix.
     *
     * @param suffix The suffix String.
     *
     * @return True if matched, false otherwise.
     */
    bool endsWith(const char * suffix) const;

    /**
     * Compares this String to the given String.
     *
     * @param s The String to compare us to.
     *
     * @return int < 0, 0, > 0 if we are greater than, equal to
     *         or less then the given String.
     */
    virtual int compareTo(const String & str) const;

    /**
     * Compare with another String.
     *
     * @param s String instance to compare against.
     * @param caseSensitive True if uppercase characters are considered
     *                      not equal to lowercase, false otherwise.
     *
     * @return Zero if equal, negative if smaller or positive if greater.
     */
    virtual int compareTo(const String & str,
                          bool caseSensitive = true) const;

    /**
     * Compare with a character string.
     *
     * @param str Character string to compare against.
     * @param caseSensitive True if uppercase characters are considered
     *                      not equal to lowercase, false otherwise.
     * @param count Number of character to compare or ZERO for whole strings.
     *
     * @return Zero if equal, negative if smaller or positive if greater.
     */
    virtual int compareTo(const char *str,
                          bool caseSensitive = true,
                          Size count = 0) const;

    /**
     * Alias for compareTo().
     */
    virtual bool equals(const String &str) const;

    /**
     * Matches the String against a mask.
     *
     * @param mask Pattern to match against.
     *
     * @return True if match, false otherwise.
     */
    bool match(const char *mask) const;

    /**
     * Returns a new String that contains a copy of this String.
     *
     * This function copies the input starting from index (inclusive),
     * and copies at most size characters.
     *
     * @param index The begin index to create the substring of.
     * @param size The maximum size of the substring.
     *
     * @return String* a pointer to the newly created substring.
     */
    String substring(Size index, Size size = 0);

    /**
     * Split the String into parts separated by a delimiter.
     *
     * @param delimiter Character to use a delimiter.
     *
     * @return List of Strings representing the parts.
     */
    List<String> split(char delimiter);

    /**
     * Split the String into parts separated by a delimiter.
     *
     * @param delimiter String which acts as a delimiter.
     *
     * @return List of Strings representing the parts.
     */
    List<String> split(const String & delimiter);

    /**
     * Convert the String to a signed long integer.
     *
     * @param base NumberBase to use. Default is decimal.
     *
     * @return Signed long integer
     */
    long toLong(Number::Base base = Number::Dec);

    /**
     * Pad line with trailing whitespace.
     *
     * This function ensures the last line inside the String
     * is at least the given length in characters, by appending
     * extra whitespace at the end of the line.
     *
     * @param length Length of the String including whitespace pads
     *
     * @return Reference to the String
     */
    String & pad(Size length);

    /**
     * Remove leading and trailing whitespace from the String.
     *
     * @return Reference to the String.
     */
    String & trim();

    /**
     * Convert all Characters to lower case.
     *
     * @return Reference to the String.
     */
    String & lower();

    /**
     * Convert all Characters to upper case.
     *
     * @return Reference to the String.
     */
    String & upper();

    /**
     * Set text-representation of a signed number.
     *
     * @param string Destination character string buffer or ZERO to fill internal String buffer.
     */
    Size set(long number, Number::Base base = Number::Dec, char *string = ZERO);

    /**
     * Set text-representation of an unsigned number.
     *
     * @param number Input number.
     * @param base Numberal base type.
     * @param string Destination character string buffer or ZERO to fill internal String buffer.
     * @param sign Threat input number as signed.
     */
    Size setUnsigned(ulong number,
                     Number::Base base = Number::Dec,
                     char *string = ZERO,
                     bool sign = false);

    /**
     * Assignment operator.
     *
     * @param s Constant string.
     */
    void operator = (const char *str);

    /**
     * Assignment operator.
     *
     * @param str Input string
     */
    void operator = (const String & str);

    /**
     * Comparision operator.
     *
     * @param str Input string
     */
    bool operator == (const String & str) const;

    /**
     * Inequal operator.
     *
     * @param str Input string
     */
    bool operator != (const String & str) const;

    /**
     * Dereference operator (read-only).
     */
    const char * operator * () const;

    /**
     * Dereference operator.
     */
    char * operator * ();

    /**
     * Append character string to the String.
     */
    String & operator << (const char *str);

    /**
     * Append String to another String.
     */
    String & operator << (const String & str);

    /**
     * Append the given signed number as text to the String.
     */
    String & operator << (int number);

    /**
     * Append the given unsigned number as text to the String.
     */
    String & operator << (unsigned int number);

    /**
     * Append the pointer as hexidecimal unsigned number to the String.
     */
    String & operator << (void *pointer);

    /**
     * Change the default number format representation.
     */
    String & operator << (Number::Base format);

  private:

    /** Current value of the String. */
    char *m_string;

    /** Size of the string buffer, including any NULL byte(s) at the end. */
    Size m_size;

    /** Length of the string text, excluding NULL byte(s) at the end. */
    Size m_count;

    /** True if the string buffer is a deep copy, false otherwise. */
    bool m_allocated;

    /** Number format to use for convertions. */
    Number::Base m_base;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_STRING_H */
