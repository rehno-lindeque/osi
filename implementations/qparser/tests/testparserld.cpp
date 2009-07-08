//////////////////////////////////////////////////////////////////////////////
//
//    TESTPARSERLD.CPP
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    UNIT TEST:
      Test the LD parser functionality
 */

/*                                 INCLUDES                                 */
// QParser
#define QPARSER_TEST_GRAMMARLD // Turn on unit testing output in the library
#include "../src/api.h"
using namespace QParser;

// QParser unit tests
#include "testcommon.h"

/*                                DEFINITIONS                               */
#define TESTPARSERLD_DEBUG_INFO

/*                                  ALIASES                                 */
typedef BuilderLD::PivotSet PivotSet;
typedef BuilderLD::GotoSet GotoSet;
typedef BuilderLD::ActionRow ActionRow;
typedef ParserLD::ParseTokens ParseTokens;

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
      printNewLine = true;
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
}

/*                                 TEST DATA                                */
// Lexical tokens to use
ParseToken x = TOKEN_FLAG_SHIFT | 0;
ParseToken y = TOKEN_FLAG_SHIFT | 1;
ParseToken z = TOKEN_FLAG_SHIFT | 2; 
ParseToken w = TOKEN_FLAG_SHIFT | 3;

/*                                   TESTS                                  */
// Build a left-recursive grammar with unbounded look-ahead (found at around page 23 of the draft)
void BuildTestGrammar1(BuilderLD& builder)
{
  ParseToken rule0 = 0; // 0.A -> x
  ParseToken rule1 = 1; // 1.B -> x
  ParseToken rule2 = 2; // 2.C -> y
  ParseToken rule3 = 3; // 3.D -> AC
  ParseToken rule4 = 4; // 4.D -> DAC
  ParseToken rule5 = 5; // 5.E -> BC
  ParseToken rule6 = 6; // 6.E -> EBC
  ParseToken rule7 = 7; // 7.S -> Dz
  ParseToken rule8 = 8; // 8.S -> Ew
 
  // s(x), r(i), s(y), r(2), r(i), p{x > 1, z > 2, w > 3}, g{2 > 4, 3 > 5}
  ActionRow& row0 = builder.AddActionRow();
  row0.AddActionShift(x);
  row0.AddActionReduce(TOKEN_SPECIAL_IGNORE);
  row0.AddActionShift(y);
  row0.AddActionReduce(rule2);
  row0.AddActionReduce(TOKEN_SPECIAL_IGNORE);
  PivotSet& pivot0 = row0.AddActionPivot();
  ActionRow& row1 = pivot0.AddPivot(x);
  ActionRow& row2 = pivot0.AddPivot(z);
  ActionRow& row3 = pivot0.AddPivot(w);
  GotoSet& goto0 = row0.AddActionGoto();
  ActionRow& row4 = goto0.AddGoto(row2);
  ActionRow& row5 = goto0.AddGoto(row3);
  
  // r(i), s(y), r(2), r(i), p{x > 1, z > 2, w > 3}, g{2 > 6, 3 > 7}
  row1.AddActionReduce(TOKEN_SPECIAL_IGNORE);
  row1.AddActionShift(y);
  row1.AddActionReduce(rule2);
  row1.AddActionReduce(TOKEN_SPECIAL_IGNORE);
  PivotSet& pivot1 = row1.AddActionPivot();
  pivot1.AddPivot(x, row1);
  pivot1.AddPivot(z, row2);
  pivot1.AddPivot(w, row3);
  GotoSet& goto1 = row1.AddActionGoto();
  ActionRow& row6 = goto1.AddGoto(row2);
  ActionRow& row7 = goto1.AddGoto(row3);
  
  // return  
  row2.AddActionReturn();
  
  // return
  row3.AddActionReturn();
  
  // rp(3), rp(0), r(7), accept
  row4.AddActionReducePrev(rule3);
  row4.AddActionReducePrev(rule0);
  row4.AddActionReduce(rule7);
  row4.AddActionAccept();
  
  // rp(5), rp(1), r(8), accept
  row5.AddActionReducePrev(rule5);
  row5.AddActionReducePrev(rule1);
  row5.AddActionReduce(rule8);
  row5.AddActionAccept();
  
  // rp(4), rp(0), return
  row6.AddActionReducePrev(rule4);
  row6.AddActionReducePrev(rule0);
  row6.AddActionReturn();  
  
  // rp(6), rp(1), return
  row7.AddActionReducePrev(rule6);
  row7.AddActionReducePrev(rule1);
  row7.AddActionReturn();
}

void PackParseResult(ParseResult& result, ParseToken* streamBegin, ParseToken* streamEnd)
{
  // Allocate the lex stream
  delete[] result.lexStream.data;
  result.lexStream.length = streamEnd-streamBegin;
  result.lexStream.elementSize = sizeof(ParseMatch);
  result.lexStream.data = new ParseMatch[result.lexStream.length];
  
  // Assign each match a token and an offset and length of 0.
  for(uint c = 0; c < result.lexStream.length; ++c)
  {
    result.lexStream.data[c].token = streamBegin[c];
    result.lexStream.data[c].offset = 0;
    result.lexStream.data[c].length = 0;
  }
}

void PrintRules(const ParseTokens& rules)
{
  cout << "Rules: ";
  for(ParseTokens::const_iterator i = rules.begin(); i != rules.end(); ++i)
    if (*i == TOKEN_SPECIAL_IGNORE)
      cout << "ignore ";
    else
      cout << *i << ' ';
  cout << endl;
}

class TestParserLD : public ParserLD
{
public:
  // Build the grammar from an external builder object rather than doing it internaly
  const ParseTokens& TEST_ConstructParser(BuilderLD& builder)
  {
    builder.ConstructParseTable(parseTable);
    return parseTable;
  }
  
  // Test the recognition pass
  void TEST_RecognitionPass(ParseResult& parseResult, ParseTokens& rules) { RecognitionPass(parseResult, rules); }
};

bool TestGrammar1()
{
  TestParserLD parser;
  
  //// Build the parse table
  GrammarLD grammar(parser.GetTokenRegistry());
  BuilderLD builder;
  BuildTestGrammar1(builder);
  const ParseTokens& parseTable = parser.TEST_ConstructParser(builder);
#ifdef TESTPARSERLD_DEBUG_INFO
  PrintParseTable(parseTable);
  cout << endl;
#endif
  
  //// Construct some test input (lexical) streams along with their expected results (rules)  
  // Stream 1: xyxyxyz (Correct input)
  ParseToken lexStream1[] = { x,y,x,y,x,y,z };
  ParseToken correctOutput1[] = { 0,2,3,0,2,4,0,2,4,7 };
  
  // Stream 2: xyxyxyz (Correct input)
  ParseToken lexStream2[] = { x,y,x,y,x,y,w };
  ParseToken correctOutput2[] = { 1,2,5,1,2,6,1,2,6,8 };
  
  // Stream 3: xyz (Correct input)
  ParseToken lexStream3[] = { x,y,w };
  ParseToken correctOutput3[] = { 1,2,5,8 };
  
  // Stream 4: xyxyxy (Incorrect input)
  //ParseToken lexStream4[] = { x,y,x,y,x,y };
  
  // Stream 5: xz (Incorrect input)
  //ParseToken lexStream5[] = { x,z };
  
  // Stream 6: xyyz (Incorrect input)
  //ParseToken lexStream6[] = { x,y,y,z };
  
  //// Test the recognition pass
  ParseResult parseResult;  // The parse result
  ParseTokens rules;        // Rules output by the recognition pass
  
  PackParseResult(parseResult, lexStream1, lexStream1 + sizeof(lexStream1)/sizeof(ParseToken));
  parser.TEST_RecognitionPass(parseResult, rules);
#ifdef TESTPARSERLD_DEBUG_INFO
  PrintRules(rules);
#endif
  for(uint c = 0; c < rules.size(); ++c)
  {
    if(rules[c] != correctOutput1[c])
    {
      cout << "Error: rule does not match the expected outcome" << endl;  
      return false;
    }
  }
  
  PackParseResult(parseResult, lexStream2, lexStream2 + sizeof(lexStream2)/sizeof(ParseToken));
  parser.TEST_RecognitionPass(parseResult, rules);
#ifdef TESTPARSERLD_DEBUG_INFO
  PrintRules(rules);
#endif
  for(uint c = 0; c < rules.size(); ++c)
  {
    if(rules[c] != correctOutput2[c])
    {
      cout << "Error: rule does not match the expected outcome" << endl;  
      return false;
    }
  }
  
  PackParseResult(parseResult, lexStream3, lexStream3 + sizeof(lexStream3)/sizeof(ParseToken));
  parser.TEST_RecognitionPass(parseResult, rules);
#ifdef TESTPARSERLD_DEBUG_INFO
  PrintRules(rules);
#endif
  for(uint c = 0; c < rules.size(); ++c)
  {
    if(rules[c] != correctOutput3[c])
    {
      cout << "Error: rule does not match the expected outcome" << endl;  
      return false;
    }
  }
    
  return true;
}

/*                                ENTRY POINT                               */
int main()
{
  cout << "-----------------------------------" << endl
       << "Testing ParserLD: " << endl;
  cout.flush();
  if (TestGrammar1())  
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
