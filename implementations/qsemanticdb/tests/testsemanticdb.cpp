//////////////////////////////////////////////////////////////////////////////
//
//    TESTGRAMMARLD.CPP
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    UNIT TEST:
      Test the LD grammar functionality
 */

/*                                 INCLUDES                                 */
// QParser
#include "../src/api.h"
using namespace QSemanticDB;

// QParser unit tests
#include "testcommon.h"

/*                                DEFINITIONS                               */
//#define TESTSEMANTICDB_DEBUG_INFO

/*                                  ALIASES                                 */

/*                                 TEST DATA                                */

/*                                  HELPERS                                 */

/*                                   TESTS                                  */
// General test of the semantic DB
bool TestSemanticDB()
{
  cout << "Nothing to do yet..." << endl;
  return true;
}

/*                                ENTRY POINT                               */
int main()
{
  cout << "-----------------------------------" << endl
       << "Testing SemanticDB: " << endl;
  cout.flush();
  if (TestSemanticDB())  
  {
    cout << "SUCCESS" << endl;
    cout.flush();
    return 0;
  }
  else
  {
    return 1; // Test case failed
  }
}
