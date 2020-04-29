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

#include "ArgumentParser.h"
#include "ConstHashIterator.h"

ArgumentParser::ArgumentParser()
{
    m_name = "PROGNAME";
    m_description = "program description";
}

ArgumentParser::~ArgumentParser()
{
    // cleanup flags
    for (HashIterator<String, Argument *> it(m_flags);
         it.hasCurrent();)
    {
        delete it.current();
        it.remove();
    }

    // cleanup positionals
    for (Size i = 0; i < m_positionals.count(); i++)
        delete m_positionals[i];
}

String ArgumentParser::getUsage() const
{
    String usage;

    // Build the syntax line
    usage << "usage: " << *m_name << " [OPTIONS] ";

    // Append positional arguments to syntax line
    for (Size i = 0; i < m_positionals.count(); i++)
    {
        if (m_positionals[i]->getCount() == 0)
            usage << "[" << m_positionals[i]->getName() << "..] ";
        else
            usage << m_positionals[i]->getName() << " ";
    }
    // Append description
    usage << "\r\n\r\n" << *m_description << "\r\n";

    if (m_positionals.count() > 0)
        usage << "\r\n  Positional Arguments:\r\n\r\n";

    // Make list of positional arguments
    for (Size i = 0; i < m_positionals.count(); i++)
    {
        (usage << "   " << m_positionals[i]->getName()).pad(16) <<
                  "   " << m_positionals[i]->getDescription()   << "\r\n";
    }

    // Make list of flag arguments
    if (m_flags.count() > 0)
        usage << "\r\n  Optional Arguments:\r\n\r\n";

    for (ConstHashIterator<String, Argument *> it(m_flags);
         it.hasCurrent(); it++)
    {
        char tmp[2] = { it.current()->getIdentifier(), 0 };

        (usage << "   -" << tmp << ", --"  << it.current()->getName()).pad(16) <<
                    "   " << it.current()->getDescription()   << "\r\n";
    }
    return usage;
}

const String & ArgumentParser::name() const
{
    return m_name;
}

void ArgumentParser::setName(const char *name)
{
    m_name = name;
}

void ArgumentParser::setDescription(const String & description)
{
    m_description = description;
}

ArgumentParser::Result ArgumentParser::registerFlag(char id,
                                                    const char *name,
                                                    const char *description)
{
    // Insert the flag by its full name
    Argument *arg = new Argument(name);
    arg->setDescription(description);
    arg->setIdentifier(id);
    m_flags.insert(name, arg);

    // Also insert the flag using its single character identifier
    char tmp[2];
    tmp[0] = id;
    tmp[1] = 0;

    m_flagsId.insert(tmp, arg);
    return Success;
}

ArgumentParser::Result ArgumentParser::registerPositional(const char *name,
                                                          const char *description,
                                                          Size count)
{
    // Check that only the last positional can have count zero.
    if (m_positionals.count() &&
        m_positionals.at(m_positionals.count() - 1)->getCount() == 0)
    {
        return AlreadyExists;
    }
    Argument *arg = new Argument(name);
    arg->setDescription(description);
    arg->setCount(count);
    m_positionals.insert(arg);
    return Success;
}

ArgumentParser::Result ArgumentParser::parse(int argc,
                                             char **argv,
                                             ArgumentContainer & output)
{
    Size pos = 0;
    Size i = 0;

    if (argc <= 0)
        return InvalidArgument;

    for (int i = 1; i < argc; i++)
    {
        String str = argv[i];
        List<String> parts = str.split('=');
        String & part1 = parts[0];
        String argname;
        Argument * const *arg;
        Argument *outarg;

        // Is this a flag based argument in full form? (e.g.: --arg)
        if (part1.length() > 1 && part1[0] == '-' && part1[1] == '-')
        {
            argname = part1.substring(2);

            // Flag must exist
            arg = m_flags.get(*argname);
            if (!arg)
            {
                arg = m_flagsId.get(*argname);
                if (!arg)
                    return InvalidArgument;
            }

            outarg = new Argument((*arg)->getName());
            outarg->setValue(*parts.last());
            output.addFlag(outarg);
        }
        // Flag based argument in short form? (e.g.: -a or a list: -abc)
        else if (part1.length() > 1 && part1[0] == '-' && part1[1] != '-')
        {
            // Loop all supplied short form arguments
            for (Size i = 1; i < part1.length(); i++)
            {
                char tmp[2];
                tmp[0] = part1[i];
                tmp[1] = 0;

                // Flag must exist
                arg = m_flags.get(tmp);
                if (!arg)
                {
                    arg = m_flagsId.get(tmp);
                    if (!arg)
                        return InvalidArgument;
                }

                outarg = new Argument((*arg)->getName());
                outarg->setValue(*parts.last());
                output.addFlag(outarg);
            }
        }
        // Positional argument
        else if (m_positionals.count() > 0)
        {
            if (pos > m_positionals.count() - 1)
                return InvalidArgument;

            if (m_positionals[pos]->getCount() == 0)
                outarg = new Argument(*part1);
            else
                outarg = new Argument(*m_positionals[pos++]->getName());

            outarg->setValue(*parts.last());
            output.addPositional(outarg);
        }
        else return InvalidArgument;
    }
    // Check that all required arguments are set
    for (i = 0; i < m_positionals.count(); i++)
    {
        if (m_positionals[i]->getCount() == 0)
            break;
    }
    if (output.getPositionals().count() >= i)
        return Success;
    else
        return NotFound;
}
