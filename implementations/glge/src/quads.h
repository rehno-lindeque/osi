#ifndef __GLGE_QUADS_H__
#define __GLGE_QUADS_H__
//////////////////////////////////////////////////////////////////////////////
//
//    QUADS.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      GLGE Quads
*/

namespace GLGE
{
/*                                  CLASSES                                 */
  class GLQuads : public BaseGE::Components
  {
  public:
    inline GLQuads() : Components(GE_QUAD) {}
  };
}

#endif
