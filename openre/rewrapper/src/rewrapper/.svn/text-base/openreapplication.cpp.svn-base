//////////////////////////////////////////////////////////////////////////////
//
//    OPENREAPPLICATION.CPP
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include <windows.h>

#include <osi/re/re.h>

#include "common/types.h"

#include "openreapplication.h"

using namespace REWrapper;

OpenREApp *globalApp = null;

OpenREApp::OpenREApp(const char *appName)
{
  endFlag = false;
  globalApp = this;
  window.setWindowName(appName);
}

OpenREApp::~OpenREApp()
{
  globalApp = null;
}

void OpenREApp::exit()
{
  endFlag |= true;
}

bool OpenREApp::osMain(int &r)
{
  bool endMessage = window.messageHandler(r);

  if(!(endFlag || endMessage))
  {
    main();

    /* todo?
    if(window.active)
      SwapBuffers(window.hDC);*/
  }
  else
  {
    PostQuitMessage(0);
  }
  endFlag |= endMessage;

  return !(endFlag);
}

void OpenREApp::updateDisplayMode(/*HINSTANCE hInstance*/)
{
  if(displayMode.fullscreen)
  {
    DEVMODE ScreenMode;
    memset(&ScreenMode, 0, sizeof(ScreenMode));
    ScreenMode.dmSize       = sizeof(ScreenMode);
    ScreenMode.dmPelsWidth  = displayMode.width;
    ScreenMode.dmPelsHeight = displayMode.height;
    ScreenMode.dmBitsPerPel = displayMode.bpp;
    ScreenMode.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    if(ChangeDisplaySettings(&ScreenMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
      displayMode.fullscreen = false;
  }
  if(!displayMode.fullscreen)
    ChangeDisplaySettings(null, 0);

  window.style |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

  if(displayMode.fullscreen)
  {
    window.cursorVisible(false);
    if(window.initFlag)
    {
      window.style   &= ~WS_OVERLAPPEDWINDOW;
      window.exStyle &= ~WS_EX_WINDOWEDGE;
    }
    window.style   |= WS_POPUP;
    window.exStyle |= WS_EX_APPWINDOW;
    window.size(0,0,displayMode.width,displayMode.height);
  }
  else
  {
    if(window.initFlag)
      window.style &= ~WS_POPUP;
    window.style   |= WS_OVERLAPPEDWINDOW;
    window.exStyle |= (WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

    DEVMODE screenMode;
    EnumDisplaySettings(null, ENUM_CURRENT_SETTINGS, &screenMode);
    uint xOffset = (screenMode.dmPelsWidth-displayMode.width)>>1;
    uint yOffset = (screenMode.dmPelsHeight-displayMode.height)>>1;

    //old window.size(0,0,displayMode.width,displayMode.height);
    window.size(xOffset, yOffset, xOffset+displayMode.width, yOffset+displayMode.height);
  }

  if(window.initFlag)
  {
    window.resize();
    if(!displayMode.fullscreen)
      InvalidateRect(null, null, false);
  }
  else
    window.init();

  /*int pixelFormat;
  static PIXELFORMATDESCRIPTOR PFD =
  {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW |
    PFD_SUPPORT_OPENGL |
    PFD_SWAP_EXCHANGE | //PFD_SWAP_COPY
    PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    displayMode.bpp,
    0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0, 0, 0, 0,
    16,
    0,
    0,
    PFD_MAIN_PLANE,
    0,
    0, 0, 0
  };

  if(!(pixelFormat = ChoosePixelFormat(window.hDC, &PFD))) return;
  if(!SetPixelFormat(window.hDC, pixelFormat, &PFD)) return;*/

  if(!wreCreateRenderContext(window.hWnd)) return;


  //todo: if(!(hRE = wreMakeCurrent(window.hDC))) return;

  /*if(!(hRC = wglCreateContext(window.hDC))) return;
  if(!wglMakeCurrent(window.hDC, hRC)) return;*/
}

void setupOpenRE()
{
  //todo

  /*glShadeModel(GL_SMOOTH);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  //glClearDepth(1.0f);?
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glViewport(0, 0, 800, 600);

  //Setup Viewport for 2D drawing
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glScalef((float)1/400.0f, (float)1/300.0f, 1.0f);
  //glTranslatef(-400.0f, -300.0f, 0.0f);

  //gluPerspective(45.0f, (GLfloat)displayMode.width / (GLfloat)displayMode.height, 0.1, 100.0); //todo : make customizable
  */
}

void OpenREApp::initInternal()
{
  displayMode.bpp = 32;
  displayMode.fullscreen = false;
  displayMode.width = 800;
  displayMode.height = 600;
  updateDisplayMode(); //calls window.init()

  setupOpenRE();
  //initREExtensions(); //???
}

void OpenREApp::destroyInternal()
{
  window.cursorVisible(true);
  if(displayMode.fullscreen)
    ChangeDisplaySettings(null, 0);
  /*if(hRC)
  {
    if(!wglMakeCurrent(null, null));
    if(!wglDeleteContext(hRC));
    hRC = null;
  }*/

  window.destroy();
}

int reMain(int nArguments, char** arguments)
{
  if(globalApp == null)
    return 0;

  globalApp->commandLine.nArguments = nArguments;
  globalApp->commandLine.arguments = arguments;

  int r = 0;
  globalApp->initInternal();
  globalApp->init();
  while(globalApp->osMain(r));
  globalApp->end();
  globalApp->destroyInternal();

  return r;
}

#ifdef _WIN32
  int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR caCmdLine, int iCmdShow)
  {
    return reMain(0, &caCmdLine);
  }
#else
  int main(int argv, char** argc)
  {
    return reMain(argv, argc);
  }
#endif
