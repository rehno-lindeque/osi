#ifndef __BASERE_STREAM_H__
#define __BASERE_STREAM_H__
//////////////////////////////////////////////////////////////////////////////
//
//    STREAM.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      GPU streams for general gpu data storage.

    USAGE:
      glee.h, gl.h, types.h must be included before this header

    IMPLEMENTATION:
      + mappedDataPos indicates the current position in the buffer if the
        stream has been mapped to memory.
        If the stream is unmapped it indicates the length of the buffer
        when the stream was unmapped (see the unmap() method)

      + pixelBufferObject is used as a vertex buffer object when bindVBO()
        is called. At this time, pixelBufferObject must be unassigned.
        I.e. bindVBO cannot be called in-between map\unmap.

    TODO:
      + Implement a stream manager that allows
      1. Very large streams to be used
      2. Streams to be swapped to system memory (and possibly swapped to
         disk (by the OS))
     + Create an inline implementation file
     + Possibly use glTexSubImage2D to define textures in the create method
       with sizeX, sizeY rather than copying the complete texture as is.

    CHANGES:
      2006-04-08 (Rehno Lindeque) - Split up files to clean up interface
      2006-04-08 (Rehno Lindeque) - Added pixel buffer object for texture
                                    transfer
*/
/*                              COMPILER MACROS                             */
#pragma warning(push)
#pragma warning(disable : 4244)

#ifndef __glee_h_
  #error("glee.h must be included before stream.h")
#endif
#if !(defined(__GL_H__) || defined(__gl_h__))
  #error("gl.h must be included before stream.h")
#endif
#ifndef __BASERE_TYPES_H__
  #error("types.h must be included before stream.h")
#endif

/*                                 INCLUDES                                 */
namespace BaseRE
{

/*                                 CLASSES                                  */
enum StreamFormat
{
  VEC4_FLOAT16_STREAM,
  VEC3_FLOAT16_STREAM,
  //VEC4_FLOAT32_STREAM ??
  VEC4_FIXED8_STREAM,
  FLOAT16_STREAM, //scalar format
  FIXED16_STREAM,
  UINT16_STREAM,
  UINT32_STREAM
  //todo: INT_FORMAT etc
  //todo: HALF formats
};

enum MapFlag
{
  WRITE_ONLY
  //READ_ONLY
  //READ_WRITE
};

class GlStream
{
protected:
  GLuint textureObject;
  GLuint pixelBufferObject;
  StreamFormat format;
  uint size;              // this is the size of the stream in all dimensions
  uint mappedElementSize;
  uint internElementSize;
  void* mappedDataPos;
  void* mappedData;

  /*  This methods finds the smallest power of two number that is larger than the given number using a bit-compare method  */
  uint32 getSmallestAlignedNumber(uint32 number);
  inline void calculateElementSize();

public:
  GlStream();
  inline GLuint getTextureObject();
  void create(uint sizeX, uint sizeY, StreamFormat format = VEC4_FLOAT16_STREAM, const void* data = null);
  void create(uint size, StreamFormat format = VEC4_FLOAT16_STREAM, const void* data = null);
  void destroy();
  inline uint getInternElementSize();
  inline uint getMappedElementSize();

  /* The following operations are intended for streaming from the cpu */
  void* map(MapFlag flag);
  void unmap();
  template<class Type> inline void appendCopy(Type value);
  inline void appendCopy(const void* data, uint length);
  inline void* appendCreate(uint length); //or appendCreate
  inline uint getMapOffset();
  inline uint getLength(); // returns the length of the data stored in the map in element units. Eg, One VEC4_FLOAT16 = 4 16-bit floats
  inline bool isMapped();
  inline uint getMappedBufferSize(); // returns the size in bytes of the mapped memory

  /* These operations write directly to video memory */
  inline void setData(const void* data, uint length = 0);
  inline void setData(uint sizeX, uint sizeY, const void* data, uint length = 0);

  /* The following operations are intended to convert the stream to geometry (e.g. vbo) */
  GLuint bindVertexBuffer();
  void   unbindVertexBuffer();
  GLuint bindIndexBuffer();
  void   unbindIndexBuffer();

  /* binding as texture memory (index specifies texture unit to bind\unbind to) */
  GLuint bindTexture(uint index);
  static void unbindTexture(uint index);

  /* binding as shader output */
  GLuint bindOutput(uint index);
  static void unbindOutput(uint index);

  /* vertex attributes */
  GLuint bindVertexAttributeBuffer();
  void unbindVertexAttributeBuffer();
};

class GlInputStream : public GlStream
{};

class GlIOStream : public GlStream
{};

#include "stream.hxx"

}

#pragma warning(pop)
#endif
