/*
 * Copyright (C) 2006-2011 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2010-2011 ScriptDev0 <http://github.com/mangos-zero/scriptdev0>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef SC_PRECOMPILED_H
#define SC_PRECOMPILED_H



//Level 4 warnings
#pragma warning(disable:4061)								// 'enumerator' in switch of 'enum' is not explicitly handled by a case label
#pragma warning(disable:4100)								// unreferenced formal parameter
#pragma warning(disable:4121)								// alignment of a member was sensitive to packing
#pragma warning(disable:4263)								// member function does not override any base class virtual member function
#pragma warning(disable:4820)								// X bytes padding added after data member
#pragma warning(disable:4668)								// X is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#pragma warning(disable:4626)								// assignment operator could not be generated because a base class assignment operator is inaccessible
#pragma warning(disable:4625)								// copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning(disable:4555)								// expression has no effect; expected expression with side-effect
#pragma warning(disable:4350)								// behavior change: 'func'
#pragma warning(disable:4266)								// no override available for virtual member function from base 'T'; function is hidden
#pragma warning(disable:4264)								// no override available for virtual member function from base 'T'; function is hidden




#pragma warning(disable:4640)								// construction of local static object is not thread-safe


#include "../ScriptMgr.h"
#include "sc_creature.h"
#include "sc_gossip.h"
#include "sc_grid_searchers.h"
#include "sc_instance.h"

enum backports
{
    UNIT_VIRTUAL_ITEM_SLOT_ID = UNIT_VIRTUAL_ITEM_SLOT_DISPLAY
};

#ifdef WIN32
#  include <windows.h>
    BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID)
    {
        return true;
    }
#endif

#endif
