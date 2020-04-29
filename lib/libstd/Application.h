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

#ifndef __LIBSTD_APPLICATION_H
#define __LIBSTD_APPLICATION_H

#include "Types.h"
#include "Macros.h"
#include "Log.h"
#include "ArgumentParser.h"
#include "ArgumentContainer.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libstd
 * @{
 */

/**
 * Generic application
 */
class Application
{
  private:

    /** Exit code for successful termination */
    static const uint ExitSuccess = 0;

    /** Exit code for failure termination */
    static const uint ExitFailure = 1;

  public:

    /**
     * Result codes.
     */
    enum Result
    {
        Success,
        NotFound,
        IOError,
        InvalidArgument,
        ShowUsage
    };

  public:

    /**
     * Class constructor.
     *
     * @param argc Argument count
     * @param argv Argument values
     */
    Application(int argc, char **argv);

    /**
     * Class destructor.
     */
    virtual ~Application();

    /**
     * Run the application
     *
     * @return Exit code
     */
    virtual int run();

  protected:

    /**
     * Initialize the application.
     *
     * @return Result code
     */
    virtual Result initialize();

    /**
     * Execute the application event loop.
     *
     * @return Result code
     */
    virtual Result exec() = 0;

    /**
     * Print text to output.
     *
     * @param string Text to print to program output.
     * @return Result code.
     */
    virtual Result output(const char *string) const = 0;

    /**
     * Print string to output.
     *
     * @param string Reference to string to output.
     *
     * @return Result code.
     */
    virtual Result output(String & string) const;

  protected:

    /**
     * Get program arguments parser.
     *
     * @return Program arguments parser.
     */
    ArgumentParser & parser();

    /**
     * Get constant program arguments parser.
     *
     * @return Program arguments parser.
     */
    const ArgumentParser & parser() const;

    /**
     * Get program arguments.
     */
    const ArgumentContainer & arguments() const;

    /**
     * Set program version.
     *
     * @param version Program version string
     */
    void setVersion(const String & version);

  private:

    /**
     * Print usage and terminate.
     */
    void usage() const;

  private:

    /** Program argument parser object */
    ArgumentParser m_parser;

    /** Parsed Arguments */
    ArgumentContainer m_arguments;

    /** Input argument count */
    int m_argc;

    /** Input argument values */
    char ** m_argv;

    /** Program version */
    String m_version;
};

/**
 * @}
 * @}
 */

#endif /* __LIBSTD_APPLICATION_H */
