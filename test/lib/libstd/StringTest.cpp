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

#include <TestCase.h>
#include <TestRunner.h>
#include <TestChar.h>
#include <TestMain.h>
#include <String.h>

TestCase(StringConstructEmpty)
{
    String s;

    // The string should allocated, empty, default sized.
    testString(s.m_string, "");
    testAssert(s.m_size == STRING_DEFAULT_SIZE);
    testAssert(s.m_count == 0);
    testAssert(s.m_allocated);
    testAssert(s.m_base == Number::Dec);
    return OK;
}

TestCase(StringConstructAlloc)
{
    String s("Test data", true);

    // The String should be allocated
    testString(s.m_string, "Test data");
    testAssert(s.m_allocated);
    testAssert(s.m_size == 10);
    testAssert(s.m_count == 9);
    testAssert(s.m_base == Number::Dec);
    return OK;
}

TestCase(StringConstructConstant)
{
    String s = "Test string";

    // The String should not be allocated
    testString(s.m_string, "Test string");
    testAssert(!s.m_allocated);
    testAssert(s.m_size == 12);
    testAssert(s.m_count == 11);
    testAssert(s.m_base == Number::Dec);
    return OK;
}

TestCase(StringConstructRandom)
{
    TestChar<char *> strings(5, 50);
    String s = strings.random();

    // The String should be allocated
    testString(s.m_string, strings.get(0));
    testAssert(s.m_allocated);
    testAssert(s.m_count == String::length(strings.get(0)));
    testAssert(s.m_size == s.m_count + 1);
    testAssert(s.m_base == Number::Dec);
    return OK;
}

TestCase(StringConstructCopy)
{
    String s1 = "Hello";
    String s2(s1);

    // The String should be copied.
    testString(s2.m_string, s1.m_string);
    testString(s2.m_string, "Hello");
    testAssert(s2.m_allocated);
    testAssert(s2.m_size == 6);
    testAssert(s2.m_count == 5);
    testAssert(s2.m_base == Number::Dec);
    return OK;
}

TestCase(StringConstructInt)
{
    String s = 123456;

    // The String should match the integer in text.
    testString(s.m_string, "123456");
    testAssert(s.m_allocated);
    testAssert(s.m_size == STRING_DEFAULT_SIZE);
    testAssert(s.m_count == 6);
    testAssert(s.m_base == Number::Dec);
    return OK;
}

TestCase(StringLength)
{
    TestChar<char *> strings(5, 50);
    String s = strings.random();

    // Check the length functions and members
    testAssert(s.length() == strings.length(0));
    testAssert(String::length(strings[0]) == strings.length(0));
    testAssert(s.m_count == strings.length(0));
    testAssert(s.m_size == s.m_count + 1);
    return OK;
}

TestCase(StringResize)
{
    String s = "test";

    // Check initial String
    testString(s.m_string, "test");
    testAssert(s.m_count == 4);
    testAssert(s.m_size == 5);

    // Resize the string
    s.resize(100);

    // Check the resized String
    testString(s.m_string, "test");
    testAssert(s.m_allocated);
    testAssert(s.m_count == 4);
    testAssert(s.m_size == 100);
    return OK;
}

TestCase(StringResizeChop)
{
    String s = "1234567890";

    // Check the initial String
    testString(s.m_string, "1234567890");
    testAssert(s.m_count == 10);
    testAssert(s.m_size == 11);

    // Resize the string
    s.resize(5);

    // Check the resized String
    testString(s.m_string, "1234");
    testAssert(s.m_count == 4);
    testAssert(s.m_size == 5);
    testAssert(s.m_allocated);
    testAssert(s.m_base == Number::Dec);
    return OK;
}

TestCase(StringResizeAssign)
{
    TestChar<char *> strings(128, 256);
    String s = "test";

    // Check the initial length
    testAssert(s.m_count == 4);
    testAssert(s.m_size == s.m_count + 1);

    // Re-assign the string with a large string
    s = strings.random();

    // Check the string has been resized
    testString(s.m_string, strings.get(0));
    testAssert(s.m_allocated);
    testAssert(s.m_count == strings.length(0));
    testAssert(s.m_size == s.m_count + 1);
    testAssert(s.length() == strings.length(0));
    testAssert(s.m_base == Number::Dec);
    return OK;
}

TestCase(StringRead)
{
    TestChar<char *> strings(128, 256);
    String s = strings.random();

    // Check get(), at() and value() functions.
    for (Size i = 0; i < strings.length(0); i++)
    {
        testAssert(s.m_string[i] == strings.get(0)[i]);
        testAssert(*s.get(i) == strings.get(0)[i]);
        testAssert(s.at(i) == strings.get(0)[i]);
        testAssert(s.value(i) == strings.get(0)[i]);
        testAssert(s[i] == strings.get(0)[i]);
    }
    return OK;
}

TestCase(StringContains)
{
    String s = "test";

    // All characters inside the Strings itself should match contains()
    for (Size i = 0; i < s.m_count; i++)
        testAssert(s.contains(s.m_string[i]));

    // Non-existing chars should return false
    testAssert(!s.contains('z'));
    return OK;
}

TestCase(StringStartsWith)
{
    String s = "testing1234";

    // Check matching prefixes.
    for (Size i = 1; i <= s.m_count; i++)
    {
        char buf[s.m_count + 1];
        MemoryBlock::copy(buf, s.m_string, i+1);
        buf[i+1] = ZERO;

        testAssert(s.startsWith(buf));
    }

    // Match against itself
    testAssert(s.startsWith(s));

    // Check for unmatching prefixes
    testAssert(!s.startsWith(""));
    testAssert(!s.startsWith("tesT"));
    testAssert(!s.startsWith("esting123"));
    testAssert(!s.startsWith("testing12345"));

    return OK;
}

TestCase(StringEndsWith)
{
    String s = "testing1234";

    // Check matching prefixes.
    Size i = s.m_count - 1;
    while (1)
    {
        char buf[s.m_count + 1];
        MemoryBlock::copy(buf, s.m_string + i, s.m_count - i + 1);
        buf[s.m_count - i] = ZERO;

        testAssert(s.endsWith(buf));

        if (i == 0)
            break;
        else
            i--;
    }

    // Match against itself
    testAssert(s.endsWith(s));

    // Check for unmatching prefixes
    testAssert(!s.endsWith(""));
    testAssert(!s.endsWith("1235"));
    testAssert(!s.endsWith("esting123"));
    testAssert(!s.endsWith("testing12345"));
    return OK;
}

TestCase(StringCompare)
{
    String s = "testing1234";
    String s2 = s;

    // Check compareTo()
    testAssert(s.compareTo(s2, true) == 0);
    testAssert(s.compareTo("testing1234", true) == 0);
    testAssert(s.compareTo("tesTing1234", true) != 0);
    testAssert(s.compareTo("testing123", true) != 0);
    testAssert(s.compareTo("testing12345", true) != 0);

    // Check case sensitivity
    testAssert(s.compareTo("TeStInG1234", false) == 0);
    testAssert(s.compareTo("TeStInG1234", true) != 0);

    // Check the comparison operators != and ==
    testAssert(s == "testing1234");
    testAssert(s != "Testing1234");
    testAssert(s != "ttesting1234");
    return OK;
}

TestCase(StringMatch)
{
    String s = "testing1234";

    // Check matching patterns
    testAssert(s.match("test*"));
    testAssert(s.match("test**"));
    testAssert(s.match("testing123*"));
    testAssert(s.match("t*"));
    testAssert(s.match("*"));
    testAssert(s.match("*1234"));
    testAssert(s.match("*ting*"));
    testAssert(s.match("*esting123*"));
    testAssert(s.match("*testing1234*"));
    testAssert(s.match("*testing1234**"));
    testAssert(s.match("testing1234"));
    testAssert(s.match("*12*4*"));
    testAssert(s.match("*12*34*"));
    testAssert(s.match("*test*1234*"));

    // Check unmatching patterns
    testAssert(!s.match(""));
    testAssert(!s.match("Test*"));
    testAssert(!s.match("*124*"));
    testAssert(!s.match("**ting1235"));
    testAssert(!s.match("testin**1233"));
    testAssert(!s.match("testing12"));
    testAssert(!s.match("testing12345"));
    testAssert(!s.match("*tinng*"));
    testAssert(!s.match("testing12345*"));
    testAssert(!s.match("testing12345**"));
    testAssert(!s.match("*testing12345"));
    testAssert(!s.match("**testing12345"));
    testAssert(!s.match("**testingz**"));
    testAssert(!s.match("Testing1234"));
    testAssert(!s.match("*Ting1234"));
    testAssert(!s.match("*test*test*"));
    testAssert(!s.match("*test*oops*"));
    return OK;
}

TestCase(StringSub)
{
    String s = "testing1234";
    String s1 = s.substring(2);

    // Index only
    testString(s1.m_string, "sting1234");
    testAssert(s1.m_count == 9);
    testAssert(s1.m_size == 10);

    // Index with size
    String s2 = s.substring(3, 4);
    testString(s2.m_string, "ting");
    testAssert(s2.m_count == 4);
    testAssert(s2.m_size == 9);

    // Too large index
    String s3 = s.substring(100);
    testString(s3.m_string, "");
    testAssert(s3.m_count == 0);
    testAssert(s3.m_size == STRING_DEFAULT_SIZE);

    // Too large size
    String s4 = s.substring(3, 100);
    testString(s4.m_string, "ting1234");
    testAssert(s4.m_count == 8);
    testAssert(s4.m_size == 9);
    return OK;
}

TestCase(StringPad)
{
    String s = "hello\nthis";

    // Pad the last line to 10 characters
    s.pad(10);

    testString(s.m_string, "hello\nthis      ");
    testAssert(s.m_count == 16);
    testAssert(s.m_size == 17);
    return OK;
}

TestCase(StringTrim)
{
    String s = " \t  testing1234   \t ";

    // Trim the String
    s.trim();

    testString(s.m_string, "testing1234");
    testAssert(s.m_count == 11);
    testAssert(s.m_size > s.m_count);
    return OK;
}

TestCase(StringUpper)
{
    String s = "testing1234";

    // Uppercase the String
    s.upper();

    testString(s.m_string, "TESTING1234");
    testAssert(s.m_count == 11);
    testAssert(s.m_size == 12);
    return OK;
}

TestCase(StringLower)
{
    String s = "TESTing1234";

    // Lowercase the String
    s.lower();

    testString(s.m_string, "testing1234");
    testAssert(s.m_count == 11);
    testAssert(s.m_size == 12);
    return OK;
}

TestCase(StringSplitChar)
{
    String s = "Test:ing:1234";

    // Split the string by colon character
    List<String> lst = s.split(':');

    // Check the list
    testAssert(lst.m_count == 3);
    testString(lst[0].m_string, "Test");
    testString(lst[1].m_string, "ing");
    testString(lst[2].m_string, "1234");
    return OK;
}

TestCase(StringSplitCharEmpty)
{
    String s = "::Test::ing::::a::::1234:::";

    // Split the string by colon character. The empty pre/post fixes must be ignored.
    List<String> lst = s.split(':');

    // Check the list
    testAssert(lst.m_count == 4);
    testString(lst[0].m_string, "Test");
    testString(lst[1].m_string, "ing");
    testString(lst[2].m_string, "a");
    testString(lst[3].m_string, "1234");
    return OK;
}

TestCase(StringSplitStr)
{
    String s = "<sep>Test<sep>ing<sep>1234<sep>";

    // Split the string by an XML tag
    List<String> lst = s.split("<sep>");

    // Check the list
    testAssert(lst.m_count == 3);
    testString(lst[0].m_string, "Test");
    testString(lst[1].m_string, "ing");
    testString(lst[2].m_string, "1234");
    return OK;
}

TestCase(StringSplitNone)
{
    String s = ":Test:ing:1234:";

    // Split the string by a comma. It should return the whole string
    List<String> lst = s.split(',');

    // Check the list
    testAssert(lst.m_count == 1);
    testString(lst[0].m_string, ":Test:ing:1234:");
    return OK;
}

TestCase(StringToLong)
{
    String s = "12345";
    testAssert(s.toLong() == 12345);

    String s1 = "1234";
    testAssert(s1.toLong(Number::Hex) == 4660);

    String s2 = "test";
    testAssert(s2.toLong() == 0);
    return OK;
}

TestCase(StringSetSigned)
{
    String s;
    char buf[128];

    // Decimal number
    testAssert(s.set(12345) == 5);
    testString(s.m_string, "12345");
    testAssert(s.m_count == 5);
    testAssert(s.m_size == STRING_DEFAULT_SIZE);

    // Hexadecimal number
    testAssert(s.set(12345, Number::Hex) == 6);
    testString(s.m_string, "0x3039");
    testAssert(s.m_count == 6);
    testAssert(s.m_size == STRING_DEFAULT_SIZE);

    // Negative decimal
    testAssert(s.set(-678910) == 7);
    testString(s.m_string, "-678910");
    testAssert(s.m_count == 7);
    testAssert(s.m_size == STRING_DEFAULT_SIZE);

    // Negative hexadecimal
    testAssert(s.set(-0xabcdef, Number::Hex) == 9);
    testString(s.m_string, "-0xabcdef");
    testAssert(s.m_count == 9);
    testAssert(s.m_size == STRING_DEFAULT_SIZE);

    // External buffer
    testAssert(s.setUnsigned(12345, Number::Hex, buf) == 6);
    testString(buf, "0x3039");
    return OK;
}

TestCase(StringSetUnsigned)
{
    String s;
    char buf[128];

    // Decimal number
    testAssert(s.setUnsigned(4294967286U) == 10);
    testString(s.m_string, "4294967286");
    testAssert(s.m_count == 10);
    testAssert(s.m_size == STRING_DEFAULT_SIZE);

    // Hexadecimal number
    testAssert(s.setUnsigned(0xffaabbcc, Number::Hex) == 10);
    testString(s.m_string, "0xffaabbcc");
    testAssert(s.m_count == 10);
    testAssert(s.m_size == STRING_DEFAULT_SIZE);

    // Hexadecimal number, from large unsigned decimal
    testAssert(s.setUnsigned(2147523736U, Number::Hex) == 10);
    testString(s.m_string, "0x80009c98");
    testAssert(s.m_count == 10);
    testAssert(s.m_size == STRING_DEFAULT_SIZE);

    // External buffer
    testAssert(s.setUnsigned(12345, Number::Hex, buf) == 6);
    testString(buf, "0x3039");
    return OK;
}

TestCase(StringShift)
{
    String s;

    // Format using text and numbers
    s << 1234 << " is even " << "and below " << 10000 << ".";
    testString(s.m_string, "1234 is even and below 10000.");
    testAssert(s.m_count == 29);
    testAssert(s.m_size > s.m_count);
    testAssert(s.m_allocated);

    return OK;
}

TestCase(StringShiftNumberBase)
{
    String s;

    // Format using decimal and hexadecimal
    s << 123 << " = " << Number::Hex << 123;
    testString(s.m_string, "123 = 0x7b");
    return OK;
}
