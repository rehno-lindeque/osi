#ifndef __WINGLKERNEL_DISPLAYCHAIN_H__
#define __WINGLKERNEL_DISPLAYCHAIN_H__
//////////////////////////////////////////////////////////////////////////////
//
//    DISPLAYCHAIN.H
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      WinGLKernel DisplayChain class.
*/

namespace WinGLKernel
{
/*                                  CLASSES                                 */
  class DisplayChain : public Base::Object
  {
  public:
    struct DisplayInstance
    {
      OSid display;
      OSfloat x1, y1, x2, y2;
    }* displayInstances;
    OSuint32 nDisplayInstances;
  };
}

#endif
