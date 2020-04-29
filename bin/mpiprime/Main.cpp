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
#include <string.h>
#include <math.h>
#include <mpi.h>
#include <unistd.h>
#include <String.h>
#include <sys/time.h>

// Start and end
#define PERNODE(id, nids, base, n)  (((n) - (base)) / (nids))
#define START(id, nids, base, n)    ((base) + ((id) * PERNODE(id, nids, base, n)))
#define END(id, nids, base, n)      (START(id, nids, base, n) + PERNODE(id, nids, base, n))

static int rank, total, length;
static char name[64];

void collect(int n, unsigned *map)
{
    int i, j, sqrt_of_n = sqrt(n), z;
    MPI_Status status;
    unsigned *mybuf = (unsigned *) malloc(sizeof(unsigned) * PERNODE(rank, total, sqrt_of_n, n));

    // Every worker sends it's results to the master
    if (rank != 0)
    {
        // Send mybuf to the master
        MPI_Send(&map[START(rank, total, sqrt_of_n, n)], PERNODE(rank, total, sqrt_of_n, n), MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    // The master gathers the parts of the list from every worker
    else
    {
        for (i = 1; i < total; i++)
        {
            // Receive from worker
            MPI_Recv(mybuf, PERNODE(rank, total, sqrt_of_n, n), MPI_INT, i, 0, MPI_COMM_WORLD, &status);

            // Copy inside our buffer
            for (j = START(i, total, sqrt_of_n, n), z = 0; j < END(i, total, sqrt_of_n, n); j++)
            {
                map[j] = mybuf[z++];
            }
        }
    }
    // Cleanup
    free(mybuf);
}

void search_sequential(int k, int n, unsigned *map, int argc, char **argv)
{
    int i, j;

    // Sequential algorithm
    // Next is a prime
    for (i = 2; i < n; i++)
    {
        // Prime number?
        if (map[i])
        {
            // Mask off all multiples
            for (j = i + 1; j < n; j++)
            {
                if (!(j % i))
                    map[j] = 0;
            }
        }
    }
}

void search_parallel(int k, int n, unsigned *map, int argc, char **argv)
{
    struct timeval t1, t2;
    struct timezone tz;
    gettimeofday(&t1, &tz);
    int i, last, sqrt_of_n = sqrt(n);

    // Find all primes below sqrt(n) sequentially
    search_sequential(k, sqrt_of_n, map, argc, argv);

    if (rank == 0)
    {
        gettimeofday(&t2, &tz);
        printf("sequential: ");
        printtimediff(&t1, &t2);
        printf("\n");
        gettimeofday(&t1, &tz);
    }

    // Every worker calculates all primes k .. sqrt(n) sequentially
    // and uses the result to mark it's part of the map, concurrently
    // Note that no communication is needed
    while (k < sqrt_of_n)
    {
        // Prime number?
        if (!map[k])
        {
            k++;
            continue;
        }

        // Mark multiples of k in my range
        i    = START(rank, total, sqrt_of_n, n);
        last = END(rank, total, sqrt_of_n, n);

        while (i < last)
        {
            // Do we need to unmark this number? (no prime)
            if (!(i % k))
                map[i] = 0;

            i++;
        }
        // Look for the next prime
        k++;
    }
    if (rank == 0)
    {
        gettimeofday(&t2, &tz);
        printf("parallel: ");
        printtimediff(&t1, &t2);
        printf("\n");    
    }

    // Collect results of all workers
    gettimeofday(&t1, &tz);
    collect(n, map);

    if (rank == 0)
    {
        gettimeofday(&t2, &tz);
        printf("collect: ");
        printtimediff(&t1, &t2);
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    int n, k, i;
    unsigned *map;
    String output;
    struct timeval t1, t2;
    struct timezone tz;

    // Initialize MPI
    gettimeofday(&t1, &tz);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &total);
    MPI_Get_processor_name(name, &length);
    gettimeofday(&t2, &tz);

    if (rank == 0)
    {
        printf("MPI_Init: ");
        printtimediff(&t1, &t2);
        printf("\n");
    }

    // Check arguments
    if (argc < 2)
    {
        printf("usage: %s <number> [--stdout]\n", argv[0]);
        return EXIT_FAILURE;
    }
    gettimeofday(&t1, &tz);
    n = atoi(argv[1]);

    // Make sure n is divisible by the number of workers
    if ((n % total) != 0)
    {
        n += total;
        n -= (n % total);
    }

    // Initialize map. Clear all entries
    map = (unsigned *) malloc(sizeof(unsigned) * n);
    for (i = 0; i < n; i++)
        map[i] = 1;

    // We start with 2
    k = 2;
    gettimeofday(&t2, &tz);

    if (rank == 0)
    {
        printf("Setup: ");
        printtimediff(&t1, &t2);
        printf("\n");
    }

    // Search for primes until done
    gettimeofday(&t1, &tz);
    search_parallel(k, n, map, argc, argv);
    gettimeofday(&t2, &tz);
    
    printf("Search_parallel: ");
    printtimediff(&t1, &t2);
    printf("\n");

    gettimeofday(&t1, &tz);

    // Only the master reports the results.
    if (rank == 0 && argc >= 3 && strcmp(argv[2], "--stdout") == 0)
    {
        // Print the result
        for (i = 2; i < n; i++)
        {
            if (map[i] == 1)
                output << " " << i;
        }
        output << "\n";
        write(1, *output, output.length());
    }

    // Free resources
    MPI_Finalize();
    free(map);

    if (rank == 0)
    {
        gettimeofday(&t2, &tz);
        printf("Finalize: ");
        printtimediff(&t1, &t2);
        printf("\n");
    }
    return EXIT_SUCCESS;
}
