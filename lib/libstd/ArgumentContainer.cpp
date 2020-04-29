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

#include "ArgumentContainer.h"

ArgumentContainer::ArgumentContainer()
{
}

ArgumentContainer::~ArgumentContainer()
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

const Vector<Argument *> & ArgumentContainer::getPositionals() const
{
    return m_positionals;
}

const HashTable<String, Argument *> & ArgumentContainer::getFlags() const
{
    return m_flags;
}

const char * ArgumentContainer::get(const char *name) const
{
    // Search flag arguments
    const Argument * const *arg = m_flags.get(name);
    if (arg)
        return *(*arg)->getValue();

    // Search positional arguments
    for (Size i = 0; i < m_positionals.count(); i++)
    {
        const Argument *a = m_positionals[i];

        if (a->getName().equals(name))
            return *a->getValue();
    }
    return ZERO;
}

ArgumentContainer::Result ArgumentContainer::addPositional(Argument *arg)
{
    m_positionals.insert(arg);
    return Success;
}

ArgumentContainer::Result ArgumentContainer::addFlag(Argument *arg)
{
    m_flags.insert(arg->getName(), arg);
    return Success;
}
