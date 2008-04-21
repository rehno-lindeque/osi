#ifndef __OSI_KERNEL_HPP__
#define __OSI_KERNEL_HPP__
//////////////////////////////////////////////////////////////////////////////
//
//    KERNEL.HPP
//
//    Ver 0.1 (2007-02-02)
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Main OpenKernel C++ interface.
*/
/*                                 INCLUDES                                 */
#include "../os.hpp"

/*                                 CONSTANTS                                */
#define KERNEL_VERSION "0.1"

/*                                   TYPES                                  */

/*                                    API                                   */
class Kernel
{
protected:
  inline Kernel() {}
  inline Kernel(const Kernel&) {}

public:
  /* display */
  OSI_DYNAMIC_METHOD OSobject OSI_API_CALL beginDisplay(OSuint displayNumber, const OSchar* name, OSbool fullScreen, OSuint width, OSuint height);
  OSI_DYNAMIC_METHOD void OSI_API_CALL endDisplay();

  OSI_DYNAMIC_METHOD OSobject OSI_API_CALL beginDisplayChain();
  OSI_DYNAMIC_METHOD void OSI_API_CALL endDisplayChain();

  OSI_DYNAMIC_METHOD void OSI_API_CALL display(OSobject display, OSfloat x1, OSfloat y1, OSfloat x2, OSfloat y2);

  OSI_DYNAMIC_METHOD void OSI_API_CALL refreshDisplay(OSobject display = 0);

  /* frame */
  OSI_DYNAMIC_METHOD OSobject OSI_API_CALL beginFrame(OSobject displayChain, OSobject parentFrame);
  OSI_DYNAMIC_METHOD void OSI_API_CALL endFrame();

  OSI_DYNAMIC_METHOD void OSI_API_CALL openFrame(OSobject frame);
  OSI_DYNAMIC_METHOD void OSI_API_CALL closeFrame();

  OSI_DYNAMIC_METHOD void OSI_API_CALL frameArea(OSfloat x1, OSfloat y1, OSfloat x2, OSfloat y2);

  /* dynamic library */
  OSI_DYNAMIC_METHOD void OSI_API_CALL loadDynamicLibrary(OSstring path);

  /* update */
  //OSI_DYNAMIC_METHOD void OSI_API_CALL update();

  /* miscelaneous */
  OSI_DYNAMIC_METHOD void* OSI_API_CALL getIntKernel();
  OSI_DYNAMIC_METHOD const OSstring OSI_API_CALL getVersion();
  OSI_DYNAMIC_METHOD void OSI_API_CALL shutdown();
};

/* initialize */
Kernel* OSI_API_CALL kernelInit(/*bool automaticUpdate*/);


#endif
