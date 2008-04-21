#ifndef __BASERE_BASEREDBG_HXX__
#define __BASERE_BASEREDBG_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    BASEREDBG.HXX
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace BaseREDBG
{
/*                                FUNCTIONS                                 */
  inline bool isRasterPositionBreakpoint(OSuint x, OSuint y)
  {
    for(std::vector< std::pair<OSuint, OSuint> >::iterator i = rasterPositionBreakpoints.begin(); i != rasterPositionBreakpoints.end(); ++i)
      if((*i).first == x && (*i).second == y)
        return true;
    return false;
  }
}

#endif
