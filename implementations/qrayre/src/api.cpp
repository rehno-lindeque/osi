//////////////////////////////////////////////////////////////////////////////
//
//    API.CPP
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      QRayRE api implementation
*/
/*                              COMPILER MACROS                             */
#pragma warning(push)
#pragma warning(disable:4311) // 'type cast' : pointer truncation
#pragma warning(disable:4312) // 'type cast' : conversion to greater size

/*                                 INCLUDES                                 */
// Windows
#ifdef _WIN32
  #include <windows.h>
#endif

// OpenGL
#include <GL/GLee.h>
#include <GL/gl.h>

// QRayRE
#include "api.h"

using namespace QRayRE;
using namespace BaseRE;

/*                                  GLOBALS                                 */
#ifdef _WIN32
  std::stack<HDC> deviceContexts; // Windows
  std::stack<HGLRC> renderContexts; // OpenGL
  HDC oldHDC = null;
  HGLRC oldHRC = null;
  HWND hwnd;
#endif

RE_DISPLAY_MODE displayMode = RE_TEMPORARY;

/*                               IMPLEMENTATION                             */

OSobject reBeginScene()
{
  QRayScene* sceneObject = beginObject<QRayScene>();
  return cast_object(sceneObject);
}

void reEndScene()
{
  QRayScene* sceneObject = endObject<QRayScene>();
  sceneObject->build();
}

void reGeometryPool(OSobject geometryPool)
{
  QRayScene* sceneObject = getActiveObject<QRayScene>();
  sceneObject->geometryPool = geometryPool;
}

OSobject reBeginLight(RE_LIGHT type)
{
  QRayLight* lightObject = beginObject<QRayLight>();
  return cast_object(lightObject);
}

void reEndLight()
{
  QRayLight* lightObject = endObject<QRayLight>();
}

void reLightPosition(OSfloat x, OSfloat y, OSfloat z)
{
  QRayLight* lightObject = getActiveObject<QRayLight>();
  lightObject->position.set(x,y,z);
}

void reLightPosition(const OSfloat* position)
{
  QRayLight* lightObject = getActiveObject<QRayLight>();
  lightObject->position.set(position);
}

void reLightDirection(const OSfloat* direction)
{
  QRayLight* lightObject = getActiveObject<QRayLight>();
  //lightObject->direction.set(direction)
}

OSobject reBeginCamera(bool referenceFrame)
{
  QRayCamera* cameraObject = beginObject<QRayCamera>();
  return cast_object(cameraObject);
}

void reEndCamera()
{
  Camera* cameraObject = endObject<QRayCamera>();
}

void reCameraPosition(OSfloat x, OSfloat y, OSfloat z)
{
  QRayCamera* cameraObject = getActiveObject<QRayCamera>();
  cameraObject->position.set(x,y,z);
}

void reCameraPosition(const OSfloat* position)
{
  QRayCamera* cameraObject = getActiveObject<QRayCamera>();
  cameraObject->position.set(position);
}

void reCameraDirection(const OSfloat* direction)
{
  QRayCamera* cameraObject = getActiveObject<QRayCamera>();
  cameraObject->direction.set(direction);
}

OSobject reBeginRender(RE_RENDER_TARGET renderTarget, OSobject scene)
{
  QRayRender* renderObject = beginObject<QRayRender>();
  renderObject->scene = cast_id<Scene>(scene);
  switch(displayMode)
  {
  case RE_TEMPORARY:
    break;
  case RE_PERSISTENT:
    break;
  case RE_INCREMENTAL:
    break;
  }
  return cast_object(renderObject);
}

void reEndRender()
{
  endObject<QRayRender>();
}

void reCamera(OSobject camera)
{
  QRayRender* renderObject = getActiveObject<QRayRender>();
  renderObject->camera = cast_id<Camera>(camera);
}

void reLight(OSobject light)
{
  QRayScene& sceneObject = *getActiveObject<QRayScene>();
  sceneObject.addLight(cast_id<QRayLight>(light));
}

void reParameter(RE_PARAMETER type, OSid param)
{
  Render* renderObject = getActiveObject<Render>();
  //todo: renderObject->setParameter();
}

void reQuality(RE_PARAMETER type, OSid param, float quality)
{
  //todo
}

void reParameterQuality(RE_PARAMETER type, OSid param, OSint quality)
{
  //todo
}

void reParameterQuality(RE_PARAMETER type, OSid param, float quality)
{
  //todo
}

const REParameterInfo* reGetParameters(RE_PARAMETER type, int& num)
{
  switch(type)
  {
  case RE_GLOBAL_ILLUMINATION:
    //todo: (See GLRasRE for an example)
    num = 0;
    return null;
  case RE_LOCAL_ILLUMINATION:
    //todo
    num = 0;
    return null;
  default:
    num = 0;
    return null; //error
  }
}

const RESettingInfo* reGetParameterQualitySettings(RE_PARAMETER type, OSid param, int& num)
{
  switch(type)
  {
  case RE_GLOBAL_ILLUMINATION:
    //todo: (See GLRasRE for an example)
    /*
    switch(param)
    {
    case RE_AMBIENT_OCCLUSION:

      num = 0;
      return null;
    default:
      num = 0;
      return null; //error
    }*/
  case RE_LOCAL_ILLUMINATION:
    //todo
    num = 0;
    return null;
  default:
    num = 0;
    return null; //error
  }
}

void reRender(OSobject render)
{
  // initialize variables
  QRayRender* renderObject = cast_id<QRayRender>(render);
  if(renderObject == null)
    return; //error

  // render scene
  renderObject->render();
}

// misc
void reDebugRender(OSobject object, OSuint flag)
{
  //todo
}

void reInit()
{
  // Set up glee
  //GLeeInit();

  // Set up opengl
  glViewport(0, 0, 512, 512);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void reShutdown()
{
 /* triangleStream.destroy();*/

#ifdef _WIN32
  // Restore previous OpenGL Render Contex and GDI Device Context (needed?)
  wglMakeCurrent(oldHDC, oldHRC);

  // Destroy all other contexts
  while(!renderContexts.empty())
  {
    wglDeleteContext(renderContexts.top());
    renderContexts.pop();
  }
  while(!renderContexts.empty())
  {
    ReleaseDC(hwnd, deviceContexts.top());
    deviceContexts.pop();
  }
#endif
}

void reDisplayMode(RE_DISPLAY_MODE displayMode)
{
  ::displayMode = displayMode;
}

void reDisplay()
{
  switch(displayMode)
  {
  case RE_TEMPORARY:
    // Swap display buffers
#ifdef _WIN32
    if(deviceContexts.empty())
      return; //error
    SwapBuffers(deviceContexts.top());
#endif
    break;

  case RE_INCREMENTAL:

  case RE_PERSISTENT:
    // Swap display buffers
#ifdef _WIN32
    if(deviceContexts.empty())
      return; //error
    SwapBuffers(deviceContexts.top());
#endif
    break;
  }
}

RErc reCopyRenderContext(RErc originalRC)
{
  /*todo: see GLRayRE*/
  return (RErc)null;
}

void reBackgroundColor(OSfloat* color)
{
  //todo
}

void reDelObject(OSobject object)
{
  delObject((Object*)object);
}

#ifdef _WIN32
RErc wreCreateRenderContext(HWND hwnd)
{
  /* PFD_SWAP_EXCHANGE flag alows the back buffer to be swapped rather than copied
     Also, this is important for SLI cards (see GPU_Programming_Guide.pdf by nvidia) */

  // Store the original contexts (needed?)
  oldHRC = wglGetCurrentContext();
  oldHDC = wglGetCurrentDC();
  ::hwnd = hwnd;

  // Get & store the window's Device Context
  HDC hDC = ::GetDC(hwnd);
  if(!hDC)
    return null;
  deviceContexts.push(hDC);

  // Set up an appropriate pixel format
  int pixelFormat;
  static PIXELFORMATDESCRIPTOR PFD =
  {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW |
    PFD_SUPPORT_OPENGL |
    PFD_SWAP_EXCHANGE | //PFD_SWAP_COPY
    PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,
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

  if(!(pixelFormat = ChoosePixelFormat(hDC, &PFD)))
  {
    ReleaseDC(hwnd, hDC);
    deviceContexts.pop();
    return null;
  }
  if(!SetPixelFormat(hDC, pixelFormat, &PFD))
  {
    ReleaseDC(hwnd, hDC);
    deviceContexts.pop();
    return null;
  }

  // Set up an appropriate pixel format

  HGLRC hRC;
  if(!(hRC = wglCreateContext(hDC)))
  {
    ReleaseDC(hwnd, hDC);
    deviceContexts.pop();
    return null;
  }
  if(!(wglMakeCurrent(hDC, hRC)))
  {
    wglMakeCurrent(oldHDC, oldHRC);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
    deviceContexts.pop();
    return null;
  }
  renderContexts.push(hRC);
  return (RErc)hRC;
}
#endif

/*                              COMPILER MACROS                             */
#pragma warning(pop)
