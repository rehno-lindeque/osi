#ifndef __REWRAPPER_OPENREAPPLICATION_H__
#define __REWRAPPER_OPENREAPPLICATION_H__
//////////////////////////////////////////////////////////////////////////////
//
//    OPENREAPPLICATION.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Standard OpenRE application wrapper
*/

#include "rewindow.h"

int reMain(int nArguments, char** arguments);

namespace REWrapper {

class OpenREApp
{
/*#ifdef _WIN32
  friend int WINAPI WinMain(HINSTANCE hInstance,
				            HINSTANCE hPrevInstance,
					          PSTR caCmdLine,
					          int iCmdShow);
#endif*/
  friend int ::reMain(int nArguments, char** arguments);

public:
  class CommandLine
  {
  public:
    int nArguments;
    char** arguments;
  } commandLine;

private:
  bool endFlag;
  bool osMain(int &r);

//protected:
private:
  //Replace, but don't call any of these functions. They are called automatically.
  virtual void init() {}
  virtual void main() {}
  virtual void end() {}

  REWindow window;
  //todo: HRERC hRC;

  void updateDisplayMode();
public:
  class DisplayMode
  {
  public:
    bool  fullscreen;
    uint  width;
    uint  height;
    uint8 bpp;
  }displayMode;

  void initInternal(); //do not override or call these
  void destroyInternal();

  OpenREApp(const char *appName);
  ~OpenREApp();
  void exit();
};

}

#endif
