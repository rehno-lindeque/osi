//////////////////////////////////////////////////////////////////////////////
//
//    SHADER.CPP
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
#include "shader.h"
using namespace BaseRE;

/*                                  GLOBALS                                 */
char GlShader::shaderLog[512];
//vector<GLuint> GlShader::attributeBuffers;

/*                              IMPLEMENTATION                              */
void GlShader::build(const char* shaderProgram, const char* shaderAttributeProgram)
{  
  if(shaderProgram == null)
    return; //error
  programObject = glCreateProgram();
  if(shaderAttributeProgram)
  {
    vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderObject, 1, (const GLchar**)&shaderAttributeProgram, null);
    glCompileShader(vertexShaderObject);
    glAttachShader(programObject, vertexShaderObject);
  }

  fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);        
  glShaderSource(fragmentShaderObject, 1, (const GLchar**)&shaderProgram, null);        
  glCompileShader(fragmentShaderObject);
  glAttachShader(programObject, fragmentShaderObject);    
  glLinkProgram(programObject);
  
  //glDeleteShader(fragmentShaderObject);
  //glDeleteShader(vertexShaderObject);

  GLsizei len;
  if(shaderAttributeProgram)
    glGetShaderInfoLog(vertexShaderObject, 512, &len, shaderLog);
  glGetShaderInfoLog(fragmentShaderObject, 512, &len, shaderLog);    
  //invalid?? glGetShaderInfoLog(programObject, 512, &len, shaderLog);
}

void GlShader::destroy()
{
  //unbind()
  glDeleteProgram(programObject);
}

/*void GlShader::attachAttributes(const char* attributeName, const void* attributes, uint attributeElements, GLuint attributeType, bool normalized)
{
  //glUseProgram(programObject);
  GLuint attributetextureObject = glGetAttribLocation(programObject, attributeName);
  glVertexAttribPointer(attributetextureObject, attributeElements, attributeType, normalized, 0, attributes);
  glEnableVertexAttribArray(attributetextureObject);
  attributeBuffers.push_back(attributetextureObject);
}*/

void GlShader::setUniform1i(const char* inputName, GLuint value)
{
  //glUseProgram(programObject);
  GLuint samplerUniformLocation = glGetUniformLocation(programObject, inputName);
  glUniform1i(samplerUniformLocation, value);
}

void GlShader::setUniform1f(const char* inputName, GLfloat value)
{
  GLuint floatUniformLocation = glGetUniformLocation(programObject, inputName);
  glUniform1f(floatUniformLocation, value);
}

GLuint GlShader::getAttribute(const char* attributeName)
{
  return glGetAttribLocation(programObject, attributeName);

}

/*                              COMPILER MACROS                             */
#pragma warning(pop)
