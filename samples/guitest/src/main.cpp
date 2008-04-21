//////////////////////////////////////////////////////////////////////////////
//
//    MAIN.CPP
//
//    Copyright (c) 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      A OpenGUI unit test
*/
/*                                  INCLUDES                                */

#include <windows.h>

/* Common */
#include <common/types.h>

/* OpenGUI */
#include <osi/gui/gui.h>


#include <stdio.h>
//#include <iostream.h>


/*                              COMPILER MACROS                             */
#pragma warning(push)
//#pragma warning(disable: 4305)

/*                                  CLASSES                                 */
class GUITest
{
protected:
  
public:

  virtual void init()
  {
    // Initialize OpenGUI
    //guiInit();    
  }

  virtual bool main()
  {
    return false;
  }

  virtual void end()
  {
    //guiShutdown();
  }

  GUITest() {}
}application;

void guiAddEvent(void* tmp) //todo: make this a c-call
{
  printf("%d", (int)tmp);
}

int main()
{
  /*application.init();
  while(application.main()) {}
  application.end();*/

  __asm push temp;
  __asm call guiAddEvent;

  __asm mov eax, temp;
  __asm jmp eax;


buttonClick:
  return 2;

temp:

  // temporary event test
  //int eventLoc = buttonClick;
  //goto eventLoc;
  
  printf("jo");
  getc(stdin);

  return 5;


}

/*                              COMPILER MACROS                             */
#pragma warning(pop)
