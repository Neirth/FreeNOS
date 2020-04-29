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

#ifndef __LIBC_STRING_H
#define __LIBC_STRING_H

#include <sys/types.h>
#include "errno.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/**
 * Compare two strings.
 *
 * @param dest Destination string to compare.
 * @param src Source string to compare.
 *
 * @return Zero if equal, non-zero if not.
 */
extern C int strcmp(const char *dest, const char *src);

/**
 * Compare two strings, by only a maximum number of bytes.
 *
 * @param dest Destination string to compare.
 * @param src Source string to compare.
 * @param count Maximum number of bytes to compare.
 *
 * @return Zero if equal, non-zero if not.
 */
extern C int strncmp(const char *dest, const char *src, size_t count);

/**
 * Compare two strings, ignoring case considerations.
 *
 * @param dest Destination string to compare.
 * @param src Source string to compare.
 *
 * @return Zero if equal, non-zero if not.
 */
extern C int strcasecmp(const char *dest, const char *src);

/**
 * Compare two strings, ignoring case considerations.
 *
 * @param dest Destination string to compare.
 * @param src Source string to compare.
 * @param count Maximum number of bytes to compare.
 *
 * @return Zero if equal, non-zero if not.
 */
extern C int strncasecmp(const char *dest, const char *src, size_t count);

/**
 * Duplicate a string
 *
 * @param str String to duplicate
 *
 * @return Duplicated string or NULL if not enough memory is available.
 */
extern C char *strdup(const char *str);

/**
 * @brief Duplicate a specific number of bytes from a string.
 *
 * The strndup() function shall be equivalent to the strdup()
 * function, duplicating the provided s in a new block of memory
 * allocated as if by using malloc(), with the exception being that
 * strndup() copies at most size plus one bytes into the newly allocated
 * memory, terminating the new string with a NUL character. If the length
 * of s is larger than size, only size bytes shall be duplicated. If size
 * is larger than the length of s, all bytes in s shall be copied into the
 * new memory buffer, including the terminating NUL character. The newly
 * created string shall always be properly terminated.
 *
 * @param s String to duplicate.
 * @param size Maximum number of bytes to copy, excluding the NULL byte.
 *
 * @return Upon successful completion, the strndup() function shall return
 *         a pointer to the newly allocated memory containing the duplicated
 *         string. Otherwise, it shall return a null pointer and set errno
 *         to indicate the error.
 */
extern C char *strndup(const char *s, size_t size);

/**
 * Fill memory with a constant byte.
 *
 * @param dest Memory to write to.
 * @param ch Constant byte.
 *
 * @return Pointer to dest.
 */
extern C void * memset(void *dest, int ch, size_t count);

/**
 * Copy memory from one place to another.
 *
 * @param dest Destination address.
 * @param src Source address.
 * @param count Number of bytes to copy.
 *
 * @return The destination address.
 */
extern C void * memcpy(void *dest, const void *src, size_t count);

/**
 * Calculate the length of a string.
 *
 * @param str String to calculate length for.
 *
 * @return Length of the string.
 */
extern C size_t strlen(const char *str);

/**
 * Copy a string.
 *
 * @param dest Destination buffer.
 * @param src Source string.
 *
 * @return Number of bytes copied.
 */
extern C int strcpy(char *dest, const char *src);

/**
 * Copy a string, given a maximum number of bytes.
 *
 * @param dest Destination buffer.
 * @param src Source string.
 * @param sz Maximum number of bytes to copy.
 *
 * @return Number of bytes copied.
 */
extern C int strncpy(char *dest, const char *src, size_t sz);

/**
 * Copy src to string dst of size siz.
 *
 * At most siz-1 characters will be copied.  Always NUL terminates (unless siz == 0).
 *
 * @param dst Destination string
 * @param src Source string
 * @param siz size_t of dst buffer
 *
 * @return strlen(src); if retval >= siz, truncation occurred.
 *
 * @note This function is copied from OpenBSD-4.3
 */
extern C size_t strlcpy(char *dst, const char *src, size_t siz);

/**
 * @brief Concatenate two strings.
 *
 * The strcat() function shall append a copy of the string pointed to
 * by s2 (including the terminating NUL character) to the end of the
 * string pointed to by s1.
 *
 * @param dest Destination string.
 * @param src Source string.
 *
 * @return The strcat() function shall return s1; no return value is
 *         reserved to indicate an error.
 */
extern C char * strcat(char *dest, const char *src);

/**
 * @brief Concatenate a string with part of another
 *
 * The strncat() function shall append not more than n bytes (a NUL
 * character and bytes that follow it are not appended) from the array
 * pointed to by s2 to the end of the string pointed to by s1. The initial
 * byte of s2 overwrites the NUL character at the end of s1. A terminating
 * NUL character is always appended to the result. If copying takes place
 * between objects that overlap, the behavior is undefined.
 *
 * @param dest Destination string.
 * @param src Source string.
 * @param n Number of character to concatenate.
 *
 * @return The strncat() function shall return s1; no return value
 *         shall be reserved to indicate an error.
 */
extern C char * strncat(char *dest, const char *src, size_t siz);

/**
 * The strerror function maps the number in errnum to a message string.
 *
 * @param errnum Error number to convert.
 *
 * @return The strerror function returns a pointer to the string, the contents
 *         of which are locale specific. The array pointed to shall not be modified
 *         by the program, but may be overwritten by a subsequent call to the
 *         strerror function.
 */
extern C char * strerror(int errnum);

/**
 * @brief String scanning operation.
 *
 * The strchr() function shall locate the first occurrence of c
 * (converted to a char) in the string pointed to by s. The terminating
 * NULL character is considered to be part of the string.
 *
 * @param s String to search in.
 * @param c Character to look for.
 * @return Upon completion, strchr() shall return a pointer to the byte,
 *         or a null pointer if the byte was not found.
 */
extern C char * strchr(const char *s, int c);

/**
 * @brief String scanning operation.
 *
 * The strrchr() function shall locate the last occurrence of c
 * (converted to a char) in the string pointed to by s. The terminating
 * NUL character is considered to be part of the string.
 *
 * @param s String to search in.
 * @param c Character to look for.
 *
 * @return Upon successful completion, strrchr() shall return a pointer
 *         to the byte or a null pointer if c does not occur in the string.
 */
extern C char * strrchr(const char *s, int c);

/**
 * @}
 * @}
 */

#endif /* __LIBC_STRING_H */
