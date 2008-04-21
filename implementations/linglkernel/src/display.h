#ifndef __LINGLKERNEL_DISPLAY_H__
#define __LINGLKERNEL_DISPLAY_H__
//////////////////////////////////////////////////////////////////////////////
//
//    DISPLAY.H
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      LinGLKernel Display class.
*/

//TEMPORARY:
#include <iostream>
using namespace std;

namespace LinGLKernel
{
/*                                  CLASSES                                 */
  class Display : public Base::Object
  {
  public:
    //todo: LinGLKernelDisplayObject displayObject;

    static void update(const LinGLKernelDisplayObject& displayObject)
    {
      handleEvents(displayObject);
      glXSwapBuffers(displayObject.xDisplay, displayObject.glxDrawable);
    }

  protected:
    static void handleEvents(const LinGLKernelDisplayObject& displayObject)
    {
      // Handle x windows events
      XEvent event;
      while (XPending(displayObject.xDisplay) > 0)
      {
        XNextEvent(displayObject.xDisplay, &event);

#ifdef _DEBUG
        cout << "EVENT: ";
#endif

        switch (event.type)
        {
        case Expose:
#ifdef _DEBUG //  TEMPORARY
          cout << "Expose" << endl;
#endif
          return;

        case ConfigureNotify:
          if(event.xconfigure.width != displayObject.x2 - displayObject.x1
            || event.xconfigure.height != displayObject.y2 - displayObject.y1)
          {
            //todo: (NB!) handle window resize
          }

#ifdef _DEBUG //  TEMPORARY
          cout << "Configure Notify" << endl;
#endif
          break;

        /*case ButtonPress:
          break;*/

        case KeyPress:
          if(XLookupKeysym(&event.xkey, 0) == XK_Escape)
          {
            //...
          }
#ifdef _DEBUG //  TEMPORARY
          cout << "Key press" << endl;
#endif
          break;

        case ClientMessage:
          /*if (*XGetAtomName(displayObject.xDisplay, event.xclient.message_type) == *"WM_PROTOCOLS")
          {
              todo: What is this? exit the program?
          }*/

#ifdef _DEBUG //  TEMPORARY
          cout << "Client Message" << endl;
#endif
          break;

        default:

        /*KeyPress
        KeyRelease
        ButtonPress
        ButtonRelease
        MotionNotify
        EnterNotify
        LeaveNotify
        FocusIn
        FocusOut
        KeymapNotify
        Expose
        GraphicsExpose
        NoExpose
        VisibilityNotify
        CreateNotify
        DestroyNotify
        UnmapNotify
        MapNotify
        MapRequest
        ReparentNotify
        ConfigureNotify
        ConfigureRequest
        GravityNotify
        ResizeRequest
        CirculateNotify
        CirculateRequest
        PropertyNotify
        SelectionClear
        SelectionRequest
        SelectionNotify
        ColormapNotify
        ClientMessage
        MappingNotify*/

#ifdef _DEBUG //  TEMPORARY
          cout << "unknown event" << endl;
#endif
          break;
        }

        // Handle glX events (only used for pbuffer clobber...)
        //todo...
      }
    }
  };
}

#endif
