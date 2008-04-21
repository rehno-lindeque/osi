#ifndef __NATIVEGE_QUADS_H__
#define __NATIVEGE_QUADS_H__
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
      NativeGE Quads
*/

namespace NativeGE
{
/*                                  CLASSES                                 */
  class NativeQuads : public BaseGE::Components
  {
  public:
    inline NativeQuads() : Components(GE_QUAD) {}
  };
}

#endif
