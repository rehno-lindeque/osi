#ifndef __BASERE_RENDERTARGET_H__
#define __BASERE_RENDERTARGET_H__
//////////////////////////////////////////////////////////////////////////////
//
//    RENDERTARGET.H
//
//    Author:      Rehno Lindeque
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Used to specify a render target and attach input and output streams

    DEPENDENCIES:
      + glee.h, gl.h, types.h must be included before this header
      + GlStream and its subclasses
      + GlKernel

    CHANGES:
      2006-04-08 (Rehno Lindeque) - Split from stream.h
*/
/*                              COMPILER MACROS                             */
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
/*                          FORWARD DECLERATIONS                            */
class GlStream;
class GlInputStream;
class GlIOStream;
class GlKernel;

/*                                 CLASSES                                  */
class GlRenderTarget
{
protected:
  GLuint frameBufferObject;
  //bool active;
  //bool inputStreams[8];
public:
  inline GlRenderTarget();
  inline void create();
  inline void destroy();
  inline void bind();
  static inline void unbind();
};

#include "rendertarget.hxx"

}
#endif
