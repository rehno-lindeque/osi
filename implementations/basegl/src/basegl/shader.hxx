#ifdef __BASERE_SHADER_H__
#ifndef __BASERE_SHADER_HXX__
#define __BASERE_SHADER_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    SHADER.HXX
//
//    Author:      Rehno Lindeque
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */

/*                               IMPLEMENTATION                             */
inline GlShader::GlShader() : programObject(0), fragmentShaderObject(0)
{}

inline const char* GlShader::getErrors()
{
  return shaderLog;
}

inline GLuint GlShader::bind()
{
  glUseProgram(programObject);
  return programObject;
}

inline void GlShader::unbind()
{
  /*for(vector<GLuint>::iterator i = attributeBuffers.begin(); i != attributeBuffers.end(); ++i)
    glDisableVertexAttribArray(*i);
  attributeBuffers.clear();*/
  glUseProgram(0);    
}

#endif
#endif
