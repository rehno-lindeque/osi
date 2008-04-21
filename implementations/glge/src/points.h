#ifndef __GLGE_POINTS_H__
#define __GLGE_POINTS_H__
//////////////////////////////////////////////////////////////////////////////
//
//    POINTS.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      GLGE Points
*/

namespace GLGE
{
/*                                  CLASSES                                 */
  class GLPoints : public BaseGE::Components
  {
  public:
    inline GLPoints() : Components(GE_POINT) {}
  };
}

#endif
