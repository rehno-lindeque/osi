//////////////////////////////////////////////////////////////////////////////
//
//    TESTBUILDERLD.CPP
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    UNIT TEST:
      Test the LD builder functionality
 */

/*                                 INCLUDES                                 */
// OpenParser
#include <osix/parser/parser.hpp>
#ifdef _DEBUG
# include <osix/parser/parserdbg.hpp>
#endif

// BaseParser
#include <baseparser/baseparser.h>

// STL
#include <algorithm>

// QParser
#include "../src/builderld.h"
using namespace QParser;

// QParser unit tests
#include "testcommon.h"

/*                                DEFINITIONS                               */
#define TESTBUILDERLD_DEBUG_INFO

/*                                  ALIASES                                 */
typedef BuilderLD::ActionRow ActionRow;
typedef BuilderLD::PivotSet PivotSet;
typedef BuilderLD::ParseTokens ParseTokens;

/*                                  HELPERS                                 */
void PrintParseTable(const ParseTokens& parseTable)
{
  //cout << "\tThe constructed parse table:" << endl;
  cout << "\tOffset\t| Actions";  
  bool printNewLine = true;
  for(uint cToken = 0; cToken < parseTable.size(); ++cToken)
  {
    // Print the offset of the new line
    if(printNewLine)
    {
      cout << endl << "\t" << cToken << "\t| ";
      printNewLine = false;
    }
    
    // Print the token info
    ParseToken token = parseTable[cToken];
    if(token == TOKEN_SPECIAL_IGNORE)
      cout << "ignore ";
    else if(token == TOKEN_ACTION_PIVOT)
    {
      // Get the length of the pivot
      TEST_ASSERT(cToken < parseTable.size()-1);
      ParseToken pivotLength = parseTable[++cToken];
      
      // Print out each of the pivots
      cout << "pivot { ";
          
      for(uint cPivot = 0; cPivot < pivotLength; ++cPivot)
      {
        // Get the token to match for this pivot
        TEST_ASSERT(cToken < parseTable.size()-1);
        ParseToken terminal = parseTable[++cToken];

        // Get the parse table offset for this tokem
        TEST_ASSERT(cToken < parseTable.size()-1);
        ParseToken targetOffset = parseTable[++cToken];
        
        cout << "shift(" << (terminal & ~TOKEN_FLAG_SHIFT) <<")->" << targetOffset << " ";
      }
      
      cout << "} ";
    }
    else if(token == TOKEN_ACTION_GOTO)
    {
      // Get the parse table offset of the lookahead state
      TEST_ASSERT(cToken < parseTable.size()-1);
      ParseToken lookaheadOffset = parseTable[++cToken];
      
      // Get the parse table offset of the goto target
      TEST_ASSERT(cToken < parseTable.size()-1);
      ParseToken targetOffset = parseTable[++cToken];
        
      cout << "goto(" << lookaheadOffset << "->" << targetOffset << ") ";
    }
    else if(token == TOKEN_ACTION_RETURN)
    {
      cout << "return";
      printNewLine = true;
    }
    else if(token == TOKEN_ACTION_ACCEPT)
    {
      cout << "accept";
      printNewLine = true;
    }
    else if(token & TOKEN_FLAG_SHIFT)
      cout << "shift(" << (token & ~TOKEN_FLAG_SHIFT) << ") ";
    else if(token & TOKEN_FLAG_REDUCEPREV)
      cout << "reduceprev(" << (token & ~TOKEN_FLAG_REDUCEPREV) << ") ";
    else
      cout << "reduce(" << token <<") ";
  }
  cout << endl;
  cout.flush();
}

/*                                   TESTS                                  */
// Build a left-recursive grammar with unbounded look-ahead
void BuildTestGrammar1(BuilderLD& builder)
{
  ParseToken x = TOKEN_FLAG_SHIFT | 0;
  ParseToken y = TOKEN_FLAG_SHIFT | 1;
  ParseToken z = TOKEN_FLAG_SHIFT | 2; 
  ParseToken w = TOKEN_FLAG_SHIFT | 3;

  ParseToken rule1 = 0; // 1.A -> x
  ParseToken rule2 = 1; // 2.B -> x
  ParseToken rule3 = 2; // 3.C -> y
  ParseToken rule4 = 3; // 4.D -> AC
  ParseToken rule5 = 4; // 5.D -> DAC
  ParseToken rule6 = 5; // 6.E -> BC
  ParseToken rule7 = 6; // 7.E -> EBC
  ParseToken rule8 = 7; // 8.S -> Dz
  ParseToken rule9 = 8; // 9.S -> Ew
 
  // s(x), r(i), s(y), r(3), r(i), p{x > 1, z > 2, w > 3}, g{3 > 5}, rp(4), rp(1), r(8), accept
  ActionRow& row0 = builder.AddActionRow();
  row0.AddActionShift(x);
  row0.AddActionReduce(TOKEN_SPECIAL_IGNORE);
  row0.AddActionShift(y);
  row0.AddActionReduce(rule3);
  row0.AddActionReduce(TOKEN_SPECIAL_IGNORE);
  PivotSet& pivot0 = row0.AddActionPivot();
  row0.AddActionGoto(3, 5);
  row0.AddActionReducePrev(rule4);
  row0.AddActionReducePrev(rule1);
  row0.AddActionReduce(rule8);
  row0.AddActionAccept();

  // r(i), s(y), r(3), r(i), p{x > 1, z > 2, w > 3}, g{3 > 4}, rp(5), rp(1), return
  ActionRow& row1 = pivot0.AddPivot(x);
  row1.AddActionReduce(TOKEN_SPECIAL_IGNORE);
  row1.AddActionShift(y);
  row1.AddActionReduce(rule3);
  row1.AddActionReduce(TOKEN_SPECIAL_IGNORE);
  PivotSet& pivot1 = row1.AddActionPivot();
  pivot1.AddPivot(x, 1);
  pivot1.AddPivot(z, 2);
  pivot1.AddPivot(w, 3);
  row1.AddActionGoto(3, 4);
  row1.AddActionReducePrev(rule5);
  row1.AddActionReducePrev(rule1);
  row1.AddActionReturn();

  // return
  ActionRow& row2 = pivot0.AddPivot(z);
  row2.AddActionReturn();
  
  // return
  ActionRow& row3 = pivot0.AddPivot(w);
  row3.AddActionReturn();
  
  // rp(7), rp(2), return
  ActionRow& row4 = builder.AddActionRow();
  row4.AddActionReducePrev(rule7);
  row4.AddActionReducePrev(rule2);
  row4.AddActionReturn();
  
  // rp(6), rp(2), r(9), accept
  ActionRow& row5 = builder.AddActionRow();
  row5.AddActionReducePrev(rule6);
  row5.AddActionReducePrev(rule2);
  row5.AddActionReduce(rule9);
  row5.AddActionAccept();
}

/*void BuildTestGrammar1()
{
  BuilderLD builder;
  
  // Left-recursive grammar with unbounded look-ahead
  ParseToken x = TOKEN_FLAG_SHIFT | 0;
  ParseToken y = TOKEN_FLAG_SHIFT | 1;
  ParseToken z = TOKEN_FLAG_SHIFT | 2;
  ParseToken w = TOKEN_FLAG_SHIFT | 3;

  ParseToken rule1 = 0; // 1.A -> x
  ParseToken rule2 = 1; // 2.B -> x
  ParseToken rule3 = 2; // 3.C -> y
  ParseToken rule4 = 3; // 4.D -> AC
  ParseToken rule5 = 4; // 5.D -> DAC
  ParseToken rule6 = 5; // 6.E -> BC
  ParseToken rule7 = 6; // 7.E -> EBC
  ParseToken rule8 = 7; // 8.S -> Dz
  ParseToken rule9 = 8; // 9.S -> Ew

#ifdef TESTBUILDERLD_DEBUG_INFO
  cout << "\tCreate row 0" << endl;
  cout.flush();
#endif
  
  // s(x), r(i), s(y), r(3), r(i), p{x > 1, z > 2, w > 3}, g{3 > 5}, rp(4), rp(1), r(8), accept
  ActionRow& row0 = builder.AddActionRow();
  row0.AddActionShift(x);
  row0.AddActionReduce(TOKEN_SPECIAL_IGNORE);
  row0.AddActionShift(y);
  row0.AddActionReduce(rule3);
  row0.AddActionReduce(TOKEN_SPECIAL_IGNORE);
  PivotSet& pivot0 = row0.AddActionPivot();
  row0.AddActionGoto(3, 5);
  row0.AddActionReducePrev(rule4);
  row0.AddActionReducePrev(rule1);
  row0.AddActionReduce(rule8);
  row0.AddActionAccept();
  
#ifdef TESTBUILDERLD_DEBUG_INFO
  cout << "\tCreate row 1" << endl;
  cout.flush();
#endif

  // r(i), s(y), r(3), r(i), p{x > 1, z > 2, w > 3}, g{3 > 4}, rp(5), rp(1), return
  ActionRow& row1 = pivot0.AddPivot(x);
  row1.AddActionReduce(TOKEN_SPECIAL_IGNORE);
  row1.AddActionShift(y);
  row1.AddActionReduce(rule3);
  row1.AddActionShift(TOKEN_SPECIAL_IGNORE);
  PivotSet& pivot1 = row1.AddActionPivot();
  pivot1.AddPivot(x, 1);
  pivot1.AddPivot(z, 2);
  pivot1.AddPivot(w, 3);
  row1.AddActionGoto(3, 4);
  row1.AddActionReducePrev(rule5);
  row1.AddActionReducePrev(rule1);
  row1.AddActionReturn();

#ifdef TESTBUILDERLD_DEBUG_INFO
  cout << "\tCreate row 2" << endl;
  cout.flush();
#endif

  // return
  ActionRow& row2 = pivot0.AddPivot(z);
  row2.AddActionReturn();
  
  
#ifdef TESTBUILDERLD_DEBUG_INFO
  cout << "\tCreate row 3" << endl;
  cout.flush();
#endif

  // return
  ActionRow& row3 = pivot0.AddPivot(w);
  row3.AddActionReturn();
  
#ifdef TESTBUILDERLD_DEBUG_INFO
  cout << "\tCreate row 4" << endl;
  cout.flush();
#endif
  
  // rp(7), rp(2), return
  ActionRow& row4 = builder.AddActionRow();
  row4.AddActionReducePrev(rule7);
  row4.AddActionReducePrev(rule2);
  row4.AddActionReturn();

#ifdef TESTBUILDERLD_DEBUG_INFO 
  cout << "\tCreate row 5" << endl;
  cout.flush();
#endif
  
  // rp(6), rp(2), r(9), accept
  ActionRow& row5 = builder.AddActionRow();
  row5.AddActionReducePrev(rule6);
  row5.AddActionReducePrev(rule2);
  row5.AddActionReduce(rule9);
  row5.AddActionAccept();
  
#ifdef TESTBUILDERLD_DEBUG_INFO
  cout << "\tConstruct the parse table" << endl;
  cout.flush();
#endif
  
  // Build a parse table from the given definitions
  typedef BuilderLD::ParseTokens ParseTokens;
  ParseTokens parseTable;
  builder.ConstructParseTable(parseTable);
  
  // Print out the constructed parse table
#ifdef TESTBUILDERLD_DEBUG_INFO
  PrintParseTable(parseTable);
#endif
}*/

void TestGrammar1()
{
  // Build the test grammar
  BuilderLD builder;  
  BuildTestGrammar1(builder);
  
#ifdef TESTBUILDERLD_DEBUG_INFO
  cout << "\tConstruct the parse table" << endl;
  cout.flush();
#endif
  
  // Construct a parse table from the given definitions
  typedef BuilderLD::ParseTokens ParseTokens;
  ParseTokens parseTable;
  builder.ConstructParseTable(parseTable);
  
  // Print out the constructed parse table
#ifdef TESTBUILDERLD_DEBUG_INFO
  PrintParseTable(parseTable);
#endif
}

/*                                ENTRY POINT                               */
int main(int argc, const char **argv)
{
  cout << "-----------------------------------" << endl
       << "Testing BuilderLD: " << endl;
  cout.flush();
  TestGrammar1();
  
  cout << "SUCCESS" << endl;
  cout.flush();
  return 0; // Unit test successfull
}
