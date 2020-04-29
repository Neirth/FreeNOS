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
#include <errno.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "ExternalTest.h"

ExternalTest::ExternalTest(const char *name, int argc, char **argv)
    : TestInstance(name)
{
    m_argc = argc;
    m_argv = argv;
}

TestResult ExternalTest::run()
{
    int status;
    pid_t pid;
    char **argv = new char * [m_argc + 2];

    for (int i = 1; i < m_argc; i++)
        argv[i] = m_argv[i];

    argv[0]        = *m_name;
    argv[m_argc]   = (char *) "-n";
    argv[m_argc+1] = 0;

#ifdef __HOST__
    if ((pid = fork()) == 0)
        execv(argv[0], argv);
#else
    pid = forkexec(*m_name, (const char **) argv);
#endif
    waitpid(pid, &status, 0);
    delete[] argv;

    return status == 0 ? OK : FAIL;
}
