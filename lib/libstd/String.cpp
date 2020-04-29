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

#include "Character.h"
#include "MemoryBlock.h"
#include "String.h"

String::String()
{
    m_string    = new char[STRING_DEFAULT_SIZE];
    m_string[0] = ZERO;
    m_allocated = true;
    m_size      = STRING_DEFAULT_SIZE;
    m_count     = 0;
    m_base      = Number::Dec;
}

String::String(const String & str)
{
    m_size      = str.m_size;
    m_count     = str.m_count;
    m_base      = str.m_base;
    m_string    = new char[m_size];
    m_allocated = true;
    MemoryBlock::copy(m_string, str.m_string, m_count + 1);
}

String::String(char *str, bool copy)
{
    m_count     = length(str);
    m_size      = m_count ? m_count + 1 : STRING_DEFAULT_SIZE;
    m_allocated = copy;
    m_base      = Number::Dec;

    if (copy)
    {
        m_string = new char[m_size];
        MemoryBlock::copy(m_string, str, m_count + 1);
    }
    else
        m_string = str;
}

String::String(const char *str, bool copy)
{
    m_count     = length(str);
    m_size      = m_count ? m_count + 1 : STRING_DEFAULT_SIZE;
    m_allocated = copy;
    m_base      = Number::Dec;

    if (copy)
    {
        m_string = new char[m_size];
        MemoryBlock::copy(m_string, str, m_count + 1);
    }
    else
        m_string = (char *) str;
}

String::String(int number)
{
    m_string    = new char[STRING_DEFAULT_SIZE];
    m_string[0] = ZERO;
    m_allocated = true;
    m_size      = STRING_DEFAULT_SIZE;
    m_count     = 0;
    m_base      = Number::Dec;

    set(number);
}

String::~String()
{
    if (m_allocated)
    {
        delete[] m_string;
        m_allocated = false;
    }
}

Size String::size() const
{
    return m_size;
}

Size String::count() const
{
    return m_count;
}

Size String::length() const
{
    return count();
}

Size String::length(char *str)
{
    return length((const char *) str);
}

Size String::length(const char *str)
{
    Size len = 0;

    while (*str++)
        len++;

    return len;
}

bool String::resize(Size size)
{
    char *buffer;

    // Refuse zero-sized Strings.
    if (size == 0)
        return false;

    // Chop-off String if the current buffer is larger.
    if (m_count >= size)
        m_count = size - 1;

    // Allocate buffer
    buffer = new char[size];
    if (!buffer)
        return false;

    // Copy the contents of the old buffer, if any.
    MemoryBlock::copy(buffer, m_string, m_count + 1);
    buffer[m_count] = ZERO;

    // Only cleanup the old buffer if it was previously allocated
    if (m_allocated)
        delete[] m_string;

    // Update administration
    m_string = buffer;
    m_allocated = true;
    m_size = size;
    return true;
}

bool String::reserve(Size count)
{
    if (!m_allocated || count > m_size - 1)
        return resize(count + 1);
    else
        return true;
}

const char * String::get(Size position) const
{
    return position < m_count ? m_string + position : ZERO;
}

const char & String::at(Size position) const
{
    return m_string[position];
}

const char String::value(Size position) const
{
    return m_string[position];
}

bool String::contains(char character) const
{
    for (Size i = 0; i < m_count - 1; i++)
        if (m_string[i] == character)
            return true;

    return false;
}

bool String::startsWith(String & prefix) const
{
    return startsWith(prefix.m_string);
}

bool String::startsWith(const char *prefix) const
{
    Size len = length(prefix);

    // If the prefix is larger than the String itself, it cannot match.
    if (!len || len > m_count)
        return false;

    // Compare against the prefix
    for (Size i = 0; i < len; i++)
        if (m_string[i] != prefix[i])
            return false;

    return true;
}

bool String::endsWith(String & suffix) const
{
    return endsWith(suffix.m_string);
}

bool String::endsWith(const char *suffix) const
{
    Size len = length(suffix);

    // If the suffix is larger than the String itself, it cannot match
    if (!len || len > m_count)
        return false;

    // Compare against the suffix.
    for (Size i = m_count - len, j = 0; i < m_count; i++, j++)
        if (m_string[i] != suffix[j])
            return false;

    return true;
}

int String::compareTo(const String & str) const
{
    return compareTo(str, true);
}

int String::compareTo(const String & str, bool caseSensitive) const
{
    return compareTo(str.m_string, caseSensitive, 0);
}

int String::compareTo(const char *str, bool caseSensitive, Size count) const
{
    const char *dest = m_string, *src = str;
    Size n = count;

    while (*dest && *src)
    {
        if (count && n-1 == 0)
            break;

        if (( caseSensitive && *dest != *src) ||
            (!caseSensitive && Character::lower(*dest) != Character::lower(*src)))
                break;

        dest++, src++, n--;
    }
    return *dest - *src;
}

bool String::equals(const String & str) const
{
    return compareTo(str.m_string, true, 0) == 0;
}

bool String::match(const char *mask) const
{
    const char *string = m_string;
    const char *end = ZERO;

    while (*string && *mask)
    {
        if (Character::isWildcard(*mask))
        {
            // Skip extra wildcards
            while (Character::isWildcard(*mask))
                mask++;

            // Find end of the string after the mask, if any.
            for (end = mask; *end && !Character::isWildcard(*end); end++)
                ;

            // If the wildcard was last, its a match.
            if (mask == end)
                return true;

            // Move the string forward until a match
            for (; *string; string++)
            {
                const char *s = string, *m = mask;

                while (*s && *m && *s == *m)
                    s++, m++;

                if (m == end)
                {
                    break;
                }
            }
        }
        else if (*string != *mask)
            break;

        if (*string) string++;
        if (*mask) mask++;
    }
    while (Character::isWildcard(*mask))
        mask++;

    return (*string == *mask);
}

String String::substring(Size index, Size size)
{
    // Make sure index is within bounds.
    if (index >= m_count)
        index = m_count;

    // Copy the string from the index.
    String str(m_string + index);

    // Set a ZERO byte at the right place, if needed.
    if (size && size < m_count-index)
    {
        str.m_string[size] = ZERO;
        str.m_count = size;
    }
    return str;
}

String & String::pad(Size length)
{
    Size idx = 0;

    // Look for the last newline character
    for (Size i = 0; i < m_count; i++)
        if (m_string[i] == '\n')
            idx = i;

    // Last line length
    Size curlen = m_count - (idx+1);

    // Skip if the line is already the given length
    if (length <= curlen)
        return (*this);

    if (reserve(m_count + length - curlen))
    {
        MemoryBlock::set(m_string + idx + curlen + 1, ' ', length-curlen);
        m_count += length-curlen;
        m_string[m_count] = ZERO;
    }
    return (*this);
}

String & String::trim()
{
    Size from = 0, to = m_count - 1;

    if (!m_count)
        return (*this);

    // Make sure the string is allocated
    reserve(m_count);

    // Skip before
    for (Size i = 0; i < m_count - 1; i++, from++)
        if (!Character::isWhitespace(m_string[i]))
            break;

    // Skip after
    for (Size i = m_count - 1; i > 0; i--, to--)
        if (!Character::isWhitespace(m_string[i]))
            break;

    // Copy actual string content
    if (from < to)
    {
        MemoryBlock::copy(m_string, m_string + from, to-from+2);
        m_count = to - from + 1;
    }
    return (*this);
}

String & String::lower()
{
    // Make sure the string is allocated
    reserve(m_count);

    for (Size i = 0; i < m_count; i++)
        m_string[i] = Character::lower(m_string[i]);

    return (*this);
}

String & String::upper()
{
    // Make sure the string is allocated
    reserve(m_count);

    for (Size i = 0; i < m_count; i++)
        m_string[i] = Character::upper(m_string[i]);

    return (*this);
}

List<String> String::split(char delimiter)
{
    char str[2];
    str[0] = delimiter;
    str[1] = ZERO;

    String s(str);

    return split(s);
}

List<String> String::split(const String & delimiter)
{
    List<String> lst;
    String copy(m_string);
    Size from = 0, i = 0;

    // Save copy string pointer
    char *saved = copy.m_string;

    // Loop the String.
    while (i < m_count)
    {
        // Find delimiter
        if (copy.compareTo(delimiter.m_string, true, delimiter.m_count) == 0)
        {
            copy.m_string += delimiter.m_count;

            if (i > from)
            {
                String sub = substring(from, i - from);
                lst.append(sub);
            }
            from = i + delimiter.m_count;
            i += delimiter.m_count;
        }
        else
            copy.m_string++, i++;
    }
    // Append last part, if no more delimiters found
    if (from < m_count)
    {
        String sub = substring(from);
        lst.append(sub);
    }

    // Restore saved
    copy.m_string = saved;
    return lst;
}

long String::toLong(Number::Base base)
{
    const char *s = m_string;
    long acc = 0, cutoff;
    bool negative = false;
    int cutlim, basenum = 10;

    // Set the number base
    switch (base)
    {
        case Number::Dec: basenum = 10; break;
        case Number::Hex: basenum = 16; break;
    }

    // Skip whitespace
    while (Character::isWhitespace(*s))
        s++;

    // Negative number?
    if (s[0] == '-')
    {
        negative = true; s++;
    }

    // Skip '0x' prefix
    if (basenum == 16 && s[0] == '0' && s[1] == 'x')
        s += 2;

    // Compute output range limits
    cutoff = negative ? LONG_MIN : LONG_MAX;
    cutlim = cutoff % basenum;
    cutoff /= basenum;

    if (negative) {
        if (cutlim > 0) {
            cutlim -= base;
            cutoff += 1;
        }
        cutlim = -cutlim;
    }

    while (1)
    {
        unsigned char c = (unsigned char) *s++;

        if (Character::isDigit(c))
            c -= '0';
        else if (Character::isAlpha(c))
            c -= Character::isUpper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;

        if (c >= basenum)
            break;

        if (negative)
        {
            if (acc < cutoff || (acc == cutoff && c > cutlim))
            {
                acc = LONG_MIN;
            } else {
                acc *= basenum;
                acc -= c;
            }
        } else {
            if (acc > cutoff || (acc == cutoff && c > cutlim))
            {
                acc = LONG_MAX;
            } else {
                acc *= basenum;
                acc += c;
            }
        }
    }
    return acc;
}

Size String::set(long number, Number::Base base, char *string)
{
    return setUnsigned((ulong) number, base, string, true);
}

Size String::setUnsigned(ulong number, Number::Base base, char *string, bool sign)
{
    char *p, *p1, *p2, *saved, tmp;
    unsigned long ud = number;
    int remainder, divisor = 10;
    Size written = 0;

    // If needed, make sure enough allocated space is available.
    if (!string)
        reserve(STRING_DEFAULT_SIZE - 1);

    // Set target buffer
    p = string ? string : m_string;

    // Set divider according to the number system base.
    switch (base)
    {
        case Number::Dec: divisor = 10; break;
        case Number::Hex: divisor = 16; break;
    };

    // Negative prefix.
    if (sign && (long)number < 0)
    {
        *p++ = '-';
        ud = -number;
        written++;
    }
    // Add '0x' prefix for hexadecimal numbers
    if (base == Number::Hex)
    {
        *p++ = '0';
        *p++ = 'x';
        written += 2;
    }
    saved = p;

    // Divide ud by the divisor, until ud == 0
    do
    {
        remainder = ud % divisor;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
    while (ud /= divisor);

    // Terminate buffer
    *p = 0;

    // Initialize pointers
    p1 = saved;
    p2 = p - 1;
    written += p2-p1+1;

    // Reverse buf
    while (p1 < p2)
    {
        tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
    // Update String administration, if needed.
    if (!string)
        m_count = written;

    return written;
}

void String::operator = (const char *s)
{
    Size len = length(s);

    if (reserve(len))
    {
        MemoryBlock::copy(m_string, s, len + 1);
        m_count = len;
        m_string[m_count] = ZERO;
    }
}

void String::operator = (const String & str)
{
    Size len = length(str.m_string);

    if (reserve(len))
    {
        MemoryBlock::copy(m_string, str.m_string, len + 1);
        m_count = len;
        m_string[m_count] = ZERO;
    }
}

bool String::operator == (const String & str) const
{
    return compareTo(str, true) == 0;
}

bool String::operator != (const String & str) const
{
    return compareTo(str, true) != 0;
}

const char * String::operator * () const
{
    return m_string;
}

char * String::operator * ()
{
    return m_string;
}

String & String::operator << (const char *str)
{
    Size len = length(str);

    if (reserve(m_count + len))
    {
        MemoryBlock::copy(m_string + m_count, str, len + 1);
        m_count += len;
        m_string[m_count] = ZERO;
    }
    return (*this);
}

String & String::operator << (const String & str)
{
    this->operator << (str.m_string);
    return (*this);
}

String & String::operator << (int number)
{
    if (reserve(m_count + 16))
        m_count += set(number, m_base, m_string + m_count);

    return (*this);
}

String & String::operator << (unsigned int number)
{
    if (reserve(m_count + 16))
        m_count += setUnsigned(number, m_base, m_string + m_count);

    return (*this);
}

String & String::operator << (void *ptr)
{
    if (reserve(m_count + 16))
        m_count += setUnsigned((unsigned long) ptr, Number::Hex, m_string + m_count);

    return (*this);
}

String & String::operator << (Number::Base base)
{
    m_base = base;
    return (*this);
}
