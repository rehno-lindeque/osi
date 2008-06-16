#ifndef __LINGLKERNEL_API_H__
#define __LINGLKERNEL_API_H__
//////////////////////////////////////////////////////////////////////////////
//
//    API.H
//
//    Author:      Rehno Lindeque
//
//    Description: LinGLKernel classes used to implement api
//
//    Version: 0.1 (prototype)
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                                   INCLUDES                               */
// GLX
#include <GL/glx.h>
//#include <X11/Xlib.h>

// OpenKernel
#include <osi/kernel/kernel.hpp>
#include <osi/kernel/intkernel.hpp>
#include <osi/kernel/ext/linglkernel.h>

// BaseKernel
#include <basekernel/basekernel.h>

// LinGLKernel
#include "displaychain.h"
#include "display.h"
#include "frame.h"

/*                             FORWARD DECLERATIONS                         */

/*                                   CLASSES                                */
namespace LinGLKernel
{
  class Kernel : public BaseKernel::Kernel
  {
  protected:

  public:
    int glxVersionMinor, glxVersionMajor;
    ::IntKernel* intKernel;

    OSobject createWindowGLX1_2(::Display* display, int screen, const OSchar* name, OSbool fullScreen, OSuint width, OSuint height);
    OSobject createWindowGLX1_3(::Display* display, int screen, const OSchar* name, OSbool fullScreen, OSuint width, OSuint height);
  };

  class IntKernel : public ::IntKernel
  {
  /*public:
    Kernel* kernel;*/
  };

}

/*                                   INCLUDES                               */


#endif
