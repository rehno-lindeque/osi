#ifndef __OSIX_STREAM_HPP__
#define __OSIX_STREAM_HPP__
//////////////////////////////////////////////////////////////////////////////
//
//    STREAM.HPP
//
//    Ver 0.1 (2008-06-29)
//
//    Copyright © 2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Main OpenStream C++ interface.
*/
/*                                 INCLUDES                                 */
#include "../../osi/os.hpp"

/*                                 CONSTANTS                                */
#define STREAM_VERSION "0.1"

namespace OSIX
{
  /*                                   TYPES                                  */

  /*                                     API                                  */
  class Stream
  {
  protected:
    inline Stream() {}
    inline Stream(const Stream&) {}

  public:
    /* read */
    OSI_DYNAMIC_METHOD const OSuint8* ReadElement();
    OSI_DYNAMIC_METHOD const OSuint8* ReadIndex(OSuint index);
      
    /* write */
    
    /* query */
    //OSI_DYNAMIC_METHOD bool QueryOpen();
    
    /* helpers */
    template<typename T> OSI_INLINE_METHOD const T* ReadTypeElement() { return reinterpret_cast<const T*>(ReadElement()); }
    template<typename T> OSI_INLINE_METHOD const T* ReadTypeIndex(OSuint index) { return reinterpret_cast<const T*>(ReadIndex(index)); }
  };

  Stream* OSI_API_CALL StreamInit(OSobject stream);
}

#endif
