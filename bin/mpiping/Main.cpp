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

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int id, buf, cores;
    const int pong = 0xaabbccdd;
    MPI_Status status;

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
    {
        printf("%s: failed to initialize MPI\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (MPI_Comm_rank(MPI_COMM_WORLD, &id) != MPI_SUCCESS)
    {
        printf("%s: failed to lookup MPI rank\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (id == 0)
    {
        if (MPI_Comm_size(MPI_COMM_WORLD, &cores) != MPI_SUCCESS)
        {
            printf("%s: failed to lookup MPI core count\n", argv[0]);
            return EXIT_FAILURE;
        }
        printf("%s: ping %d cores\n", argv[0], cores - 1);

        for (int i = 1; i < cores; i++)
        {
            if (MPI_Recv(&buf, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status) != MPI_SUCCESS)
            {
                printf("%s: failed to receive message from core%d\n",
                        argv[0], i);
                return EXIT_FAILURE;
            }
            if (buf == pong)
                printf("%s: pong from core%d\n", argv[0], i);
            else
                printf("%s: invalid message %d from core%d\n", argv[0], buf, i);
        }
    }
    else
    {
        buf = pong;

        if (MPI_Send(&buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD) != MPI_SUCCESS)
        {
            printf("%s: failed to send message to core0\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (MPI_Finalize() != MPI_SUCCESS)
    {
        printf("%s: failed to finalize MPI\n", argv[0]);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
