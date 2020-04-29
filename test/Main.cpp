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
#include <libgen.h>
#include <TestRunner.h>
#include <TestReporter.h>
#include <DirectoryScanner.h>

int main(int argc, char **argv)
{
    char path[255], tmp[255];
    int iterations = 1, r;
    DirectoryScanner scanner(argc, argv);
    TestRunner runner(argc, argv);

    // Grab command-line arguments, if any.
    // First positional argument is the directory.
    if (argc > 1)
    {
        strncpy(path, argv[1], sizeof(path));
        path[254] = 0;
    }
    else
    {
        strncpy(tmp, argv[0], sizeof(tmp));
        strncpy(path, dirname(tmp), sizeof(path));
        path[254] = 0;
    }
    // Optional arguments.
    for (int i = 1; i < argc; i++)
    {
        if ((strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--iterations") == 0) &&
             i < argc - 1)
        {
            iterations = atoi(argv[i+1]);
        }
    }
    runner.getReporter()->setMultiline(true);

    // Discover tests
    if (scanner.scan(path) != EXIT_SUCCESS)
    {
        printf("%s: failed to discover tests\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Run tests
    for (int i = 0; i < iterations; i++)
    {
        if ((r = runner.run()) != 0)
            return r;
    }
    // Success
    return EXIT_SUCCESS;
}
