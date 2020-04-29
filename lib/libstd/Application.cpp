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

#include "Application.h"

Application::Application(int argc, char **argv)
{
    m_argc = argc;
    m_argv = argv;
    m_parser.registerFlag('h', "help", "show program help");
    m_parser.registerFlag('v', "version", "show program version");
    m_parser.registerFlag('d', "debug", "set log level to debug");
}

Application::~Application()
{
}

Application::Result Application::initialize()
{
    return Success;
}

int Application::run()
{
    // Set application name
    if (m_argc < 1)
    {
        usage();
        return ExitFailure;
    }
    else
        m_parser.setName(m_argv[0]);

    // Parse commandline arguments
    if (m_parser.parse(m_argc, m_argv, m_arguments) != ArgumentParser::Success)
    {
        usage();
        return ExitFailure;
    }

    // If the help argument is given, show the usage
    if (m_arguments.get("help"))
    {
        usage();
        return ExitFailure;
    }

    // If the version argument is given, show version
    if (m_arguments.get("version"))
    {
        output(m_version);
        output("\n");
        return ExitSuccess;
    }

    // Enable debug logging if specified
    if (m_arguments.get("debug") && Log::instance)
        Log::instance->setMinimumLogLevel(Log::Debug);

    // Initialize the application first
    Result r = initialize();
    if (r == ShowUsage)
        usage();
    if (r != Success)
        return ExitFailure;

    // Run the application
    if (exec() == Success)
        return ExitSuccess;
    else
        return ExitFailure;
}

void Application::usage() const
{
    String s = m_parser.getUsage();
    output(s);
}

Application::Result Application::output(String & string) const
{
    return output(*string);
}

ArgumentParser & Application::parser()
{
    return m_parser;
}

const ArgumentParser & Application::parser() const
{
    return m_parser;
}

const ArgumentContainer & Application::arguments() const
{
    return m_arguments;
}

void Application::setVersion(const String & version)
{
    m_version = version;
}
