//////////////////////////////////////////////////////////////////////////////
//
//    TESTSCHEDULER.CPP
//
//    Copyright © 2010, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    UNIT TEST:
      Test the schedule functionality
 */

/*                                 INCLUDES                                 */
// QSemanticDB
#include "../src/api.h"
using namespace QSemanticDB;

// QSemanticDB unit tests
#include "testcommon.h"

/*                                DEFINITIONS                               */

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
