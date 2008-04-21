//////////////////////////////////////////////////////////////////////////////
//
//    REWINDOW.CPP
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include <windows.h>

#include "common/types.h"

#include "rewindow.h"

using namespace REWrapper;

REWindow::REWindow()
{
  initFlag          = false;
  windowName        = null;
  memset(&keys, 0, sizeof(keys));
  active            = false;
  hWnd              = null;
  memset(&windowRect, 0, sizeof(windowRect));
  pCustomWindowProc = null;
  style   = 0;
  exStyle = 0;
  memset(&wndClass, 0, sizeof(wndClass));
  wndClass.cbSize         = sizeof(wndClass);
  wndClass.hInstance      = GetModuleHandle(null);
  wndClass.cbClsExtra     = sizeof(REWindow*);
  wndClass.style	        = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  wndClass.lpfnWndProc    = (WNDPROC)defaultWindowProc;
  wndClass.hIcon	        = LoadIcon(NULL, IDI_WINLOGO);
  wndClass.hCursor	      = LoadCursor(NULL, IDC_ARROW);
  wndClass.lpszClassName  = "OPENGL_WINDOW";
}

REWindow::~REWindow()
{
  if(initFlag)
    destroy();
}

void REWindow::init()
{
  if(initFlag || (wndClass.hInstance == null))
    return;
  AdjustWindowRectEx(&windowRect, style, false, exStyle);
  if(!RegisterClassEx(&wndClass))
    return;
  hWnd = CreateWindowEx(exStyle,
                        wndClass.lpszClassName,
                        windowName,
                        style,
                        windowRect.left,
                        windowRect.top,
                        windowRect.right-windowRect.left,
                        windowRect.bottom-windowRect.top,
                        null,
                        null,
                        wndClass.hInstance,
                        null);
  if(hWnd == null)
    return; /*error*/
  SetWindowLong(hWnd, GWL_USERDATA, (uint32)this);
  ShowWindow(hWnd, SW_SHOW);
  SetForegroundWindow(hWnd);
  SetFocus(hWnd);
  //OLD: getDC();
  initFlag = true;
}

void REWindow::destroy()
{
  if(!initFlag)
    return;

  /*if(hDC)
  {
    if(!ReleaseDC(hWnd, hDC)); /*error*
    hDC = NULL;
  }*/

	if(hWnd)
	{
		if(!DestroyWindow(hWnd));
		else    //remove all remaining messages
		{
			MSG msg;
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				DispatchMessage(&msg);
		}
		hWnd = NULL;
	}

  if(!UnregisterClass(wndClass.lpszClassName, wndClass.hInstance));
  initFlag = false;
}

/*void REWindow::getDC()
{
  if(!(hDC = ::GetDC(hWnd)));
}*/

void REWindow::setWindowName(const char *windowName)
{
  REWindow::windowName = windowName;
}

bool REWindow::getKey(uint16 key)
{
  return keys[ key ];
}

void REWindow::resize()
{
  if(initFlag)
  {
    AdjustWindowRectEx(&windowRect, style, false, exStyle);
    if(!MoveWindow(hWnd, windowRect.left, windowRect.top, windowRect.right-windowRect.left, windowRect.bottom-windowRect.top, false));
    ShowWindow(hWnd, SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
  }
}

void REWindow::size(uint x1, uint y1, uint x2, uint y2)
{
  windowRect.left = x1;
  windowRect.top  = y1;
  windowRect.right  = x2;
  windowRect.bottom = y2;
}

void REWindow::cursorVisible(bool visible)
{
  ShowCursor(visible);
}

LRESULT CALLBACK REWindow::processMessage( HWND hWnd, uint uiMsg, WPARAM wParam, LPARAM lParam)
{
  if(pCustomWindowProc != NULL)
    if(pCustomWindowProc(this, hWnd, uiMsg, wParam, lParam))
      return 0;

  switch(uiMsg)
  {
    case WM_ACTIVATE:
      {
        active = (LOWORD(wParam) != WA_INACTIVE);
        return 0;
      }
    case WM_CREATE:
      return 0;
    case WM_PAINT:
      {
        ValidateRect(hWnd, NULL);
        return 0;
      }
    case WM_CLOSE:  //??
    case WM_DESTROY:
      {
        PostQuitMessage(0);
        return 0;
      }
    case WM_SYSCOMMAND:
      {
        switch(wParam)  //Prevent screen saver
        {
        case SC_SCREENSAVE:
        case SC_MONITORPOWER:
          return 0;
        }
        break;
      }
    case WM_KEYDOWN:
      {
        keys[wParam] = true;

        if(pCustomKeyProc)
          pCustomKeyProc(keys);

        return 0;
      }
    case WM_KEYUP:
      {
        keys[wParam] = false;
        return 0;
      }
  }

  return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

LRESULT CALLBACK REWindow::defaultWindowProc(HWND hWnd, uint uiMsg, WPARAM wParam, LPARAM lParam)
{
  REWindow *pThis = (REWindow*)GetWindowLong(hWnd, GWL_USERDATA);

  if(pThis)
    return pThis->processMessage(hWnd, uiMsg, wParam, lParam);
  else
    return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

bool REWindow::messageHandler(int& returnCode)
{
  MSG Msg;
  if(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
  {
    if(Msg.message == WM_QUIT)
    {
      returnCode = (int)Msg.wParam;
      return true;
    }
    TranslateMessage(&Msg);
    DispatchMessage(&Msg);
  }
  return false;
}

void REWindow::setCustomKeyProc(WIN_CUSTOM_KEY_PROC pCustomKeyProc)
{
  REWindow::pCustomKeyProc = pCustomKeyProc;
}

void REWindow::setCustomWindowProc(WIN_CUSTOM_WINDOW_PROC pCustomWindowProc)
{
  REWindow::pCustomWindowProc = pCustomWindowProc;
}
