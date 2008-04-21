#ifndef __OSI_INTKERNEL_HPP__
#define __OSI_INTKERNEL_HPP__
//////////////////////////////////////////////////////////////////////////////
//
//    INTKERNEL.HPP
//
//    Ver 0.1 (2007-02-02)
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Internal OpenKernel C++ interface.

    DEPENDENCIES:
      kernel.h must be included before this file
*/
/*                                    API                                   */
class IntKernel
{
public:
  /* types */
  struct ClipFrame
  {
    OSid display;
    OSuint x1, y1, x2, y2;
  };

  /* frames */
  OSI_DYNAMIC_METHOD const void* OSI_API_CALL openClipFrames(OSobject displayChain, OSobject frame);
  OSI_DYNAMIC_METHOD void OSI_API_CALL closeClipFrames();

  /* displays */
  OSI_DYNAMIC_METHOD const void* OSI_API_CALL openDisplays(OSobject displayChain);
  OSI_DYNAMIC_METHOD void OSI_API_CALL closeDisplays();
};

#endif
