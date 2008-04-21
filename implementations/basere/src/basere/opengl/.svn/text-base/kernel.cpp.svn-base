//////////////////////////////////////////////////////////////////////////////
//
//    KERNEL.CPP
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
#include "kernel.h"
using namespace BaseRE;

/*                                  GLOBALS                                 */
/*                              IMPLEMENTATION                              */
void GlKernel::execute()
{

  //transform
  glMatrixMode(GL_PROJECTION);
  glViewport(0, 0, 512, 512);
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(-1.0f, -1.0f, 0.0f);
  glScalef(2.0f, 2.0f, 0.0f);

  float vertices[] = { 0.0f, 1.0f,
                       1.0f, 1.0f,
                       1.0f, 0.0f,
                       0.0f, 0.0f };
  float texcoords[] = { 0.0f, 1.0f,
                        1.0f, 1.0f,
                        1.0f, 0.0f,
                        0.0f, 0.0f };

  bind();
  //temp: glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

  glEnableClientState(GL_VERTEX_ARRAY);
  glClientActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
  glVertexPointer(2, GL_FLOAT, 0, vertices);   
  glDrawArrays(GL_QUADS, 0, 4);
  glDisableClientState(GL_VERTEX_ARRAY);

  // restore transform
  glPopMatrix();
  glViewport(0, 0, 800, 600);
  glMatrixMode(GL_MODELVIEW);
}

/*                              COMPILER MACROS                             */
#pragma warning(pop)
