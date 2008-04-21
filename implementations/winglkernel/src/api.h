#ifndef __WINGLKERNEL_API_H__
#define __WINGLKERNEL_API_H__
//////////////////////////////////////////////////////////////////////////////
//
//    API.H
//
//    Author:      Rehno Lindeque
//
//    Description: WinGLKernel classes used to implement api
//
//    Version: 0.1 (prototype)
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                                   INCLUDES                               */
// Windows
#include <windows.h>

// OpenKernel
#include <osi/kernel/kernel.hpp>
#include <osi/kernel/intkernel.hpp>
#include <osi/kernel/ext/winglkernel.h>

// BaseKernel
#include <basekernel/basekernel.h>

// WinGLKernel
#include "displaychain.h"
#include "frame.h"

/*                             FORWARD DECLERATIONS                         */

/*                                   CLASSES                                */
namespace WinGLKernel
{
  class Kernel : public BaseKernel::Kernel
  {
  protected:
    
  public:
    ::IntKernel* intKernel;

    static LRESULT CALLBACK windowProc(HWND hWnd, uint uiMsg, WPARAM wParam, LPARAM lParam);
  };

  class IntKernel : public ::IntKernel
  {
  /*public:
    Kernel* kernel;*/
  };

}

/*                                   INCLUDES                               */


#endif
