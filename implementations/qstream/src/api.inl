#ifdef  __QSTREAM_API_H__
#ifndef __QSTREAM_API_INL__
#define __QSTREAM_API_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    API.INL
//
//    Copyright © 2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              COMPILER MACROS                             */
#define _this (*static_cast<QStream::Stream*>(this))

#define cast_id     QStream::Stream::cast_id
#define cast_object QStream::Stream::cast_object

/*                              IMPLEMENTATION                              */
/*OSobject OSI_API_CALL OSIX::Stream::openStream()
{
}*/

const OSuint8* OSI_API_CALL OSIX::Stream::ReadElement()
{
  // Pre-conditions: check that the stream is (still) valid
  if(!_this.streamPosition || _this.remainingStreamSize == 0 || _this.elementSize == 0)
    return null;

  // Read an element from the stream
  const OSuint8* element = _this.streamPosition;
  --_this.remainingStreamSize;
  _this.streamPosition += _this.elementSize;
  return element;
}

const OSuint8* OSI_API_CALL OSIX::Stream::ReadIndex(OSuint index)
{
  // Pre-conditions: check that the stream is (still) valid
  if(!_this.streamPosition || _this.remainingStreamSize == 0 || _this.elementSize == 0)
    return null;

  // Read the element at index from the stream
  const OSuint8* element = _this.streamData + _this.elementSize * index;
  return element;
}

OSIX::Stream* OSI_API_CALL OSIX::StreamInit(OSobject stream)
{
  uint32* streamSize = reinterpret_cast<uint32*>(stream);
  uint32* elementSize = reinterpret_cast<uint32*>(stream)+1;
  OSobject streamDataAddr = *reinterpret_cast<OSobject*>(reinterpret_cast<uint32*>(stream)+2);
  
  QStream::Stream *streamObject = new QStream::Stream();
  streamObject->remainingStreamSize = *streamSize;
  streamObject->elementSize = *elementSize;
  streamObject->streamData = reinterpret_cast<uint8*>(streamDataAddr);
  streamObject->streamPosition = streamObject->streamData;
  return static_cast<OSIX::Stream*>(streamObject);
}

#endif
#endif

