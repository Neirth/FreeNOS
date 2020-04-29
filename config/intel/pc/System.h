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

#ifndef __INTEL_PC_SYSTEM_H
#define __INTEL_PC_SYSTEM_H

#define INTEL
#define PC
#define __SYSTEM

#include <intel/IntelConstant.h>

#ifndef __HOST__
#include <intel/IntelCore.h>
#include <intel/IntelCache.h>
#include <intel/IntelIO.h>
#include <intel/IntelState.h>
#include <intel/IntelPaging.h>
#include <intel/IntelMap.h>
#include <intel/IntelKernel.h>
#include <intel/IntelProcess.h>
#include <intel/IntelSerial.h>

#include <FreeNOS/API.h>
#endif /* __HOST__ */

#endif /* __INTEL_PC_SYSTEM_H */
