//////////////////////////////////////////////////////////////////////////////
//
//    STREAM.CPP
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                              COMPILER MACROS                             */
#pragma warning(push)

/*                                 INCLUDES                                 */
#include <windows.h>
#include <gl/glee.h>
#include <gl/gl.h>
#include "../common/types.h"
#include "stream.h"
using namespace BaseRE;

#include <math.h>

/*                                  GLOBALS                                 */

/*                              IMPLEMENTATION                              */

uint32 GlStream::getSmallestAlignedNumber(uint32 number)
{
  uint32 mostSignificantBit = 0x80000000;

  if(number > mostSignificantBit)
    return 0; // error

  for(int c = 0; c < 32; c++)
  {
    if((mostSignificantBit & number) == 0)
    {
      //mostSignificantBit >> 1; //COMPILER BUG: THIS WON'T SEEM TO WORK!!! (probably due to SSE2 ceil function preceding this method call???)
      mostSignificantBit /= 2;
    }
    else // if number contains the bit, then it is its most significant bit
    {
      if(number - mostSignificantBit > 0)
        return mostSignificantBit << 1; // but this does seem to work???
        //return mostSignificantBit * 2;
      else
        return mostSignificantBit;
    }

  }
  return 0;
}

void GlStream::create(uint sizeX, uint sizeY, StreamFormat format, const void* data)
{
  GlStream::format = format;

  // Calculate optimal texture size (todo: correct?)
  size = getSmallestAlignedNumber(max(sizeX, sizeY));

  // Generate the texture
  glGenTextures(1, &textureObject);
  glBindTexture(GL_TEXTURE_2D, textureObject);

  // Initialize and set the initial texture
  void* tempData = null;
  switch(format)
  {
  case VEC4_FLOAT16_STREAM:
    if(data)
    {
      tempData = new float[size*size*4];
      for(uint cy = 0; cy < sizeY; ++cy)
        memcpy((float*)tempData+size*4*cy, ((const float*)data)+sizeX*4*cy, sizeX*4*sizeof(float));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, size, size, 0, GL_RGBA, GL_FLOAT, tempData); // requires GL_ARB_texture_float extension
    break;
  case VEC3_FLOAT16_STREAM:
    if(data)
    {
      tempData = new float[size*size*3];
      for(uint cy = 0; cy < sizeY; ++cy)
        memcpy((float*)tempData+size*3*cy, ((const float*)data)+sizeX*3*cy, sizeX*3*sizeof(float));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, size, size, 0, GL_RGB, GL_FLOAT, tempData); // requires GL_ARB_texture_float extension
    break;
  case VEC4_FIXED8_STREAM:
    if(data)
    {
      tempData = new uint8[size*size*4];
      for(uint cy = 0; cy < sizeY; ++cy)
        memcpy((uint8*)tempData+size*4*cy, ((const uint8*)data)+sizeX*4*cy, sizeX*4*sizeof(uint8));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempData);
    break;
  case FLOAT16_STREAM:
    if(data)
    {
      tempData = new float[size*size];
      for(uint cy = 0; cy < sizeY; ++cy)
        memcpy((float*)tempData+size*cy, ((const float*)data)+sizeX*cy, sizeX*sizeof(float));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA16F_ARB, size, size, 0, GL_ALPHA, GL_FLOAT, tempData);
    break;
  case UINT32_STREAM:
    if(data)
    {
      tempData = new uint32[size*size];
      for(uint cy = 0; cy < sizeY; ++cy)
        memcpy((uint32*)tempData+size*cy, ((const uint32*)data)+sizeX*cy, sizeX*sizeof(uint32));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, size, size, 0, GL_ALPHA, GL_UNSIGNED_INT, tempData);
    break;
  case FIXED16_STREAM:
  case UINT16_STREAM:
    if(data)
    {
      tempData = new uint16[size*size];
      for(uint cy = 0; cy < sizeY; ++cy)
        memcpy((uint16*)tempData+size*cy, ((const uint16*)data)+sizeX*cy, sizeX*sizeof(uint16));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA16, size, size, 0, GL_ALPHA, GL_UNSIGNED_SHORT, tempData);
    /*GL_ALPHA16_EXT
    GL_LUMINANCE16_EXT
    GL_ALPHA16F_ARB
    GL_INTENSITY16F_ARB
    GL_LUMINANCE16F_ARB*/
    break;
  }
  delete[] tempData;
  tempData = null;

  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //as opposed to GL_LINEAR \ mipmap etc
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //todo? glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//todo? glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  // calculate element sizes
  calculateElementSize();
}

void GlStream::create(uint length, StreamFormat format, const void* data)
{
  uint sqrtSize = (uint)ceil(sqrt((float)length));
  size = getSmallestAlignedNumber(sqrtSize);
  GlStream::format = format;

  glGenTextures(1, &textureObject);
  glBindTexture(GL_TEXTURE_2D, textureObject);

  /*temp */
  if(size < 512)
    size = 512;

  void* tempData = null;
  switch(format)
  {
  case VEC4_FLOAT16_STREAM:
    if(data)
    {
      tempData = new float[size*size*4];
      memcpy(tempData, data, length*4*sizeof(float));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, size, size, 0, GL_RGBA, GL_FLOAT, tempData); // requires GL_ARB_texture_float extension
    break;
  case VEC3_FLOAT16_STREAM:
    if(data)
    {
      tempData = new float[size*size*3];
      memcpy(tempData, data, length*3*sizeof(float));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, size, size, 0, GL_RGB, GL_FLOAT, tempData); // requires GL_ARB_texture_float extension
    break;
  case VEC4_FIXED8_STREAM:
    if(data)
    {
      tempData = new uint8[size*size*4];
      memcpy(tempData, data, length*4*sizeof(uint8));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempData);
    break;
  case FLOAT16_STREAM:
    if(data)
    {
      tempData = new float[size*size];
      memcpy(tempData, data, length*sizeof(float));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA16F_ARB, size, size, 0, GL_ALPHA, GL_FLOAT, tempData); // requires GL_ARB_texture_float extension
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE16F_ARB, size, size, 0, GL_LUMINANCE, GL_FLOAT, tempData);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16_ARB, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, tempData);
    break;
  case UINT32_STREAM:
    if(data)
    {
      tempData = new uint32[size*size];
      memcpy(tempData, data, length*sizeof(uint32));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, size, size, 0, GL_ALPHA, GL_UNSIGNED_INT, tempData);
    break;
  case FIXED16_STREAM:
  case UINT16_STREAM:
    if(data)
    {
      tempData = new uint16[size*size];
      memcpy(tempData, data, length*sizeof(uint16));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA16, size, size, 0, GL_ALPHA, GL_UNSIGNED_SHORT, tempData);
    break;
  }
  delete[] tempData;
  tempData = null;

  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //as opposed to GL_LINEAR \ mipmap etc
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // calculate element size
  calculateElementSize();
}

void GlStream::destroy()
{
  glDeleteTextures(1, &textureObject);
  //delete[] mappedMemory
}

void* GlStream::map(MapFlag flag)
{
  // check that the pbo does not exist yet
  if(pixelBufferObject != null)
    return null; // error: the glstream probably hasn't been unmapped since a previous map operation

  if(flag == WRITE_ONLY)
  {
    // create & bind a pbo to use for texture transfer
    glGenBuffers(1, &pixelBufferObject);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pixelBufferObject);
    //glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, size*size*4, NULL, GL_STREAM_DRAW); //todo: correct flag?? not GL_STATIC_DRAW \ GL_DYNAMIC_DRAW?
    //glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, size*size, NULL, GL_STREAM_DRAW); //todo: correct flag?? not GL_STATIC_DRAW \ GL_DYNAMIC_DRAW?
    glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, size*size*getMappedElementSize(), NULL, GL_STREAM_DRAW); //todo: correct flag?? not GL_STATIC_DRAW \ GL_DYNAMIC_DRAW?
    //glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, size*size*getInternElementSize(), NULL, GL_STREAM_DRAW); //todo: correct flag?? not GL_STATIC_DRAW \ GL_DYNAMIC_DRAW?

    // map the pbo
    mappedDataPos = mappedData = glMapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY);
#ifdef _DEBUG
    GLenum error = glGetError();
#endif
    return mappedData;
  }
  else
    return null;
}

void GlStream::unmap()
{
  //unmap the pbo
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pixelBufferObject);
  glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB);

  // update the texture from the pbo
  glBindTexture(GL_TEXTURE_2D, textureObject);
  //old: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);  ????
  switch(format)
  {
  case VEC4_FLOAT16_STREAM:
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, size, size, 0, GL_RGBA, GL_FLOAT, 0);
    //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size, GL_RGBA, GL_FLOAT, null);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size, GL_RGBA, GL_FLOAT, null);
    break;
  case VEC3_FLOAT16_STREAM:
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size, GL_RGB, GL_FLOAT, null);
    break;
  case VEC4_FIXED8_STREAM:
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size, GL_RGBA, GL_UNSIGNED_BYTE, null);
    break;
  case FLOAT16_STREAM:
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size, GL_ALPHA, GL_FLOAT, null);
    break;
  case UINT32_STREAM:
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size, GL_ALPHA, GL_UNSIGNED_INT, null);
    break;
  case FIXED16_STREAM:
  case UINT16_STREAM:
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size, GL_ALPHA, GL_UNSIGNED_SHORT, null);
    break;
  }

  // unbind & destroy the pbo
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
  glDeleteBuffers(1, &pixelBufferObject);
  pixelBufferObject = 0;
  mappedDataPos = (uint8*)((uint8*)mappedDataPos-(uint8*)mappedData); // mappedDataPos is the length of the buffer
  mappedData = null;
}

GLuint GlStream::bindVertexBuffer()
{
  // create buffer
  glGenBuffers(1, &pixelBufferObject);

  // bind buffer as PBO
  glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pixelBufferObject);
  //glBufferData(GL_PIXEL_PACK_BUFFER_ARB, numberVertices*4, NULL, GL_DYNAMIC_DRAW); //GL_STATIC_DRAW???
  //glBufferData(GL_PIXEL_PACK_BUFFER_ARB, getLength(), NULL, GL_DYNAMIC_DRAW); //GL_STATIC_DRAW???
  //glBufferData(GL_PIXEL_PACK_BUFFER_ARB, size*size, NULL, GL_DYNAMIC_DRAW); //GL_STATIC_DRAW???
  //glBufferData(GL_PIXEL_PACK_BUFFER_ARB, size*size*4, NULL, GL_DYNAMIC_DRAW); //GL_STATIC_DRAW???
  glBufferData(GL_PIXEL_PACK_BUFFER_ARB, size*size*getMappedElementSize(), NULL, GL_DYNAMIC_DRAW); //GL_STATIC_DRAW???

  // bind texture
  glBindTexture(GL_TEXTURE_2D, textureObject);

  // read buffer from texture
  switch(format)
  {
  case VEC4_FLOAT16_STREAM:
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, 0);
    //or perhaps: glGetTexImage(TEXTURE_2D, 0, GL_RGBA16F_ARB, GL_FLOAT, 0); ???
    break;
  case VEC3_FLOAT16_STREAM:
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, 0);
    break;
  case VEC4_FIXED8_STREAM:
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    break;
  }

  // unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);

  // unbind PBO
  glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);

  // bind buffer as VBO
  glBindBuffer(GL_ARRAY_BUFFER, pixelBufferObject);
  return pixelBufferObject;
}

void GlStream::unbindVertexBuffer()
{
  // unbind buffer (as VBO)
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // destroy buffer
  glDeleteBuffers(1, &pixelBufferObject);
}

GLuint GlStream::bindIndexBuffer()
{
  // create buffer
  glGenBuffers(1, &pixelBufferObject);

  // bind buffer as PBO
  glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pixelBufferObject);
  glBufferData(GL_PIXEL_PACK_BUFFER_ARB, size*size*getMappedElementSize(), NULL, /*GL_STATIC_DRAW*/ GL_DYNAMIC_DRAW); //GL_DYNAMIC_DRAW???

  // bind texture
  glBindTexture(GL_TEXTURE_2D, textureObject);

  // read buffer from texture
  switch(format)
  {
  /*NOT SUPPORTED BY OPENGL: case UINT32_STREAM:
    glGetTexImage(GL_TEXTURE_2D, 0, GL_ALPHA, GL_UNSIGNED_INT, 0);
    break;*/
  case FIXED16_STREAM:
  case UINT16_STREAM:
    glGetTexImage(GL_TEXTURE_2D, 0, GL_ALPHA, GL_UNSIGNED_SHORT, 0);
    break;
  }

  // unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);

  // unbind PBO
  glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);

  // bind buffer as IBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pixelBufferObject);
  return pixelBufferObject;
}

void GlStream::unbindIndexBuffer()
{
  // unbind buffer (as IBO)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // destroy buffer
  glDeleteBuffers(1, &pixelBufferObject);
}

GLuint GlStream::bindTexture(uint index)
{
  glActiveTexture(GL_TEXTURE0 + index);
  //glEnable(GL_TEXTURE_2D); //todo: needed (by shader)? move to initialization?
  glBindTexture(GL_TEXTURE_2D, textureObject);
  return textureObject;
}

void GlStream::unbindTexture(uint index)
{
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint GlStream::bindOutput(uint index)
{
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + index, GL_TEXTURE_2D, textureObject, 0);
  return textureObject;
}

void GlStream::unbindOutput(uint index)
{
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + index, GL_TEXTURE_2D, 0, 0);
}

GLuint GlStream::bindVertexAttributeBuffer()
{
  // create buffer
  glGenBuffers(1, &pixelBufferObject);

  // bind buffer as PBO
  glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pixelBufferObject);
  glBufferData(GL_PIXEL_PACK_BUFFER_ARB, size*size*getMappedElementSize(), NULL, GL_DYNAMIC_DRAW); //GL_STATIC_DRAW???

  // bind texture
  glBindTexture(GL_TEXTURE_2D, textureObject);

  // read buffer from texture
  switch(format)
  {
  case VEC4_FLOAT16_STREAM:
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, 0);
    //or perhaps: glGetTexImage(TEXTURE_2D, 0, GL_RGBA16F_ARB, GL_FLOAT, 0); ???
    break;
  case VEC3_FLOAT16_STREAM:
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, 0);
    break;
  case VEC4_FIXED8_STREAM:
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    break;
  case FLOAT16_STREAM:
    glGetTexImage(GL_TEXTURE_2D, 0, GL_ALPHA, GL_FLOAT, 0);
    break;
  case FIXED16_STREAM:
    glGetTexImage(GL_TEXTURE_2D, 0, GL_ALPHA, GL_UNSIGNED_SHORT, 0);
    break;
  }

  // unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);

  // unbind PBO
  glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);

  // bind buffer as Vertex Attribute Buffer Object
  glBindBuffer(GL_ARRAY_BUFFER, pixelBufferObject);
  return pixelBufferObject;
}

void GlStream::unbindVertexAttributeBuffer()
{
  // unbind buffer (as VBO)
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // destroy buffer
  glDeleteBuffers(1, &pixelBufferObject);
}

/*                              COMPILER MACROS                             */
#pragma warning(pop)
