#ifndef __OSI_GUI_H__
#define __OSI_GUI_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GUI.H
//
//    Ver 0.1 (2007-02-02)
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*
    DESCRIPTION:
      Main OpenGUI C interface.
*/
/*                                 INCLUDES                                 */
#include "../os.h"

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

/*                                     API                                  */
/* workspace */
OSobject guiBeginWorkspace(OSobject device);
void guiEndWorkspace();

/* layout */
OSobject guiBeginLayout(OSobject frame);
void guiEndLayout();

void guiLayout(OSobject layout);

/* view */
OSobject guiBeginView(OSobject layout);
void guiEndView();

/* group */
OSobject guiBeginGroup();
void guiEndGroup();

/* label */
OSobject guiBeginLabel(OSparam parameter);
void guiEndLabel();

/* info */
void guiShortInfo(OSparam parameter);
void guiLongInfo(OSparam parameter);

/* text output */
OSobject guiBeginTextOutput(OSparam parameter);
void guiEndTextOutput();

/* config */
OSobject guiBeginConfig();
void guiEndConfig();

void guiConfig(OSobject config);

/* miscelaneous */
void guiDisplay();
void guiShutdown();

/* initialize */
void guiInit();

#endif
