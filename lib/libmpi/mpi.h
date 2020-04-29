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

#ifndef __LIBMPI_MPI_H
#define __LIBMPI_MPI_H

#include <Types.h>
#include <Macros.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libmpi
 * @{
 */

/**
 * @brief Defined Constants
 * @{
 */

/** Communicator identifier */
typedef uint MPI_Comm;

/** Status holder */
typedef uint MPI_Status;

/**
 * Named Predefined Datatypes
 */
typedef enum
{
    MPI_CHAR,
    MPI_SHORT,
    MPI_LONG,
    MPI_INT,
    MPI_UNSIGNED_CHAR,
    MPI_UNSIGNED_SHORT,
    MPI_UNSIGNED,
    MPI_UNSIGNED_LONG
}
MPI_Datatype;

/**
 * Reserved communicators.
 */
enum
{
    MPI_COMM_WORLD,
    MPI_COMM_SELF
};

/**
 * MPI Error Codes.
 */
enum
{
    MPI_SUCCESS,
    MPI_ERR_BUFFER,
    MPI_ERR_COUNT,
    MPI_ERR_TYPE,
    MPI_ERR_TAG,
    MPI_ERR_COMM,
    MPI_ERR_RANK,
    MPI_ERR_REQUEST,
    MPI_ERR_ROOT,
    MPI_ERR_GROUP,
    MPI_ERR_OP,
    MPI_ERR_TOPOLOGY,
    MPI_ERR_DIMS,
    MPI_ERR_ARG,
    MPI_ERR_UNKNOWN,
    MPI_ERR_TRUNCATE,
    MPI_ERR_OTHER,
    MPI_ERR_INTERN,
    MPI_ERR_PENDING,
    MPI_ERR_IN_STATUS,
    MPI_ERR_ACCESS,
    MPI_ERR_AMODE,
    MPI_ERR_ASSERT,
    MPI_ERR_BAD_FILE,
    MPI_ERR_BASE,
    MPI_ERR_CONVERSION,
    MPI_ERR_DISP,
    MPI_ERR_DUP_DATAREP,
    MPI_ERR_FILE_EXISTS,
    MPI_ERR_FILE_IN_USE,
    MPI_ERR_FILE,
    MPI_ERR_INFO_KEY,
    MPI_ERR_INFO_NOKEY,
    MPI_ERR_INFO_VALUE,
    MPI_ERR_INFO,
    MPI_ERR_IO,
    MPI_ERR_KEYVAL,
    MPI_ERR_LOCKTYPE,
    MPI_ERR_NAME,
    MPI_ERR_NO_MEM,
    MPI_ERR_NOT_SAME,
    MPI_ERR_NO_SPACE,
    MPI_ERR_NO_SUCH_FILE,
    MPI_ERR_PORT,
    MPI_ERR_QUOTA,
    MPI_ERR_READ_ONLY,
    MPI_ERR_RMA_ATTACH,
    MPI_ERR_RMA_CONFLICT,
    MPI_ERR_RMA_RANGE,
    MPI_ERR_RMA_SHARED,
    MPI_ERR_RMA_SYNC,
    MPI_ERR_RMA_FLAVOR,
    MPI_ERR_SERVICE,
    MPI_ERR_SIZE,
    MPI_ERR_SPAWN,
    MPI_ERR_UNSUPPORTED_DATAREP,
    MPI_ERR_UNSUPPORTED_OPERATION,
    MPI_ERR_WIN,
    MPI_T_ERR_CANNOT_INIT,
    MPI_T_ERR_NOT_INITIALIZED,
    MPI_T_ERR_MEMORY,
    MPI_T_ERR_INVALID,
    MPI_T_ERR_INVALID_INDEX,
    MPI_T_ERR_INVALID_ITEM,
    MPI_T_ERR_INVALID_SESSION,
    MPI_T_ERR_INVALID_HANDLE,
    MPI_T_ERR_INVALID_NAME,
    MPI_T_ERR_OUT_OF_HANDLES,
    MPI_T_ERR_OUT_OF_SESSIONS,
    MPI_T_ERR_CVAR_SET_NOT_NOW,
    MPI_T_ERR_CVAR_SET_NEVER,
    MPI_T_ERR_PVAR_NO_WRITE,
    MPI_T_ERR_PVAR_NO_STARTSTOP,
    MPI_T_ERR_PVAR_NO_ATOMIC,
    MPI_ERR_LASTCODE
};

/**
 * @}
 */

/**
 * @brief Point-to-Point Communication
 * @{
 */

extern C int MPI_Send(const void *buf,
                      int count,
                      MPI_Datatype datatype,
                      int dest,
                      int tag,
                      MPI_Comm comm);

extern C int MPI_Recv(void *buf,
                      int count,
                      MPI_Datatype datatype,
                      int source,
                      int tag,
                      MPI_Comm comm,
                      MPI_Status *status);

/**
 * @}
 */

/**
 * @brief Communicator Contexts
 * @{
 */

extern C int MPI_Comm_rank(MPI_Comm comm,
                           int *rank);

extern C int MPI_Comm_size(MPI_Comm comm,
                           int *size);

/**
 * @}
 */

/**
 * @brief Environmental Management
 * @{
 */

extern C int MPI_Init(int *argc, char ***argv);

extern C int MPI_Finalize(void);

extern C int MPI_Get_processor_name(char *name, int *resultlen);

extern C int MPI_Get_version(int *version, int *subversion);

extern C int MPI_Get_library_version(char *version, int *resultlen);

/**
 * @}
 */

/**
 * @}
 * @}
 */

#endif /* __LIBMPI_MPI_H */
