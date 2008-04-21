//////////////////////////////////////////////////////////////////////////////
//
//    WINDOWPROC.CPP
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                                 INCLUDES                                 */
#include "api.h"

#include <windows.h>

/*                               IMPLEMENTATION                             */
LRESULT CALLBACK WinGLKernel::Kernel::windowProc(HWND hWnd, uint uiMsg, WPARAM wParam, LPARAM lParam)
{
  /* *pWindow = (*)GetWindowLong(hWnd, GWL_USERDATA);

  if(pThis)
    return pThis->processMessage(hWnd, uiMsg, wParam, lParam);
  else
    return DefWindowProc(hWnd, uiMsg, wParam, lParam);*/
  return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}
