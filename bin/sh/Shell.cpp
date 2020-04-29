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

#include <TerminalCodes.h>
#include "Shell.h"
#include "ChangeDirCommand.h"
#include "ExitCommand.h"
#include "StdioCommand.h"
#include "WriteCommand.h"
#include "HelpCommand.h"
#include "TimeCommand.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <Runtime.h>

/** Maximum number of supported command arguments. */
#define MAX_ARGV 16

Shell::Shell(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("System command shell interpreter");
    parser().registerPositional("FILE", "File(s) containing shell commands to execute", 0);

    registerCommand(new ChangeDirCommand());
    registerCommand(new ExitCommand());
    registerCommand(new StdioCommand());
    registerCommand(new WriteCommand());
    registerCommand(new HelpCommand(this));
    registerCommand(new TimeCommand());
}

Shell::~Shell()
{
}

Shell::Result Shell::exec()
{
    const Vector<Argument *> & positionals = arguments().getPositionals();
    FILE *fp;
    struct stat st;
    char *contents;

    // Refresh mount points
    refreshMounts(0);

    // Check if shell script was given as argument
    if (positionals.count() > 0)
    {
        // Execute commands in each file
        for (Size i = 0; i < positionals.count(); i++)
        {
            const char *file = *(positionals[i]->getValue());

            // Query the file size
            if (stat(file, &st) != 0)
            {
                ERROR("failed to stat() `" << file << "': " << strerror(errno));
                continue;
            }

            // Open file
            if ((fp = fopen(file, "r")) == NULL)
            {
                ERROR("failed to fopen() `" << file << "': " << strerror(errno));
                continue;
            }

            // Allocate buffer storage
            contents = new char[st.st_size + 1];

            // Read the entire file into memory
            if (fread(contents, st.st_size, 1, fp) != (size_t) st.st_size)
            {
                ERROR("failed to fread() `" << file << "': " << strerror(errno));
                fclose(fp);
                continue;
            }
            // Null terminate
            contents[st.st_size] = 0;

            // Parse it into lines
            String contentString(contents);
            List<String> lines = contentString.split('\n');
    
            // Execute each command
            for (ListIterator<String> i(lines); i.hasCurrent(); i++)
            {
                executeInput((char *) *i.current());
            }

            // Cleanup
            delete contents;
            fclose(fp);
        }
    }
    // Run an interactive Shell
    else
    {
        // Show the user where to get help
        printf( "\r\n"
                "Entering interactive Shell. Type 'help' for the command list.\r\n"
                "\r\n");

        // Begin loop
        return runInteractive();
    }

    // Done
    return Success;
}

Shell::Result Shell::runInteractive()
{
    char *cmdStr;

    // Read commands
    while (true)
    {
        // Print the prompt
        prompt();
        
        // Wait for a command string
        cmdStr = getInput();
        
        // Enough input?
        if (strlen(cmdStr) == 0)
        {
            continue;
        }
        // Execute the command
        executeInput(cmdStr);
    }

    // Done (never reached)
    return Success;
}

int Shell::executeInput(char *command)
{
    char *argv[MAX_ARGV];
    char tmp[128];
    ShellCommand *cmd;
    Size argc;
    int pid, status;
    bool background;

    // Valid argument?
    if (!strlen(command))
    {
        return EXIT_SUCCESS;
    }

    DEBUG("command = '" << command << "'");

    // Attempt to extract arguments
    argc = parse(command, argv, MAX_ARGV, &background);

    // Ignore comments
    if (argv[0][0] == '#')
        return EXIT_SUCCESS;

    // Do we have a matching ShellCommand?
    if (!(cmd = getCommand(argv[0])))
    {
        // If not, try to execute it as a file directly
        if ((pid = forkexec(argv[0], (const char **) argv)) != -1)
        {
            if (!background)
            {
                waitpid(pid, &status, 0);
                return status;
            } else
                return EXIT_SUCCESS;
        }

        // Try to find it on the filesystem. (temporary hardcoded PATH)
        else if (snprintf(tmp, sizeof(tmp), "/bin/%s", argv[0]) &&
                (pid = forkexec(tmp, (const char **) argv)) != -1)
        {
            if (!background)
            {
                waitpid(pid, &status, 0);
                return status;
            } else
                return EXIT_SUCCESS;
        }
        else
        {
            ERROR("forkexec `" << argv[0] << "' failed: " << strerror(errno));
        }
    }
    // Enough arguments given?
    else if (argc - 1 < cmd->getMinimumParams())
    {
        ERROR(cmd->getName() << ": not enough arguments (" << cmd->getMinimumParams() << " required)");
    }
    // Execute it
    else
    {
        return cmd->execute(argc - 1, argv + 1);
    }
    // Not successful
    return EXIT_FAILURE;
}

char * Shell::getInput()
{
    static char line[1024];
    Size total = 0;

    // Read a line
    while (total < sizeof(line) - 1)
    {
        // Read a character
        read(0, line + total, 1);

        // Process character
        switch (line[total])
        {
            // Enter
            case '\r':
            case '\n':
                printf("\r\n");
                line[total] = ZERO;
                return line;

            // Backspace
            case '\b':
            case 0x7F:
                if (total > 0)
                {
                    total--;
                    printf("\b \b");
                }
                break;

            default:
                printf("%c", line[total]);
                total++;
                break;
        }
    }
    line[total] = ZERO;
    return line;
}

void Shell::prompt()
{
    char host[128], cwd[128];
    
    // Retrieve current hostname
    gethostname(host, sizeof(host));
    
    // Retrieve current working directory
    getcwd(cwd, sizeof(cwd));
    
    // Print out the prompt
    printf(WHITE "(" GREEN "%s" WHITE ") " BLUE "%s" WHITE " # ",
           host, cwd);
}

HashTable<String, ShellCommand *> & Shell::getCommands()
{
    return m_commands;
}

ShellCommand * Shell::getCommand(const char *name)
{
    return m_commands.get(name) ? *m_commands.get(name) : ZERO;
}

void Shell::registerCommand(ShellCommand *command)
{
    m_commands.insert(command->getName(), command);
}

Size Shell::parse(char *cmdline, char **argv, Size maxArgv, bool *background)
{
    Size argc;

    *background = false;
    
    for (argc = 0; argc < maxArgv && *cmdline; argc++)
    {
        while (*cmdline && *cmdline == ' ')
            cmdline++;

        if (*cmdline == '&')
        {
            *background = true;
            break;
        }

        argv[argc] = cmdline;

        while (*cmdline && *cmdline != ' ')
            cmdline++;

        if (*cmdline) *cmdline++ = ZERO;
    }
    argv[argc] = ZERO;
    return argc;
}
