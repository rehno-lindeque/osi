#ifndef __BASERE_BASEREDBG_H__
#define __BASERE_BASEREDBG_H__
//////////////////////////////////////////////////////////////////////////////
//
//    BASEREDBG.H
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Debug modules for OpenRE backends.
*/
/*                              COMPILER MACROS                             */
#ifdef _DEBUG

#define BASEREDBG_BREAKPOINT _ASSERT(false);
#define BASEREDBG_RASTERPOSITION(x,y) if(BaseREDBG::isRasterPositionBreakpoint(x,y)) BASEREDBG_BREAKPOINT

#pragma warning(push)

/*                                 INCLUDES                                 */
// OpenRE
#include <osi/os.h>
#include <osi/re/redbg.h>

// libstdc++
#include <vector>

// Base
#include <base/common/types.h>

namespace BaseREDBG
{
/*                                 GLOBALS                                  */
  extern std::vector< std::pair<OSuint, OSuint> > rasterPositionBreakpoints;

/*                          FORWARD DECLERATIONS                            */
  inline bool isRasterPositionBreakpoint(OSuint x, OSuint y);
}

/*                              IMPLEMENTATION                              */
#include "baseredbg.hxx"

/*                              COMPILER MACROS                             */
#pragma warning(pop)

#else

#define BASEREDBG_BREAKPOINT
#define BASEREDBG_RASTERPOSITION(x,y)

#endif
#endif
