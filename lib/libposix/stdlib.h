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

#ifndef __LIBC_STDLIB_H
#define __LIBC_STDLIB_H

#include <Macros.h>
#include <sys/types.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/** Successful termination. */
#define EXIT_SUCCESS 0

/** Unsuccessful termination. */
#define EXIT_FAILURE 1

/**
 * @brief Terminate a process.
 *
 * The exit function causes normal program termination to occur.
 * If more than one call to the exit function is executed by a program,
 * the behaviour is undefined.
 *
 * @param status If the value of status is zero or EXIT_SUCCESS, an
 *               implementation defined form of the status successful
 *               termination is returned. If the value of status is
 *               EXIT_FAILURE, an implementation-defined form of the status
 *               unsuccesful termination is returned. Otherwise the status
 *               is implementation-defined.
 * @return The exit function cannot return to its caller.
 */
extern C void exit(int status);

/**
 * @brief Convert a number to a string.
 *
 * @param buffer String buffer to write to.
 * @param divisor Base of the number, e.g. 16 for hexadecimal.
 * @param number The number to convert.
 */
extern C void itoa(char *buffer, int divisor, int number);

/**
 * @brief Convert a string to an integer.
 *
 * The atoi, atol, and atoll functions convert the initial portion of the string
 * pointed to by nptr to int, long int, and long long int representation,
 * respectively.
 *
 * @param nptr String pointer to convert.
 *
 * @return The atoi, atol, and atoll functions return the converted value.
 */
extern C int atoi(const char *nptr);

/**
 * @brief Convert a string to a long integer
 *
 * These functions shall convert the initial portion of the string pointed to
 * by str to a type long and long long representation, respectively. First,
 * they decompose the input string into three parts:
 *
 * @param nptr Input string pointer to convert.
 * @param endptr If the subject sequence is empty or does not have the
 *               expected form, no conversion is performed; the value of
 *               str is stored in the object pointed to by endptr, provided
 *               that endptr is not a null pointer.
 * @param base Base of the number, e.g. 16 for hexadecimal.
 *
 * @return Upon successful completion, these functions shall return the
 *         converted value, if any. If no conversion could be performed,
 *         0 shall be returned and errno may be set to [EINVAL]. If the
 *         correct value is outside the range of representable values,
 *         {LONG_MIN}, {LONG_MAX}, {LLONG_MIN}, or {LLONG_MAX} shall be
 *         returned (according to the sign of the value), and errno set
 *         to [ERANGE].
 */
extern C long strtol(const char *nptr, char **endptr, int base);

/**
 * @brief A memory allocator
 *
 * The malloc() function shall allocate unused space for an object whose
 * size in bytes is specified by size and whose value is unspecified.
 *
 * @param size Number of bytes to allocate.
 *
 * @return Upon successful completion with size not equal to 0, malloc()
 *         shall return a pointer to the allocated space. If size is 0,
 *         either a null pointer or a unique pointer that can be
 *         successfully passed to free() shall be returned. Otherwise,
 *         it shall return a null pointer [CX] [Option Start]  and set
 *         errno to indicate the error.
 */
extern C void * malloc(size_t size);

/**
 * @brief Free allocated memory
 *
 * The free() function shall cause the space pointed to by ptr to be
 * deallocated; that is, made available for further allocation. If ptr
 * is a null pointer, no action shall occur. Otherwise, if the argument
 * does not match a pointer earlier returned by a function in POSIX.1-2008
 * that allocates memory as if by malloc(), or if the space has been
 * deallocated by a call to free() or realloc(), the behavior is undefined.
 * Any use of a pointer that refers to freed space results in undefined behavior.
 *
 * @param ptr Previously allocated memory to free.
 *
 * @return The free() function shall not return a value.
 */
extern C void free(void *ptr);

/**
 * @brief Random number generator
 *
 * The srandom() function sets its argument as the seed for a new sequence of
 * pseudo-random integers to be returned by random(). These sequences are
 * repeatable by calling srandom() with the same seed value. If no seed value
 * is provided, the random() function is automatically seeded with a value of 1.
 *
 * @param seed Initial seed value for the randomizer
 */
extern C void srandom(unsigned int seed);

/**
 * @brief Random number generator
 *
 * @return Successive pseudo-random numbers in the range from 0 to RAND_MAX.
 */
extern C long int random(void);

/**
 * @}
 * @}
 */

#endif /* __LIBC_STDLIB_H */
