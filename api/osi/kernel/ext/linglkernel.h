#ifndef __OSIEXT_LINGLKERNEL_H__
#define __OSIEXT_LINGLKERNEL_H__
//////////////////////////////////////////////////////////////////////////////
//
//    LINGLKERNEL.H
//
//    Author:      Rehno Lindeque
//
//    Description: LinGLKernel extensions
//
//    Version: 0.1 (informal prototype)
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

struct LinGLKernelDisplayObject
{
  Display* xDisplay;
  GLXDrawable glxDrawable;
  GLXContext glxContext;
  OSuint32 x1, y1, x2, y2;
};

#endif
