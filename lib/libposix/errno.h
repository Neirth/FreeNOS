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

#ifndef __LIBC_ERRNO_H
#define __LIBC_ERRNO_H

#include <Macros.h>
#include <Types.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/**
 * @name Error codes
 *
 * Functions that use errno variable can set the errno value to any of these values.
 *
 * @see errno
 *
 * @{
 */

/** Reports a success operation. */
#define ESUCCESS        0

/** Argument list too long. */
#define E2BIG           -1

/** Permission denied. */
#define EACCES          -2

/** Address in use. */
#define EADDRINUSE      -3

/** Address not available. */
#define EADDRNOTAVAIL   -4

/** Address family not supported. */
#define EAFNOSUPPORT    -5

/** Resource unavailable, try again (may be the same value as [EWOULDBLOCK]). */
#define EAGAIN          -6

/** Connection already in progress. */
#define EALREADY        -7

/** Bad file descriptor. */
#define EBADF           -8

/** Bad message. */
#define EBADMSG         -9

/** Device or resource busy. */
#define EBUSY           -10

/** Operation canceled. */
#define ECANCELED       -11

/** No child processes. */
#define ECHILD          -12

/** Connection aborted. */
#define ECONNABORTED    -13

/** Connection refused. */
#define ECONNREFUSED    -14

/** Connection reset. */
#define ECONNRESET      -15

/** Resource deadlock would occur. */
#define EDEADLK         -16

/** Destination address required. */
#define EDESTADDRREQ    -17

/** Mathematics argument out of domain of function. */
#define EDOM            -18

/** Reserved. */
#define EDQUOT          -19

/** File exists. */
#define EEXIST          -20

/** Bad address. */
#define EFAULT          -21

/** File too large. */
#define EFBIG           -22

/** Host is unreachable. */
#define EHOSTUNREACH    -23

/** Identifier removed. */
#define EIDRM           -24

/** Illegal byte sequence. */
#define EILSEQ          -25

/** Operation in progress. */
#define EINPROGRESS     -26

/** Interrupted function. */
#define EINTR           -27

/** Invalid argument. */
#define EINVAL          -28

/** I/O error. */
#define EIO             -29

/** Socket is connected. */
#define EISCONN         -30

/** Is a directory. */
#define EISDIR          -31

/** Too many levels of symbolic links. */
#define ELOOP           -32

/** File descriptor value too large. */
#define EMFILE          -33

/** Too many links. */
#define EMLINK          -34

/** Message too large. */
#define EMSGSIZE        -35

/** Reserved. */
#define EMULTIHOP       -36

/** Filename too long. */
#define ENAMETOOLONG    -37

/** Network is down. */
#define ENETDOWN        -38

/** Connection aborted by network. */
#define ENETRESET       -39

/** Network unreachable. */
#define ENETUNREACH     -40

/** Too many files open in system. */
#define ENFILE          -41

/** No buffer space available. */
#define ENOBUFS         -42

/** No message is available on the STREAM head read queue. */
#define ENODATA         -43

/** No such device. */
#define ENODEV          -44

/** No such file or directory. */
#define ENOENT          -45

/** Executable file format error. */
#define ENOEXEC         -46

/** No locks available. */
#define ENOLCK          -47

/** Reserved. */
#define ENOLINK         -48

/** Not enough space. */
#define ENOMEM          -49

/** No message of the desired type. */
#define ENOMSG          -50

/** Protocol not available. */
#define ENOPROTOOPT     -51

/** No space left on device. */
#define ENOSPC          -52

/** No STREAM resources. */
#define ENOSR           -53

/** Not a STREAM. */
#define ENOSTR          -54

/** Function not supported. */
#define ENOSYS          -55

/** The socket is not connected. */
#define ENOTCONN        -56

/** Not a directory. */
#define ENOTDIR         -57

/** Directory not empty. */
#define ENOTEMPTY       -58

/** State not recoverable. */
#define ENOTRECOVERABLE -59

/** Not a socket. */
#define ENOTSOCK        -60

/** Not supported (may be the same value as [EOPNOTSUPP]). */
#define ENOTSUP         -61

/** Inappropriate I/O control operation. */
#define ENOTTY          -62

/** No such device or address. */
#define ENXIO           -63

/** Operation not supported on socket (may be the same value as [ENOTSUP]). */
#define EOPNOTSUPP      -64

/** Value too large to be stored in data type. */
#define EOVERFLOW       -65

/** Previous owner died. */
#define EOWNERDEAD      -66

/** Operation not permitted. */
#define EPERM           -67

/** Broken pipe. */
#define EPIPE           -68

/** Protocol error. */
#define EPROTO          -69

/** Protocol not supported. */
#define EPROTONOSUPPORT -70

/** Protocol wrong type for socket. */
#define EPROTOTYPE      -71

/** Result too large. */
#define ERANGE          -72

/** Read-only file system. */
#define EROFS           -73

/** Invalid seek. */
#define ESPIPE          -74

/** No such process. */
#define ESRCH           -75

/** Reserved. */
#define ESTALE          -76

/** Stream ioctl() timeout. */
#define ETIME           -77

/** Connection timed out. */
#define ETIMEDOUT       -78

/** Text file busy. */
#define ETXTBSY         -79

/** Operation would block (may be the same value as [EAGAIN]). */
#define EWOULDBLOCK     -80

/** Cross-device link. */
#define EXDEV           -81

/** Triggers a restart of all filesystem requests */
#define ERESTART        (1 << 17)

/** Last defined error value (do not use). */
#define ELAST           ERESTART

/**
 * @}
 */

/**
 * The lvalue errno is used by many functions to return error values.
 */
extern C int errno;

/**
 * Contains a array of character strings, representing errno values.
 */
extern C char *error_map[];

/**
 * @}
 * @}
 */

#endif /* __LIBC_ERRNO_H */
