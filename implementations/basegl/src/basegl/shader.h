#ifndef __BASERE_SHADER_H__
#define __BASERE_SHADER_H__
//////////////////////////////////////////////////////////////////////////////
//
//    SHADER.H
//
//    Author:      Rehno Lindeque
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      GPU shaders

    DEPENDENCIES:
      glee.h, gl.h, types.h must be included before this header

    CHANGES:
      2006-04-08 (Rehno Lindeque) - Split from stream.h
*/
/*                              COMPILER MACROS                             */
#ifndef __glee_h_
  #error("glee.h must be included before shader.h")
#endif
#if !(defined(__GL_H__) || defined(__gl_h__))
  #error("gl.h must be included before shader.h")
#endif
#ifndef __BASERE_TYPES_H__
  #error("types.h must be included before shader.h")
#endif

/*                                 INCLUDES                                 */
#include <vector>
using namespace std;

namespace BaseRE
{
/*                          FORWARD DECLERATIONS                            */
/*                                 CLASSES                                  */

class GlShader
{
protected:
  GLuint programObject;
  GLuint fragmentShaderObject;
  GLuint vertexShaderObject;
  static char shaderLog[512];
  //static vector<GLuint> attributeBuffers;

public:
  inline GlShader();

  /* TODO: Change this so that many source code fragments can be linked together */
  void build(const char* shaderProgram, const char* shaderVertexProgram);
  inline const char* getErrors();
  //void execute();
  void destroy();
  //void attachAttributes(const char* attributeName, const void* attributes, uint attributeElements, GLuint attributeType = GL_FLOAT, bool normalized = false); // naming: normalized \ fixedpoint???
  void setUniform1i(const char* inputName, GLuint value);
  void setUniform1f(const char* inputName, GLfloat value);
  GLuint getAttribute(const char* attributeName);

  inline GLuint bind();
  static inline void unbind();
};

#include "shader.hxx"
}
#endif
