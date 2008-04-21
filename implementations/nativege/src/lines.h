#ifndef __NATIVEGE_LINES_H__
#define __NATIVEGE_LINES_H__
//////////////////////////////////////////////////////////////////////////////
//
//    LINES.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      NativeGE Lines
*/

namespace NativeGE
{
/*                                  CLASSES                                 */
  class NativeLines : public BaseGE::Components
  {
  public:
    inline NativeLines() : Components(GE_LINE) {}
  };
}

#endif
