#ifndef __GLGUI_WORKSPACE_H__
#define __GLGUI_WORKSPACE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    WORKSPACE.H
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      GLGUI workspace class.

    TODO:
      + Remove?
*/

namespace GLGUI
{
/*                                  CLASSES                                 */
  class Workspace : public Base::Object
  {
  public:
    OSobject displayChain;
    std::vector<GLGUI::Layout*> layouts;
  };
}

#endif
