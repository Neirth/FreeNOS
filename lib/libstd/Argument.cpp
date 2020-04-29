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

#include "Argument.h"

Argument::Argument(const char *name)
    : m_id(0)
    , m_name(name)
    , m_count(0)
{
}

Argument::Argument(const String & name)
    : m_id(0)
    , m_name(name)
    , m_count(0)
{
}

char Argument::getIdentifier() const
{
    return m_id;
}

Size Argument::getCount() const
{
    return m_count;
}

const String & Argument::getName() const
{
    return m_name;
}

const String & Argument::getDescription() const
{
    return m_description;
}

const String & Argument::getValue() const
{
    return m_value;
}

void Argument::setIdentifier(char id)
{
    m_id = id;
}

void Argument::setName(const char *name)
{
    m_name = name;
}

void Argument::setDescription(const char *description)
{
    m_description = description;
}

void Argument::setValue(const char *value)
{
    m_value = value;
}

void Argument::setCount(Size count)
{
    m_count = count;
}

bool Argument::operator == (const Argument & arg) const
{
    return arg.m_id == m_id;
}

bool Argument::operator != (const Argument & arg) const
{
    return arg.m_id != m_id;
}
