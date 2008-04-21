#ifndef __BASERE_KERNEL_H__
#define __BASERE_KERNEL_H__
//////////////////////////////////////////////////////////////////////////////
//
//    KERNEL.H
//
//    Author:      Rehno Lindeque
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      GPU kernel for shader execution using streams.

    DEPENDENCIES:
      glee.h, gl.h, types.h, shader.h must be included before this header

    CHANGES:
      2006-04-08 (Rehno Lindeque) - Split from stream.h
*/
/*                              COMPILER MACROS                             */
#ifndef __glee_h_
  #error("glee.h must be included before kernel.h")
#endif
#if !(defined(__gl_h_) || defined(__GL_H__))
  #error("gl.h must be included before kernel.h")
#endif
#ifndef __BASE_TYPES_H__
  #error("types.h must be included before kernel.h")
#endif
#ifndef __BASERE_SHADER_H__
  #error("shader.h must be included before kernel.h")
#endif

/*                                 INCLUDES                                 */
#include <vector>
using namespace std;

namespace BaseRE
{
/*                          FORWARD DECLERATIONS                            */
/*                                 CLASSES                                  */

class GlKernel : public GlShader
{
public:
  void execute();
};

#include "kernel.hxx"
}
#endif
