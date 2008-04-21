#ifndef __GLGE_TRIANGLES_H__
#define __GLGE_TRIANGLES_H__
//////////////////////////////////////////////////////////////////////////////
//
//    TRIANGLES.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      GLGE Triangles
*/

namespace GLGE
{
/*                                  CLASSES                                 */
  class GLTriangles : public BaseGE::Components
  {
  public:
    inline GLTriangles() : Components(GE_TRIANGLE) {}
  };
}

#endif
