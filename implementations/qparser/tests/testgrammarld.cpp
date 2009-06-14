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
using namespace QParser;

// QParser unit tests
#include "testcommon.h"

/*                                DEFINITIONS                               */
#define TESTGRAMMARLD_DEBUG_INFO

/*                                  ALIASES                                 */
//typedef BuilderLD::ActionRow ActionRow;
//typedef BuilderLD::PivotSet PivotSet;
typedef ParserLD::ParseTokens ParseTokens;

/*                                 TEST DATA                                */
// Lexical tokens to use
ParseToken x = 0, y = 0, z = 0, w = 0;

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
}

/*                                   TESTS                                  */
// Build a left-recursive grammar with unbounded look-ahead (found at around page 23 of the draft)
void BuildTestGrammar1(ParserLD& parser, Lexer& lexer, GrammarLD& grammar)
{
  // Add lexical tokens
  x = lexer.CharToken("x", 'x');
  y = lexer.CharToken("y", 'y');
  z = lexer.CharToken("z", 'z');
  w = lexer.CharToken("w", 'w');
  lexer.Build(QParser::Lexer::TOKENTYPE_LEX_WORD);
  
  cout << "Terminals:" << endl;
  cout << "  x = " << x << endl;
  cout << "  y = " << y << endl;
  cout << "  z = " << z << endl;
  cout << "  w = " << w << endl;

  // Add productions
  // 1.A -> x
  grammar.BeginProduction("A");
    grammar.ProductionToken("x");
  grammar.EndProduction();
  
  // 2.B -> x
  grammar.BeginProduction("B");
    grammar.ProductionToken("x");
  grammar.EndProduction();
  
  // 3.C -> y
  grammar.BeginProduction("C");
    grammar.ProductionToken("y");
  grammar.EndProduction();
  
  // 4.D -> AC
  grammar.BeginProduction("D");
    grammar.ProductionToken("A");
    grammar.ProductionToken("C");
  grammar.EndProduction();
  
  // 5.D -> DAC
  grammar.BeginProduction("D");
    grammar.ProductionToken("D");
    grammar.ProductionToken("A");
    grammar.ProductionToken("C");
  grammar.EndProduction();
  
  // 6.E -> BC
  grammar.BeginProduction("E");
    grammar.ProductionToken("B");
    grammar.ProductionToken("C");
  grammar.EndProduction();
  
  // 7.E -> EBC
  grammar.BeginProduction("E");
    grammar.ProductionToken("E");
    grammar.ProductionToken("B");
    grammar.ProductionToken("C");
  grammar.EndProduction();
  
  // 8.S -> Dz
  grammar.BeginProduction("S");
    grammar.ProductionToken("D");
    grammar.ProductionToken("z");
  grammar.EndProduction();
  
  // 9.S -> Ew
  grammar.BeginProduction("S");
    grammar.ProductionToken("E");
    grammar.ProductionToken("w");
  grammar.EndProduction();
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
  for(auto i = rules.begin(); i != rules.end(); ++i)
    if (*i == TOKEN_SPECIAL_IGNORE)
      cout << "ignore ";
    else
      cout << *i << ' ';
  cout << endl;
}

class TestParserLD : public ParserLD
{
public:
  // Test the recognition pass
  void TEST_RecognitionPass(ParseResult& parseResult, ParseTokens& rules) { RecognitionPass(parseResult, rules); }
  
  // Aditional accessors
  //BuilderLD& TEST_GetBuilder() { return builder; }
  
  const ParseTokens& TEST_GetParseTable() const { return parseTable; }
};

class TestGrammarLD : public GrammarLD
{
public:
  // Constructor
  TestGrammarLD(TokenRegistry& tokenRegistry) : GrammarLD(tokenRegistry) {}
  
  // Print out the grammar rules
  void TEST_PrintGrammarRules() const
  {
    cout << "Grammar Rules:" << endl;
    int cRule = 0;
    for(auto i = rules.begin(); i != rules.end(); ++i, ++cRule)
    {
      cout << '\t' << cRule << ". " << tokenRegistry.GetTokenName(i->second) << " ->";      
      for(auto c = 0; c < i->first.tokensLength; ++c)
        cout << ' ' << tokenRegistry.GetTokenName(i->first.tokens[c]);
      cout << endl;
    }
  }
  
  // Print out the states used during the grammar construction
  void TEST_PrintStates() const
  {
    int cState = 0;
    for(auto i = states.begin(); i != states.end(); ++i, ++cState)
    {
      cout << "State #" << cState << ':' << endl;
      TEST_PrintItems((*i)->items);
    }
  }
  
  // Print out a list of items present in an end-state
  void TEST_PrintItems(const vector<LDItem>& items) const
  {
    //cout << "Items: ";
    for(auto i = items.begin(); i != items.end(); ++i)
    {
      cout << i->ruleIndex << ". " << tokenRegistry.GetTokenName(i->nonterminal) << " ->";
      const auto& rule = GetRule(i->ruleIndex);
      for(uint c = 0; c < rule.tokensLength; ++c)
      {
        cout << (c == i->inputPosition? " ." : " ") << tokenRegistry.GetTokenName(rule.tokens[c]);
        if(c == i->inputPosition && i->inputPositionRule != uint(-1))
          cout << '(' << i->inputPositionRule << ')';
      }
      if (i->inputPosition == rule.tokensLength)
        cout << "    <complete>";
      cout << endl;
    }
  }
};

bool TestGrammar1()
{
  TestParserLD parser;
  
  //// Build the parse table
  TestGrammarLD grammar(parser.GetTokenRegistry());
  Lexer lexer(parser.GetTokenRegistry());
  BuildTestGrammar1(parser, lexer, grammar);
  parser.ConstructParser(&grammar);
#ifdef TESTGRAMMARLD_DEBUG_INFO
  cout << endl;
#endif
  
  // Print out the grammar rules
#ifdef TESTGRAMMARLD_DEBUG_INFO
  grammar.TEST_PrintGrammarRules();
  cout << endl;
#endif
  
  // Print out the final state graph
#ifdef TESTGRAMMARLD_DEBUG_INFO
  grammar.TEST_PrintStates();  
  cout << endl;
#endif
    
  // Print out the parse table
#ifdef TESTGRAMMARLD_DEBUG_INFO
  PrintParseTable(parser.TEST_GetParseTable());
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
    
  //// Test the recognition pass
  /* todo
  ParseResult parseResult;  // The parse result
  ParseTokens rules;        // Rules output by the recognition pass
  
  PackParseResult(parseResult, lexStream1, lexStream1 + sizeof(lexStream1)/sizeof(ParseToken));
  parser.TEST_RecognitionPass(parseResult, rules);
  PrintRules(rules);
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
  PrintRules(rules);
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
  PrintRules(rules);
  for(uint c = 0; c < rules.size(); ++c)
  {
    if(rules[c] != correctOutput3[c])
    {
      cout << "Error: rule does not match the expected outcome" << endl;  
      return false;
    }
  }*/
    
  return true;
}

/*                                ENTRY POINT                               */
int main()
{
  cout << "-----------------------------------" << endl
       << "Testing GrammarLD: " << endl;
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
