#ifndef __GLGUI_API_H__
#define __GLGUI_API_H__
//////////////////////////////////////////////////////////////////////////////
//
//    API.H
//
//    Author:      Rehno Lindeque
//
//    Description: WinGUI classes used to implement api
//
//    Version: 0.1 (prototype)
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                                   INCLUDES                               */
// Operating system
#ifdef _WIN32
  #include <windows.h>
#endif

// OpenGL
#include <gl/gl.h>

// OpenGUI
#include <osi/gui/gui.hpp>
#include <osi/gui/ext/glgui.h>

// OpenKernel
#include <osi/kernel/intkernel.hpp>
#ifdef _WIN32
  #include <osi/kernel/ext/winglkernel.h>
#endif

// Base
#include <base/common/types.h>
#include <base/common/object.h>

// BaseGUI
#include <basegui/basegui.h>


/*                             FORWARD DECLERATIONS                         */
namespace GLGUI
{
  class Workspace;
  class Layout;
  class View;
  class Group;
  class Label;
  class TextOutput;
  class Config;
}

/*                                   INCLUDES                               */
// GLGUI
#include "workspace.h"
#include "layout.h"
#include "view.h"
#include "group.h"
#include "label.h"
#include "textoutput.h"
#include "config.h"

/*                                   CLASSES                                */
namespace GLGUI
{
  class GUI : public BaseGUI::GUI
  {
  public:
    std::vector<Workspace*> workspaces;
    std::multimap<OSobject, GLGUI::Layout*> frameLayouts;
  };
}

/*                                   INCLUDES                               */

#endif
