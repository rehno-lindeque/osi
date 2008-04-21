#ifndef __WINGLKERNEL_FRAME_H__
#define __WINGLKERNEL_FRAME_H__
//////////////////////////////////////////////////////////////////////////////
//
//    FRAME.H
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      WinGLKernel Frame class.
*/

namespace WinGLKernel
{
/*                                  CLASSES                                 */
  class Frame : public Base::Object
  {
  public:
    struct { OSfloat x1, y1, x2, y2;  } area;

    inline void setArea(OSfloat x1, OSfloat y1, OSfloat x2, OSfloat y2)
    {
      if(x1 > x2)
      {
        area.x1 = x2;
        area.x2 = x1;
      }
      else
      {
        area.x1 = x1;
        area.x2 = x2;
      }
      
      if(y1 > y2)
      {
        area.y1 = y2;
        area.y2 = y1;
      }
    }
  };
}

#endif
