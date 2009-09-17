//////////////////////////////////////////////////////////////////////////////
//
//    TESTCOMMON.H
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Common definitions and utilities for the qparser unit tests
 */

/*                                 INCLUDES                                 */
// STL
#include <iostream>
using namespace std;

/*                                DEFINITIONS                               */
#define TEST_ASSERT(condition) { if(!(condition)) cout << "Assertion failed at line" << __LINE__ << endl; }