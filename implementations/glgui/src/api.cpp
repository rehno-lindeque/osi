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
      GLGUI api implementation
*/

/*                              COMPILER MACROS                             */
#pragma warning(push)
#pragma warning(disable:4311) // Type cast: Pointer truncation
#pragma warning(disable:4312) // Type cast: Conversion to greater size (pointer)
//#pragma warning(disable:4390) // Empty controlled statement
//#pragma warning(disable:4244) // Conversion from float to int (possible loss of data)

/*                                 INCLUDES                                 */
#include "api.h"

/*                              COMPILER MACROS                             */
#define _this (*(GLGUI::GUI*)this)

#define cast_id     BaseGUI::GUI::cast_id
#define cast_object BaseGUI::GUI::cast_object

/*                                  GLOBALS                                 */
IntKernel* intKernel;

/*                               IMPLEMENTATION                             */
OSobject OSI_API_CALL GUI::beginWorkspace(OSobject displayChain)
{
  GLGUI::Workspace* workspaceObject = _this.beginObject<GLGUI::Workspace>();
  workspaceObject->displayChain = displayChain;

  _this.workspaces.push_back(workspaceObject);

  return cast_object(workspaceObject);
}

void OSI_API_CALL GUI::endWorkspace()
{
  GLGUI::Workspace* workspaceObject = _this.endObject<GLGUI::Workspace>();
}

OSobject OSI_API_CALL GUI::beginLayout(OSobject frame)
{
  GLGUI::Layout* layoutObject = _this.beginObject<GLGUI::Layout>();
  
  // Add layout to frame

  
  // todo: BUSY HERE LAST
  //note: at the moment a frame can have multiple layouts. Is this correct?? 
  // (Yes: the frame can have a docking layout + a free layout etc.)
  // Frames should be assigned to workspaces....


  //_this.frameLayouts.insert(std::pair<OSobject, GLGUI::Layout*>(frame, layoutObject));

  return cast_object(layoutObject);
}

void OSI_API_CALL GUI::endLayout()
{
  GLGUI::Layout* layoutObject = _this.endObject<GLGUI::Layout>();
}

void OSI_API_CALL GUI::layout(OSobject layout)
{
  GLGUI::Workspace& workspaceObject = *_this.getActiveObject<GLGUI::Workspace>();
  GLGUI::Layout* layoutObject = cast_id<GLGUI::Layout>(layout);

  workspaceObject.layouts.push_back(layoutObject);
}

OSobject OSI_API_CALL GUI::beginView(OSobject frame)
{
  GLGUI::View* viewObject = _this.beginObject<GLGUI::View>();

  return cast_object(viewObject);
}

void OSI_API_CALL GUI::endView()
{
  GLGUI::View* view = _this.endObject<GLGUI::View>();
}

OSobject OSI_API_CALL GUI::beginGroup()
{
  GLGUI::Group* groupObject = _this.beginObject<GLGUI::Group>();

  return cast_object(groupObject);
}

void OSI_API_CALL GUI::endGroup()
{
  GLGUI::Group* groupObject = _this.endObject<GLGUI::Group>();
}

OSobject OSI_API_CALL GUI::beginLabel(OSparam parameter)
{
  GLGUI::Label* labelObject = _this.beginObject<GLGUI::Label>();

  return cast_object(labelObject);
}

void GUI::endLabel()
{
  GLGUI::Label* labelObject = _this.endObject<GLGUI::Label>();
}

void OSI_API_CALL GUI::shortInfo(OSparam parameter)
{
  //todo
}

void OSI_API_CALL GUI::longInfo(OSparam parameter)
{
  //todo
}

OSobject OSI_API_CALL GUI::beginTextOutput(OSparam parameter)
{
  GLGUI::TextOutput* textOutputObject = _this.beginObject<GLGUI::TextOutput>();
  return cast_object(textOutputObject);
}

void OSI_API_CALL GUI::endTextOutput()
{
  GLGUI::TextOutput* textOutputObject = _this.endObject<GLGUI::TextOutput>();
}

OSobject OSI_API_CALL GUI::beginConfig()
{
  GLGUI::Config* configObject = _this.beginObject<GLGUI::Config>();

  return cast_object(configObject);
}

void OSI_API_CALL GUI::endConfig()
{
  GLGUI::Config* configObject = _this.endObject<GLGUI::Config>();
}

void OSI_API_CALL GUI::config(OSobject config)
{
  // todo
}

//OSobject positionf(OSfloat* position);
//OSobject sizef(OSfloat* size);
//OSobject maximized(OSbool maximized);

void OSI_API_CALL GUI::display()
{
  //// Display workspaces
  for(std::vector<GLGUI::Workspace*>::const_iterator iWorkspace = _this.workspaces.begin(); iWorkspace != _this.workspaces.end(); ++iWorkspace)
  {
    const GLGUI::Workspace& workspace = **iWorkspace;

    //// Clip frames to displays
    for(std::vector<GLGUI::Layout*>::const_iterator iLayout = workspace.layouts.begin(); iLayout != workspace.layouts.end(); ++iLayout)
    {
      const GLGUI::Layout& layout = **iLayout;

      // Get clip frames
      const OSuint32&             nClipFrames = *(const OSuint32*)intKernel->openClipFrames(workspace.displayChain, layout.frame);
      const IntKernel::ClipFrame* clipFrames  = (const IntKernel::ClipFrame*)((&nClipFrames) + 1);

      for(uint cClipFrame = 0; cClipFrame < nClipFrames; ++cClipFrame)
      {
        //todo... BUSY HERE
        // insert clipframe into map, indexed by layout and by display
      }

      intKernel->closeClipFrames();
    }

    //// Get the displays attached to the display chain of the workspace
    const OSuint32& nDisplays = *(const OSuint32*)intKernel->openDisplays(workspace.displayChain);

      const OSobject* displays = (const OSobject*)((&nDisplays)+1);

      for(uint cDisplay = 0; cDisplay < nDisplays; ++cDisplay)
      {
        // Make opengl rendering context current
        #ifdef _WIN32
          const WinGLKernelDisplayObject& displayObject = *(WinGLKernelDisplayObject*)(displays[cDisplay]);
          HDC hDC = GetDC(displayObject.hWnd);
          wglMakeCurrent(hDC, displayObject.hRC);
        #endif

        // Setup opengl viewport
        glViewport(0, 0, 512, 512);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Clear viewport (temporary)
        if(cDisplay == 0)
          glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        else
          glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glDepthFunc(GL_LEQUAL);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        

        // Draw a test triangle
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLES);
          glVertex2f(0.2f, 0.2f);
          glVertex2f(0.8f, 0.2f);
          glVertex2f(0.5f, 0.8f);
        glEnd();
      }

    intKernel->closeDisplays();

    
  }
}

void OSI_API_CALL GUI::shutdown()
{
  //todo

  _this.delAllObjects();
}

GUI* OSI_API_CALL guiInit(void* intKernel)
{
  ::intKernel = (IntKernel*)intKernel;

  return (GUI*)new GLGUI::GUI;
}

/*                              COMPILER MACROS                             */
#pragma warning(pop)
