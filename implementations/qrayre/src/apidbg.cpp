//////////////////////////////////////////////////////////////////////////////
//
//    APIDBG.CPP
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      QRayRE debug api implementation
*/
/*                              COMPILER MACROS                             */
#ifdef _DEBUG

#pragma warning(push)

/*                                 INCLUDES                                 */
// BaseRE
#include <basere/baseredbg.h>

using namespace BaseREDBG;

/*                                  GLOBALS                                 */

/*                               IMPLEMENTATION                             */
void redbgBreakRasterPosition(OSuint x, OSuint y)
{
  rasterPositionBreakpoints.push_back(std::pair<OSuint, OSuint>(x,y));
}

/*                              COMPILER MACROS                             */
#pragma warning(pop)

#endif
