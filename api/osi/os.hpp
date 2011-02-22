#ifndef __OSI_OS_HPP__
#define __OSI_OS_HPP__
//////////////////////////////////////////////////////////////////////////////
//
//    OS.HPP
//
//    Ver 0.2.1 (2011-02-22)
//
//    Copyright © 2006-2011, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*
    DESCRIPTION:
      Open Standard Interface C++ header. Includes mechanisms for C wrapper
      library compilation
*/
/*                                 INCLUDES                                 */
#include "os.h"

/*                                   MACROS                                 */
/* inline method declaration */
#ifdef _MSC_VER
  #define OSI_INLINE_METHOD _inline
#else
  #define OSI_INLINE_METHOD inline
#endif

/* dynamic (api) method declaration */
#if defined(OSI_C_STATIC_BUILD) || defined(OSI_C_SHARED_BUILD)
  //#define OSI_DYNAMIC_METHOD OSI_INLINE_METHOD
  #define OSI_DYNAMIC_METHOD
#elif defined(OSI_CPP_SHARED_BUILD)
  #define OSI_DYNAMIC_METHOD virtual
#else
  #ifndef OSI_CPP_STATIC_BUILD
    #define OSI_CPP_STATIC_BUILD
  #endif
  #define OSI_DYNAMIC_METHOD
#endif

#endif
