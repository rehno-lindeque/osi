#ifndef __QSTREAM_API_H__
#define __QSTREAM_API_H__
//////////////////////////////////////////////////////////////////////////////
//
//    API.H
//
//    Copyright © 2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      QStream API for OpenStream 0.1.
*/
/*                              COMPILER MACROS                             */
/*                                 INCLUDES                                 */
// OpenStream
#include <osix/stream/stream.hpp>

// BaseStream
#include <basestream/basestream.h>

// STL
//#include <list>
//#include <stack>

// QStream

/*                                  CLASSES                                 */
namespace QStream
{
  class Stream : public BaseStream::Stream
  {
  public:
    uint8* streamData;
    uint8* streamPosition;
    uint32 remainingStreamSize;
    uint32 elementSize;
    
    FORCE_INLINE Stream() {}
  };
}

/*                                   INCLUDES                               */
#ifdef OSI_STATIC_C_BUILD
#include "api.inl"
#endif

#endif
