//////////////////////////////////////////////////////////////////////////////
//
//    MAIN.CPP
//
//    Copyright (c) 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      A NativeGE unit test
*/
/*                                  INCLUDES                                */

#ifdef _WIN32
  #include <windows.h>
#endif

/* Base */
#include <base/common/types.h>

/* OpenKernel */
#include <osi/kernel/kernel.hpp>

/* OpenGL */
#include <GL/gl.h>

/*                              COMPILER MACROS                             */
#pragma warning(push)
#pragma warning(disable: 4305)

/*                                  CLASSES                                 */
class KernelTest
{
protected:
  Kernel* kernel;

  OSobject displayChain;
  OSobject mainDisplay;

public:

  virtual void init()
  {
    // Initialize OpenKernel
    kernel = kernelInit();

    // Open window
    mainDisplay = kernel->beginDisplay(0, "Main", true, 200, 150);
    kernel->endDisplay();
    if(mainDisplay == 0)
      return;

    displayChain = kernel->beginDisplayChain();

      kernel->display(mainDisplay, -1.0f, -1.0f, 1.0f, 1.0f);

    kernel->endDisplayChain();

    //(resize screen)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, 200, 150);
    glFrustum(-1.f, 1.f, -800.0f/600.0f, 800.0f/600.0f, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glFlush();
  }

  virtual bool main()
  {
    if(mainDisplay == 0)
      return false;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-1.5f, 0.0f, -6.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();
    glTranslatef(3.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
    glEnd();

    kernel->refreshDisplay();
    return true;
  }

  virtual void end()
  {
    kernel->shutdown();
  }

  KernelTest() : kernel(0), mainDisplay(0), displayChain(0) {}
}application;

int main()
{
  application.init();
  while(application.main()) {}
  application.end();
  return 0;
}

/*                              COMPILER MACROS                             */
#pragma warning(pop)
