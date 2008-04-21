#ifndef __REWRAPPER_REWINDOW_H__
#define __REWRAPPER_REWINDOW_H__
//////////////////////////////////////////////////////////////////////////////
//
//    REWINDOW.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Standard OpenRE window wrapper
*/

/*#include <windows.h>
#include "common/types.h"*/

namespace REWrapper {

class REWindow;

typedef bool WINDOW_KEYS[256];
typedef bool (*WIN_CUSTOM_WINDOW_PROC)( REWindow *pThis, HWND hwnd, uint uiMsg, WPARAM wParam, LPARAM lParam );
typedef void (*WIN_CUSTOM_KEY_PROC)( WINDOW_KEYS Keys );

class REWindow
{
private:
  WINDOW_KEYS keys;

public:
  bool active;
  HWND hWnd;
  //HDC  hDC;

protected:
  WNDCLASSEX wndClass;
  RECT windowRect;

  LRESULT CALLBACK processMessage(HWND hWnd, uint uiMsg, WPARAM wParam, LPARAM lParam);
  static LRESULT CALLBACK defaultWindowProc(HWND hWnd, uint uiMsg, WPARAM wParam, LPARAM lParam);

  WIN_CUSTOM_WINDOW_PROC pCustomWindowProc;
  const char *windowName;
  WIN_CUSTOM_KEY_PROC pCustomKeyProc;

public:
  uint32 style, exStyle;
  bool initFlag;

  REWindow();
  ~REWindow();
  void setWindowName(const char *windowName);
  void init();
  void destroy();
  bool messageHandler(int& returnCode);
  bool getKey(uint16 key);
  void size(uint x1, uint y1, uint x2, uint y2);
  void resize();
  void cursorVisible(bool visible);
  //void getDC();

  void setCustomKeyProc(WIN_CUSTOM_KEY_PROC pCustomKeyProc);
  void setCustomWindowProc(WIN_CUSTOM_WINDOW_PROC pCustomWindowProc);
};

}

#endif
