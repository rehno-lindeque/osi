#ifndef __OSIEXT_WINGLKERNEL_H__
#define __OSIEXT_WINGLKERNEL_H__
//////////////////////////////////////////////////////////////////////////////
//
//    WINGLKERNEL.H
//
//    Author:      Rehno Lindeque
//
//    Description: WinGLKernel extensions
//
//    Version: 0.1 (informal prototype)
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include <windows.h> 

struct WinGLKernelDisplayObject
{
  HWND hWnd;
  HGLRC hRC;
  OSuint32 x1, y1, x2, y2;
};

#endif
