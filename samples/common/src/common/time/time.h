#ifndef __SAMPLES_TIME_H__
#define __SAMPLES_TIME_H__
//////////////////////////////////////////////////////////////////////////////
//
//    TIME.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      Common timing functions.
    
    USAGE:
      types.h and windows.h must be included before this header can be used

    TODO:
      + Code Review 2006-08-20
        ----------------------
        (Portability):
        Find & replace with a cross platform timing library since this
        is currently windows only
      
        (Performance):
        According to the Boost timer library, the high performance counters
        on windows are very accurate, but have a very high latency, so this
        should probably be replaced (either by boost timer or by clock())

        (Conventions):
        Time is a somewhat generic name for a global type. Either rename
        this or apply a namespace.
*/

/*                              COMPILER MACROS                             */
//#ifndef _WINDOWS_
//  #error("windows.h must be included before time.h")
//#endif
#ifndef __SAMPLES_TYPES_H__
  #error("types.h must be included before time.h")
#endif

/*                                  CLASSES                                 */
#ifdef _WIN32
class Time
{
public:
  inline static uint64 getCounter()
  {
    uint64 counter;
    QueryPerformanceCounter((LARGE_INTEGER*)&counter);
    return counter;
  }

  inline static uint64 getFrequency()
  {
    uint64 counterFrequency;
    QueryPerformanceFrequency((LARGE_INTEGER*)&counterFrequency);
    return counterFrequency;
  }
};
#endif

#endif
