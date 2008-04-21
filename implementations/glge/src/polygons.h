#ifndef __GLGE_POLYGONS_H__
#define __GLGE_POLYGONS_H__
//////////////////////////////////////////////////////////////////////////////
//
//    POLYGONS.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      GLGE Polygons
*/

namespace GLGE
{
/*                                  CLASSES                                 */
  class GLPolygons : public BaseGE::Components
  {
  public:
    inline GLPolygons() : Components(GE_POLYGON) {}
  };
}

#endif
