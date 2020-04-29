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
#include "mpi.h"

extern Size coreCount;

int MPI_Comm_rank(MPI_Comm comm,
                  int *rank)
{
    SystemInformation info;
    *rank = info.coreId;
    return MPI_SUCCESS;
}

int MPI_Comm_size(MPI_Comm comm,
                  int *size)
{
    *size = coreCount;
    return MPI_SUCCESS;
}
