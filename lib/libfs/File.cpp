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

#include "File.h"

File::File(FileType type, UserID uid, GroupID gid)
    : m_type(type)
    , m_uid(uid)
    , m_gid(gid)
{
    m_access    = OwnerRWX;
    m_size      = 0;
}

File::~File()
{
}

FileType File::getType() const
{
    return m_type;
}

Error File::read(IOBuffer & buffer, Size size, Size offset)
{
    return ENOTSUP;
}

Error File::write(IOBuffer & buffer, Size size, Size offset)
{
    return ENOTSUP;
}

Error File::status(FileSystemMessage *msg)
{
    FileStat st;
    Error e;

    // Fill in the status structure
    st.type     = m_type;
    st.access   = m_access;
    st.size     = m_size;
    st.userID   = m_uid;
    st.groupID  = m_gid;
    st.deviceID.major = m_deviceId.major;
    st.deviceID.minor = m_deviceId.minor;

    // Copy to the remote process
    if ((e = VMCopy(msg->from, API::Write, (Address) &st,
                   (Address) msg->stat, sizeof(st)) > 0))
    {
        return ESUCCESS;
    }
    else
        return e;
}
