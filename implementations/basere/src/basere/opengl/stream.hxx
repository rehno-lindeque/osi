#ifdef __BASERE_STREAM_H__
#ifndef __BASERE_STREAM_HXX__
#define __BASERE_STREAM_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    STREAM.HXX
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */

/*                               IMPLEMENTATION                             */
inline GlStream::GlStream() : textureObject(0), pixelBufferObject(0), mappedData(null), mappedDataPos(null), format(VEC4_FLOAT16_STREAM), size(0)
{}

inline GLuint GlStream::getTextureObject()
{
  return textureObject;
}

inline void GlStream::calculateElementSize()
{
  switch(format)
  {
  case VEC4_FLOAT16_STREAM:
    internElementSize = 4*2;
    mappedElementSize = 4*sizeof(float32); // note: a float16 stream must be mapped to a float32 buffer since pc's don't have float16's
    return;
  case VEC3_FLOAT16_STREAM:
    internElementSize = 3*2;
    mappedElementSize = 3*sizeof(float32); // note: a float16 stream must be mapped to a float32 buffer since pc's don't have float16's
    return;
  /*TODO: case VEC4_FLOAT32_STREAM:
    internElementSize = 4*4;
    mappedElementSize = 4*sizeof(float32);
    return;*/
  case VEC4_FIXED8_STREAM:
    internElementSize = 4*1;
    mappedElementSize = 4*sizeof(uint8);
    return;
  case FLOAT16_STREAM:
    internElementSize = 1*2;
    mappedElementSize = 1*sizeof(float);
    return;
  case UINT32_STREAM:
    internElementSize = 1*4;
    mappedElementSize = 1*sizeof(uint32);
    return;
  case FIXED16_STREAM:
  case UINT16_STREAM:
    internElementSize = 1*2;
    mappedElementSize = 1*sizeof(uint16);
    return;
  default:
    internElementSize = mappedElementSize = 0;
  }
}

inline uint GlStream::getInternElementSize()
{
  return internElementSize;
}

inline uint GlStream::getMappedElementSize()
{
  return mappedElementSize;
}

template<class Type>
inline void GlStream::appendCopy(Type value)
{
  append(&value, sizeof(Type));
  //((Type*)mappedDataPos) = value;
  //mappedDataPos += (Type*)1;
}

inline void GlStream::appendCopy(const void* data, uint length)
{
  memcpy(mappedDataPos, data, length * getMappedElementSize());
  mappedDataPos = (void*)((uint8*)mappedDataPos + length * getMappedElementSize());
}

inline void* GlStream::appendCreate(uint length)
{
  void* currentPos = mappedDataPos;
  mappedDataPos = (void*)((uint8*)mappedDataPos + length * getMappedElementSize());
  return currentPos;
}

inline uint GlStream::getMapOffset()
{
  return ((uint8*)mappedDataPos-(uint8*)mappedData)/getMappedElementSize();
}

inline uint GlStream::getLength()
{
  return getMapOffset();
}

inline bool GlStream::isMapped()
{
  return mappedData != null;
}

inline uint GlStream::getMappedBufferSize()
{
  return size*size*getMappedElementSize();
}

inline void GlStream::setData(const void* data, uint length)
{
  // bind the texture
  glBindTexture(GL_TEXTURE_2D, textureObject);

  // determine opengl format
  GLint glInternalFormat;
  GLint glFormat;
  GLenum glType;
  int elementSize;
  switch(format)
  {
  case VEC4_FLOAT16_STREAM:
    glInternalFormat = GL_RGBA16F_ARB; // requires GL_ARB_texture_float extension
    glFormat = GL_RGBA;
    glType = GL_FLOAT;
    elementSize = sizeof(float) * 4;
    break;
  case VEC3_FLOAT16_STREAM:
    glInternalFormat = GL_RGB16F_ARB; // requires GL_ARB_texture_float extension
    glFormat = GL_RGB;
    glType = GL_FLOAT;
    elementSize = sizeof(float) * 3;
    break;
  case VEC4_FIXED8_STREAM:
    glInternalFormat = GL_RGBA;
    glFormat = GL_RGBA;
    glType = GL_UNSIGNED_BYTE;
    elementSize = sizeof(uint8) * 4;
    break;
  case FLOAT16_STREAM:
    glInternalFormat = GL_ALPHA16F_ARB;
    glFormat = GL_ALPHA;
    glType = GL_FLOAT;
    elementSize = sizeof(float);
    break;
  case UINT32_STREAM:
    glInternalFormat = GL_ALPHA;
    glFormat = GL_ALPHA;
    glType = GL_UNSIGNED_SHORT;
    elementSize = sizeof(uint32);
    break;
  case FIXED16_STREAM:
  case UINT16_STREAM:
    glInternalFormat = GL_ALPHA16;
    glFormat = GL_ALPHA;
    glType = GL_UNSIGNED_SHORT;
    elementSize = sizeof(uint16);
    break;
  }

  // copy data
  if(length != 0)
  {
    // copy rectangular data block
    uint sizeX = size;
    uint sizeY = length / size;
    if(sizeY > 0)
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sizeX, sizeY, glFormat, glType, data);

    // copy remaining data
    uint remaining = length - sizeY * size;
    if(sizeX > 0)
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, sizeY, remaining, 1, glFormat, glType, ((uint8*)data + (length - remaining) * elementSize));
  }
  else
    glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, size, size, 0, glFormat, glType, data);

  // set the stream length
#pragma warning(push)
#pragma warning(disable:4312)
  if(length != 0)
    mappedDataPos = (uint8*)(length * getMappedElementSize());
  else
    mappedDataPos = (uint8*)(size*size * getMappedElementSize());
#pragma warning(pop)
}

inline void GlStream::setData(uint sizeX, uint sizeY, const void* data, uint length)
{
  // bind the texture
  glBindTexture(GL_TEXTURE_2D, textureObject);

  // determine opengl format
  GLint glFormat;
  GLenum glType;
  int elementSize;
  switch(format)
  {
  case VEC4_FLOAT16_STREAM:
    glFormat = GL_RGBA;
    glType = GL_FLOAT;
    elementSize = sizeof(float) * 4;
    break;
  case VEC3_FLOAT16_STREAM:
    glFormat = GL_RGB;
    glType = GL_FLOAT;
    elementSize = sizeof(float) * 3;
    break;
  case VEC4_FIXED8_STREAM:
    glFormat = GL_RGBA;
    glType = GL_UNSIGNED_BYTE;
    elementSize = sizeof(uint8) * 4;
    break;
  case FLOAT16_STREAM:
    glFormat = GL_ALPHA;
    glType = GL_FLOAT;
    elementSize = sizeof(float);
    break;
  case UINT32_STREAM:
    glFormat = GL_ALPHA;
    glType = GL_UNSIGNED_INT;
    elementSize = sizeof(uint32);
    break;
  case FIXED16_STREAM:
  case UINT16_STREAM:
    glFormat = GL_ALPHA;
    glType = GL_UNSIGNED_SHORT;
    elementSize = sizeof(uint16);
    break;
  }

  // copy data
  if(length != 0)
  {
    // copy rectangular data block
    uint sizeY = length / sizeX;
    if(sizeY > 0)
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sizeX, sizeY, glFormat, glType, data);

    // copy remaining data
    uint remaining = length - sizeY * size;
    if(sizeX > 0)
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, sizeY, remaining, 1, glFormat, glType, ((uint8*)data + (length - remaining) * elementSize));
  }
  else
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sizeX, sizeY, glFormat, glType, data);

  // set the stream length
#pragma warning(push)
#pragma warning(disable:4312)
  if(length != 0)
    mappedDataPos = (uint8*)(length * getMappedElementSize());
  else
    mappedDataPos = (uint8*)(sizeX*sizeY * getMappedElementSize());
#pragma warning(pop)
}

#endif
#endif
