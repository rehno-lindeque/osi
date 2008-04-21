//////////////////////////////////////////////////////////////////////////////
//
//    API.CPP
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      LinGLKernel api implementation

    TODO:
      Rename to LinXGLKernel?
*/
/*                              COMPILER MACROS                             *
#pragma warning(push)
#pragma warning(disable:4311) // Type cast: Pointer truncation
#pragma warning(disable:4312) // Type cast: Conversion to greater size (pointer)
#pragma warning(disable:4390) // Empty controlled statement
#pragma warning(disable:4244) // Conversion from float to int (possible loss of data)

/*                                 INCLUDES                                 */
#include "api.h"

// OpenGL
#include <GL/gl.h>
#include <GL/glx.h>

// Xorg
//#include <X11/extensions/xf86vmode.h>

// STL headers
#include <vector>

/*                              COMPILER MACROS                             */
#define _this (*(LinGLKernel::Kernel*)this)

#define cast_id     BaseKernel::Kernel::cast_id
#define cast_object BaseKernel::Kernel::cast_object

/*                                  GLOBALS                                 */
/*std::vector<DISPLAY_DEVICE> displayDevices;*/
std::vector<LinGLKernelDisplayObject*> displayObjects;
std::vector<LinGLKernel::DisplayChain::DisplayInstance> displayChainList;

/*HGLRC oldHRC;
HDC oldHDC;
HINSTANCE hInstance;*/

/*                               IMPLEMENTATION                             */

/*HGLRC createRenderContext(HWND hWnd)
{
  /* PFD_SWAP_EXCHANGE flag alows the back buffer to be swapped rather than copied
     Also, this is important for SLI cards (see GPU_Programming_Guide.pdf by nvidia) *

  // Store the original contexts (needed?)
  oldHRC = wglGetCurrentContext();
  oldHDC = wglGetCurrentDC();

  // Get & store the window's device context
  HDC hDC = ::GetDC(hWnd);

  if(!hDC)
    return null;

  // Choose an appropriate pixel format
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
    ReleaseDC(hWnd, hDC);
    return null;
  }

  if(!SetPixelFormat(hDC, pixelFormat, &PFD))
  {
    ReleaseDC(hWnd, hDC);
    return null;
  }

  // Create rendering context
  HGLRC hRC;

  if(!(hRC = wglCreateContext(hDC)))
  {
    ReleaseDC(hWnd, hDC);
    return null;
  }

  if(!(wglMakeCurrent(hDC, hRC)))
  {
    wglMakeCurrent(oldHDC, oldHRC);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
    return null;
  }

  return hRC;
}
*/

OSobject OSI_API_CALL Kernel::beginDisplay(OSuint displayNumber, const OSchar* name, OSbool fullScreen, OSuint width, OSuint height)
{
  // Open the default X display (todo: extend this to use the display name for number...)
  Display* display = XOpenDisplay(null);
  if(display == null)
    return 0; // error: Couldn't open an X server connection

  // Get the default screen number of the display
  int screen = DefaultScreen(display);

  // Test the GLX implementation version
  // todo: change storage of glx versions since it's apparently display dependent
  if(!glXQueryExtension(display, null, null))
    return 0; // error: GLX is not available

  if(!glXQueryVersion(display, &_this.glxVersionMajor, &_this.glxVersionMinor))
    return 0; // error

  if(_this.glxVersionMajor < 1 || _this.glxVersionMinor < 2)
    return 0; //error: unsupported version of GLX

  // Create GLX window on display device
  if(_this.glxVersionMinor == 2)
    return _this.createWindowGLX1_2(display, screen, name, fullScreen, width, height);
  else
    return _this.createWindowGLX1_3(display, screen, name, fullScreen, width, height);

  // Complete all X commands before continueing
  glXWaitX();
}

OSobject OSI_API_CALL LinGLKernel::Kernel::createWindowGLX1_2(::Display* display, int screen, const OSchar* name, OSbool fullScreen, OSuint width, OSuint height)
{
  //todo
}

OSobject OSI_API_CALL LinGLKernel::Kernel::createWindowGLX1_3(::Display* display, int screen, const OSchar* name, OSbool fullScreen, OSuint width, OSuint height)
{
  //// Select an appropriate frame buffer configuration
  // (todo: this can become quite involved... To get a list of all FBConfigs use GetFBConfigs)
  // (todo: this could this be moved to openre for proper creation...?)
  static const int desiredAttributes[] = {
    /*GLX_DOUBLEBUFFER, true,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_ALPHA_SIZE, 8,
    GLX_DEPTH_SIZE, 16,
    //GLX_RENDER_TYPE, GLX_RGBA,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,*/
    None };

  int nConfigs;
  GLXFBConfig* configs = glXChooseFBConfig(display, screen, desiredAttributes, &nConfigs);
  if(!configs || nConfigs < 1)
    return 0; // error: no appropriate screen configuration found

  // Get the appropriate visual info corresponding to the best one of the selected frambebuffer configurations
  // (todo: this should be user specified...)
  XVisualInfo* visualInfo = glXGetVisualFromFBConfig(display, configs[0]);
  if(visualInfo == null)
    return 0; // error: could not retrieve visual info from frame buffer configuration (todo: try some others?)

  //visualInfo->visualid

  //// Create an opengl rendering context
  // (todo: no share list is specified at the moment but this may be necessary for multiple contexts)
  // note: We do this before creating the window in order to have an early short-circuit if a context can't be created.
  //       (I.e. this is better error handling)
  GLXContext glxContext = glXCreateNewContext(display, configs[0], GLX_RGBA_TYPE, null, GL_TRUE);
  //GLXContext glxContext = glXCreateContext(display, visualInfo, null, GL_TRUE);
  if(glxContext == null)
    return 0; // error: could not create context

  if(!glXIsDirect(display, glxContext)); //Warning: Rendering context does not support direct rendering.

  //// Create the glx window (fullscreen / windowed)
  XSetWindowAttributes attr;
  memset(&attr, 0, sizeof(attr));
  //todo: attr.colormap = getColorMap() ...
  attr.override_redirect = true; // todo: What exactly does this do? Can't find documentation on it.
  attr.event_mask =
    KeyPressMask
    | KeyReleaseMask
    | ButtonPressMask
    | ButtonReleaseMask
    | EnterWindowMask
    | LeaveWindowMask
    | PointerMotionMask
    | PointerMotionHintMask
    | Button1MotionMask
    | Button2MotionMask
    | Button3MotionMask
    | Button4MotionMask
    | Button5MotionMask
    | ButtonMotionMask
    | KeymapStateMask
    | ExposureMask
    | VisibilityChangeMask
    | StructureNotifyMask
    | ResizeRedirectMask
    | SubstructureNotifyMask
    | SubstructureRedirectMask
    | FocusChangeMask
    | PropertyChangeMask
    | ColormapChangeMask
    | OwnerGrabButtonMask;

  Window window = 0;

  //Check for XF86VIDMODE extension (native fullscreen mode extension)
  int majorOpcode, firstEventCode, firstErrorCode;
  if(fullScreen && XQueryExtension(display, "XFree86-VidModeExtension", &majorOpcode, &firstEventCode, &firstErrorCode))
  {
    window = XCreateWindow(display,
                           RootWindow(display, visualInfo->screen),
                           0, 0, width, height,
                           0,
                           visualInfo->depth,
                           InputOutput,
                           visualInfo->visual,
                           0
                           //| CWBorderPixel
                           //| CWColormap
                           | CWEventMask
                           | CWOverrideRedirect,
                           &attr);
    //todo: XSetWindowBackround?
    //todo: XSelectInput?
  }
  else
  {
    window = XCreateWindow(display,
                           RootWindow(display, visualInfo->screen),
                           0, 0, width, height,
                           0,
                           visualInfo->depth,
                           InputOutput,
                           visualInfo->visual,
                           0
                           //| CWBorderPixel
                           //| CWColormap
                           | CWEventMask
                           | CWOverrideRedirect,
                           &attr);

    //todo: to set additional window properties, use XmbSetWMProperties
  }

  GLXWindow glxWindow = glXCreateWindow(display, configs[0], window, null);

  glXMakeCurrent(display, glxWindow, glxContext);

  // Select XWindows events to handle
  //XSelectInput(display, window, attr.event_mask);

    /*XSelectInput(display, window,
      KeyPressMask
    | KeyReleaseMask
    | ButtonPressMask
    | ButtonReleaseMask
    | EnterWindowMask
    | LeaveWindowMask
    | PointerMotionMask
    | PointerMotionHintMask
    | Button1MotionMask
    | Button2MotionMask
    | Button3MotionMask
    | Button4MotionMask
    | Button5MotionMask
    | ButtonMotionMask
    | KeymapStateMask
    | ExposureMask
    | VisibilityChangeMask
    | StructureNotifyMask
    | ResizeRedirectMask
    | SubstructureNotifyMask
    | SubstructureRedirectMask
    | FocusChangeMask
    | PropertyChangeMask
    | ColormapChangeMask
    | OwnerGrabButtonMask);*/

  // Select GLX events to handle
  //todo: glXSelectEvent(Display *dpy, GLXDrawable draw, unsigned long event mask);

//XMotionEvent
//XMaskEvent
//XKeymapEvent
//XKeyPressedEvent
//XReconfigureWindow

  // Map the window to the display
  // (Temporary: is this really necessary & why?)
  XMapWindow(display, window);

  // Set the window to focussed so that it can capture keyboard input
  XSetInputFocus(display, window, RevertToParent, CurrentTime);

  //// Free temporary variables
  XFree(configs);
  XFree(visualInfo);

  //// Create display object
  LinGLKernelDisplayObject& displayObject = *new LinGLKernelDisplayObject;

  displayObject.xDisplay = display;
  displayObject.glxDrawable = glxWindow;
  displayObject.glxContext = glxContext;
  displayObject.x1 = 0;
  displayObject.y1 = 0;
  displayObject.x2 = width;
  displayObject.y2 = height;

  displayObjects.push_back(&displayObject);

  //todo: busy here... (create a LinGLKernel::Display)
  return (OSobject)&displayObject;
}

void OSI_API_CALL Kernel::endDisplay()
{
}

OSobject OSI_API_CALL Kernel::beginDisplayChain()
{
  LinGLKernel::DisplayChain* displayChainObject = _this.beginObject<LinGLKernel::DisplayChain>();
  displayChainList.reserve(10);
  return cast_object(displayChainObject);
}

void OSI_API_CALL Kernel::endDisplayChain()
{
  LinGLKernel::DisplayChain& displayChainObject = *_this.endObject<LinGLKernel::DisplayChain>();

  displayChainObject.nDisplayInstances = (OSuint32)displayChainList.size();
  displayChainObject.displayInstances = new LinGLKernel::DisplayChain::DisplayInstance[displayChainObject.nDisplayInstances];

  // Note: This can be done more efficiently by copying the entire vector at once
  //for(std::vector<WinGLKernel::DisplayChain::DisplayInstance>::iterator i = displayChainList.begin(); i != displayChainList.end(); ++i)
  for(uint c = 0; c < displayChainObject.nDisplayInstances; ++c)
    memcpy(&displayChainObject.displayInstances[c], &displayChainList[c], sizeof(LinGLKernel::DisplayChain::DisplayInstance));

  displayChainList.clear();
}

void OSI_API_CALL Kernel::display(OSobject display, OSfloat x1, OSfloat y1, OSfloat x2, OSfloat y2)
{
  displayChainList.resize(displayChainList.size()+1);
  LinGLKernel::DisplayChain::DisplayInstance& displayInstance = displayChainList.back();
  displayInstance.display = display;
  displayInstance.x1 = x1;
  displayInstance.y1 = y1;
  displayInstance.x2 = x2;
  displayInstance.y2 = y2;
}

void OSI_API_CALL Kernel::refreshDisplay(OSobject display)
{
  if(display == 0)
  {
    // Refresh all displays
    for(std::vector<LinGLKernelDisplayObject*>::iterator i = displayObjects.begin(); i != displayObjects.end(); ++i)
    {
      LinGLKernelDisplayObject& displayObject = **i;
      LinGLKernel::Display::update(displayObject);
    }
  }
  else
  {
    // Refresh display
    LinGLKernelDisplayObject& displayObject = *(LinGLKernelDisplayObject*)display;
    LinGLKernel::Display::update(displayObject);
  }
}

OSobject OSI_API_CALL Kernel::beginFrame(OSobject displayChain, OSobject parentFrame)
{
  LinGLKernel::Frame* frameObject = _this.beginObject<LinGLKernel::Frame>();
  return cast_object(frameObject);
}

void OSI_API_CALL Kernel::endFrame()
{
  LinGLKernel::Frame* frameObject = _this.endObject<LinGLKernel::Frame>();
}

void OSI_API_CALL Kernel::openFrame(OSobject frame)
{
  LinGLKernel::Frame* frameObject = _this.openObject<LinGLKernel::Frame>(frame);
}

void OSI_API_CALL Kernel::closeFrame()
{
  LinGLKernel::Frame* frameObject = _this.closeObject<LinGLKernel::Frame>();
}

void OSI_API_CALL Kernel::frameArea(OSfloat x1, OSfloat y1, OSfloat x2, OSfloat y2)
{
  LinGLKernel::Frame& frameObject = *_this.getActiveObject<LinGLKernel::Frame>();
  frameObject.setArea(x1, y1, x2, y2);
}

void OSI_API_CALL Kernel::loadDynamicLibrary(OSstring path)
{
  //todo: return OSobject?
  //todo
}

void* OSI_API_CALL Kernel::getIntKernel()
{
  /*return _this.intKernel;*/
}

const OSstring OSI_API_CALL Kernel::getVersion()
{
  return KERNEL_VERSION;
}

void OSI_API_CALL Kernel::shutdown()
{
  /*// Remove all remaining messages from the message queue
  {
	  MSG msg;
	  while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		  DispatchMessage(&msg);
  }*/

  // Destroy displays
  uint c = 0;
  for(std::vector<LinGLKernelDisplayObject*>::iterator i = displayObjects.begin(); i != displayObjects.end(); ++i)
  {
    LinGLKernelDisplayObject& displayObject = **i;

    // Release the current glx context
    glXMakeCurrent(displayObject.xDisplay, None, NULL);

    // Destroy render contexts
    glXDestroyContext(displayObject.xDisplay, displayObject.glxContext);
    displayObject.glxContext = 0;

    // Destroy  glx variables
    glXDestroyWindow(displayObject.xDisplay, displayObject.glxDrawable);
    XCloseDisplay(displayObject.xDisplay);

    // Free display object memory
    delete &displayObject;
  }
  displayObjects.clear();
}

const void* OSI_API_CALL IntKernel::openClipFrames(OSobject displayChain, OSobject frame)
{
  /*int intKernelOffset = (int)&(((WinGLKernel::Kernel*)null)->intKernel);
  WinGLKernel::Kernel* kernel = (WinGLKernel::Kernel*)(((uint8*)(WinGLKernel::IntKernel*)this) - intKernelOffset);*

  static OSuint8 buffer[sizeof(OSuint32) + sizeof(IntKernel::ClipFrame)*10];

  OSuint32& bufferSize = *(OSuint32*)buffer;
  IntKernel::ClipFrame* clipFrames = (IntKernel::ClipFrame*)(buffer+sizeof(OSuint32));

  WinGLKernel::DisplayChain& displayChainObject = *cast_id<WinGLKernel::DisplayChain>(displayChain);
  WinGLKernel::Frame&        frameObject        = *cast_id<WinGLKernel::Frame>(frame);


  // Test for frame intersection with all displays in the display chain
  for(uint c = 0; c < displayChainObject.nDisplayInstances; ++c)
  {
    WinGLKernel::DisplayChain::DisplayInstance& displayInstance = displayChainObject.displayInstances[c];
    IntKernel::ClipFrame& clipFrame = clipFrames[c];

    // Test intersection
    if(  frameObject.area.x1 > displayInstance.x2
      || frameObject.area.x2 < displayInstance.x1
      || frameObject.area.y1 > displayInstance.y2
      || frameObject.area.y2 < displayInstance.y1)
      continue;

    //// Calculate clip frame
    // Retrieve the display's "pixel" space determinants
    WinGLKernelDisplayObject& displayObject = *(WinGLKernelDisplayObject*)displayInstance.display;

    OSfloat xDet = (displayObject.x2 - displayObject.x1) / (displayInstance.x2 - displayInstance.x1);
    OSfloat yDet = (displayObject.y2 - displayObject.y1) / (displayInstance.y2 - displayInstance.y1);

    // Get clipped coordinates (and translate to unnormalized display space)
    /*OSfloat x1 = max(frameObject.area.x1, displayInstance.x1),
            y1 = max(frameObject.area.y1, displayInstance.y1),
            x2 = min(frameObject.area.x2, displayInstance.x2),
            y2 = min(frameObject.area.y2, displayInstance.y2);*

    OSfloat x1 = max(frameObject.area.x1 - displayInstance.x1, 0.0f),
            y1 = max(frameObject.area.y1 - displayInstance.y1, 0.0f),
            x2 = min(frameObject.area.x2, displayInstance.x2) - displayInstance.x1,
            y2 = min(frameObject.area.y2, displayInstance.y2) - displayInstance.y1;

    // Convert to pixel coordinates
    clipFrame.display = displayInstance.display;
    clipFrame.x1 = (OSuint)x1*xDet;
    clipFrame.y1 = (OSuint)y1*yDet;
    clipFrame.x2 = (OSuint)x2*xDet;
    clipFrame.y2 = (OSuint)y2*yDet;
  }

  return buffer;*/
}

void OSI_API_CALL IntKernel::closeClipFrames()
{
}

const void* OSI_API_CALL IntKernel::openDisplays(OSobject displayChain)
{
  /*static OSuint8 buffer[sizeof(OSuint32) + sizeof(OSobject)*10];

  OSuint32& bufferSize = *(OSuint32*)buffer;
  OSobject* displays = (OSobject*)(buffer+sizeof(OSuint32));

  WinGLKernel::DisplayChain& displayChainObject = *cast_id<WinGLKernel::DisplayChain>(displayChain);

  bufferSize = displayChainObject.nDisplayInstances;

  for(uint c = 0; c < bufferSize; ++c)
    displays[c] = displayChainObject.displayInstances[c].display;

  return buffer;*/
}

void OSI_API_CALL IntKernel::closeDisplays()
{
}

Kernel* OSI_API_CALL kernelInit()
{
  /*// Get module instance
  hInstance = GetModuleHandle(null);

  // Get all display devices
  DISPLAY_DEVICE displayDevice;

  int c = 0;
  while(true)
  {
    displayDevice.cb = sizeof(DISPLAY_DEVICE);

    if(EnumDisplayDevices(null, c, &displayDevice, 0) == 0)
      break;

    if(displayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)
    {
      if(displayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
      {
        if(displayDevices.empty())
          displayDevices.push_back(displayDevice);
        else
          memcpy(&displayDevices.front(), &displayDevice, sizeof(displayDevice));
      }
      else
      {
        if(displayDevices.empty())
        {
          DISPLAY_DEVICE emptyDisplayDevice;
          memset(&emptyDisplayDevice, 0, sizeof(emptyDisplayDevice));
          displayDevices.push_back(emptyDisplayDevice);
        }
        displayDevices.push_back(displayDevice);
      }
    }

    ++c;
  }*/

  // Create kernel object
  LinGLKernel::Kernel* kernelObject = new LinGLKernel::Kernel;
  kernelObject->intKernel = new LinGLKernel::IntKernel;

  return (::Kernel*)kernelObject;
}

/*                              COMPILER MACROS                             */
//#pragma warning(pop)
