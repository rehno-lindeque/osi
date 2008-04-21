#ifndef __OSI_GUI_HPP__
#define __OSI_GUI_HPP__
//////////////////////////////////////////////////////////////////////////////
//
//    GUI.HPP
//
//    Ver 0.1 (2007-01-28)
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*
    DESCRIPTION:
      Main OpenGUI C++ interface.
*/
/*                                 INCLUDES                                 */
#include "../os.hpp"

/*                                 CONSTANTS                                */
#define GUI_VERSION 0.1

/*                                   TYPES                                  */
enum GUI_ELEMENT
{
  GUI_WORKSPACE,
  GUI_VIEW,
  GUI_ACTION,
  GUI_TOGGLE,
  GUI_GROUP
};

enum GUI_LAYOUT_MODE
{
  GUI_LAYOUT_FREE,
  GUI_LAYOUT_FILL,
  GUI_LAYOUT_DOCK_TOP,
  GUI_LAYOUT_DOCK_LEFT,
  GUI_LAYOUT_DOCK_BOTTOM,
  GUI_LAYOUT_DOCK_RIGHT,
  GUI_LAYOUT_DOCK_TOP_LEFT,
  GUI_LAYOUT_DOCK_TOP_RIGHT,
  GUI_LAYOUT_DOCK_BOTTOM_LEFT,
  GUI_LAYOUT_DOCK_BOTTOM_RIGHT
};

/*                                     API                                  */
class GUI
{
protected:
  inline GUI() {}
  inline GUI(const GUI&) {}

public:
  /* workspace */
  OSI_DYNAMIC_METHOD OSobject OSI_API_CALL beginWorkspace(OSobject displayChain);
  OSI_DYNAMIC_METHOD void OSI_API_CALL endWorkspace();

  /* layout */
  OSI_DYNAMIC_METHOD OSobject OSI_API_CALL beginLayout(OSobject frame);
  OSI_DYNAMIC_METHOD void OSI_API_CALL endLayout();

  OSI_DYNAMIC_METHOD void OSI_API_CALL layout(OSobject layout);

  /* view */
  OSI_DYNAMIC_METHOD OSobject OSI_API_CALL beginView(OSobject layout);
  OSI_DYNAMIC_METHOD void OSI_API_CALL endView();

  /* group */
  OSI_DYNAMIC_METHOD OSobject OSI_API_CALL beginGroup();
  OSI_DYNAMIC_METHOD void OSI_API_CALL endGroup();

  /* label */
  OSI_DYNAMIC_METHOD OSobject OSI_API_CALL beginLabel(OSparam parameter);
  OSI_DYNAMIC_METHOD void OSI_API_CALL endLabel();

  /* info */
  OSI_DYNAMIC_METHOD void OSI_API_CALL shortInfo(OSparam parameter);
  OSI_DYNAMIC_METHOD void OSI_API_CALL longInfo(OSparam parameter);

  /* text output */
  OSI_DYNAMIC_METHOD OSobject OSI_API_CALL beginTextOutput(OSparam parameter);
  OSI_DYNAMIC_METHOD void OSI_API_CALL endTextOutput();

  /* config */
  OSI_DYNAMIC_METHOD OSobject OSI_API_CALL beginConfig();
  OSI_DYNAMIC_METHOD void OSI_API_CALL endConfig();

  OSI_DYNAMIC_METHOD void OSI_API_CALL config(OSobject config);

  /*OSI_DYNAMIC_METHOD OSobject positionf(OSfloat* position);
  OSI_DYNAMIC_METHOD OSobject sizef(OSfloat* size);
  OSI_DYNAMIC_METHOD OSobject maximized(OSbool maximized);*/

  /* style */
  
  /* context */
  //OSI_DYNAMIC_METHOD void context(OSobject view);

  /* attributes */
  //OSI_DYNAMIC_METHOD OSobject enabled(OSid parameter)

  /* miscelaneous */
  OSI_DYNAMIC_METHOD void OSI_API_CALL display();
  OSI_DYNAMIC_METHOD void OSI_API_CALL shutdown();
};

/* initialize */
GUI* OSI_API_CALL guiInit(void* intKernel);

#endif
