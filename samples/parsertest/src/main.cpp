//////////////////////////////////////////////////////////////////////////////
//
//    MAIN.CPP
//
//    Copyright (c) 2007-2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      OpenParser unit test
*/
/*                                  INCLUDES                                */

#ifdef _WIN32
  #include <windows.h>
#endif

/* Base */
#include <base/common/types.h>

/* OpenParser */
#include <osix/parser/parser.hpp>
#include <osix/parser/parserdbg.hpp>

/*                              COMPILER MACROS                             */
#pragma warning(push)
#pragma warning(disable: 4305)

/*                                  CLASSES                                 */
class ParserTest
{
protected:
  OSIX::Parser* parser;
  OSIX::ParserDbg* parserDbg;

  void constructLR0Grammar()
  {
    OSIX::Parser& parser = *ParserTest::parser;

    // from Modern Compiler Implementation in Java (pg 58, Grammar 3.20)
    OSid TEST_SPACE, TEST_NEWLINE, TEST_CARAIGE_RETURN, TEST_TAB;
    OSid TEST_LEFT_PARENTHESIS, TEST_RIGHT_PARENTHESIS, TEST_COMMA;
    OSid TEST_X;
    OSid TEST_SSTART, TEST_S, TEST_L;

    parser.beginGrammar();
      // Raw
      parser.beginRaw();
      parser.endRaw();

      // Nil
      parser.beginNil();
        TEST_SPACE             = parser.charToken("space",          ' ');
        TEST_NEWLINE           = parser.charToken("newline",        '\n');
        TEST_CARAIGE_RETURN    = parser.charToken("caraige_return", '\r');
        TEST_TAB               = parser.charToken("tab",            '\t');
      parser.endNil();

      // Lex
      parser.beginLex();
        parser.beginLexSymbols();
          TEST_LEFT_PARENTHESIS  = parser.charToken("left_parenthesis",  '(');
          TEST_RIGHT_PARENTHESIS = parser.charToken("right_parenthesis", ')');
          TEST_COMMA             = parser.charToken("comma",             ',');
        parser.endLexSymbols();

        parser.beginLexWords();
          TEST_X             = parser.keywordToken("x");
        parser.endLexWords();
      parser.endLex();

      // Productions
      parser.beginLanguage();

        TEST_S = parser.declareProduction("S");

        // L -> S
        TEST_L = parser.beginProduction("L");
          parser.productionToken(TEST_S);
        parser.endProduction();

        // L -> L, S
        parser.beginProduction("L");
          parser.productionToken(TEST_L);
          parser.productionToken(TEST_COMMA);
          parser.productionToken(TEST_S);
        parser.endProduction();

        // S -> (L)
        parser.beginProduction("S");
          parser.productionToken(TEST_LEFT_PARENTHESIS);
          parser.productionToken(TEST_L);
          parser.productionToken(TEST_RIGHT_PARENTHESIS);
        parser.endProduction();

        // S -> x
        parser.beginProduction("S");
          parser.productionToken(TEST_X);
        parser.endProduction();

        // S' -> S$
        TEST_SSTART = parser.beginProduction("S\'");
          parser.productionToken(TEST_S);
        parser.endProduction();

        parser.startProduction(TEST_SSTART);

      parser.endLanguage();

    parser.endGrammar();
  }

  void constructLR1Grammar()
  {
    OSIX::Parser& parser = *ParserTest::parser;

    // from Modern Compiler Implementation in Java (pg 65, Grammar 3.26)
    OSid TEST_SPACE, TEST_NEWLINE, TEST_CARAIGE_RETURN, TEST_TAB;
    OSid TEST_ASSIGN, TEST_ASTERISK;
    OSid TEST_X;
    OSid TEST_SSTART, TEST_S, TEST_V, TEST_E;

    parser.beginGrammar();
      // Raw
      parser.beginRaw();
      parser.endRaw();

      // Nil
      parser.beginNil();
        TEST_SPACE             = parser.charToken("space",          ' ');
        TEST_NEWLINE           = parser.charToken("newline",        '\n');
        TEST_CARAIGE_RETURN    = parser.charToken("caraige_return", '\r');
        TEST_TAB               = parser.charToken("tab",            '\t');
      parser.endNil();

      // Lex
      parser.beginLex();
        parser.beginLexSymbols();
          TEST_ASSIGN   = parser.charToken("assign",  '=');
          TEST_ASTERISK = parser.charToken("asterisk", '*');
        parser.endLexSymbols();

        parser.beginLexWords();
          //TEST_X             = parser.keywordToken("x");
        parser.endLexWords();
      parser.endLex();

      // Productions
      parser.beginLanguage();

        TEST_E = parser.declareProduction("E");

        // V -> * E
        TEST_V = parser.beginProduction("V");
          parser.productionToken(TEST_ASTERISK);
          parser.productionToken(TEST_E);
        parser.endProduction();

        // V -> identifier
        // V -> x
        parser.beginProduction("V");
          //parser.productionToken(TEST_X);
          parser.productionIdentifierDecl("var");
        parser.endProduction();

        // E -> V
        parser.beginProduction("E");
          parser.productionToken(TEST_V);
        parser.endProduction();

        // S -> E
        TEST_S = parser.beginProduction("S");
          parser.productionToken(TEST_E);
        parser.endProduction();

        // S -> V = E
        parser.beginProduction("S");
          parser.productionToken(TEST_V);
          parser.productionToken(TEST_ASSIGN);
          parser.productionToken(TEST_E);
        parser.endProduction();

        // S' -> S$
        TEST_SSTART = parser.beginProduction("S\'");
          parser.productionToken(TEST_S);
        parser.endProduction();

        parser.startProduction(TEST_SSTART);

      parser.endLanguage();

    parser.endGrammar();
  }

  void constructLR1GrammarC()
  {
    OSIX::Parser& parser = *ParserTest::parser;

    // from Modern Compiler Implementation in Java (pg 65, Grammar 3.26)
    OSid TEST_SPACE, TEST_NEWLINE, TEST_CARRIAGE_RETURN, TEST_TAB;
    OSid TEST_EQ, TEST_LTE, TEST_GTE, TEST_LT, TEST_GT, TEST_ANDAND, TEST_OROR, TEST_NOT, TEST_AND, TEST_OR, TEST_ASSIGN, TEST_ASTERISK, TEST_COLON, TEST_QMARK;
    OSid TEST_IF, TEST_WHILE;
    OSid TEST_EXP;

    parser.beginGrammar();
      // Raw
      parser.beginRaw();
      parser.endRaw();

      // Nil
      parser.beginNil();
        TEST_SPACE             = parser.charToken("space",          ' ');
        TEST_NEWLINE           = parser.charToken("newline",        '\n');
        TEST_CARRIAGE_RETURN   = parser.charToken("carriage_return", '\r');
        TEST_TAB               = parser.charToken("tab",            '\t');
      parser.endNil();

      // Lex
      parser.beginLex();
        parser.beginLexSymbols();
          // Boolean operators
          TEST_EQ   = parser.stringToken("eq",  "==");
          TEST_LTE  = parser.stringToken("lte", "<=");
          TEST_GTE  = parser.stringToken("gte", ">=");
          TEST_LT   = parser.charToken("lt",    '<');
          TEST_GT   = parser.charToken("gt",    '>');

          TEST_ANDAND  = parser.stringToken("andand",  "&&");
          TEST_OROR    = parser.stringToken("oror",    "||");
          TEST_NOT     = parser.stringToken("not",     "!");

          TEST_AND  = parser.charToken("and",    '&');
          TEST_OR   = parser.charToken("gt",     '>');

          TEST_ASSIGN   = parser.charToken("assign",   '=');
          TEST_ASTERISK = parser.charToken("asterisk", '*');
          TEST_COLON    = parser.charToken("colon",    ':');
          TEST_QMARK    = parser.charToken("qmark",    '?');
        parser.endLexSymbols();

        parser.beginLexWords();
          TEST_IF             = parser.keywordToken("if");
        parser.endLexWords();
      parser.endLex();

      // Productions
      parser.beginLanguage();

        /*// Exp -> identifier
        TEST_EXP = parser.beginProduction("Exp")
          parser.productionIdentifier();
        parser.endProduction();

        // Exp -> number
        parser.beginProduction("Exp")

        parser.endProduction();

        // Exp -> Exp * Exp
        parser.beginProduction("Exp");
        parser.endProduction();

        // Exp -> Exp / Exp
        parser.beginProduction("Exp");
        parser.endProduction();

        // Exp -> Exp + Exp
        parser.beginProduction("Exp");
        parser.endProduction();

        // Exp -> Exp - Exp
        parser.beginProduction("Exp");
        parser.endProduction();

        // Exp -> Exp = Exp
        parser.beginProduction("Exp");
        parser.endProduction("Exp");*/


        //TEST_E = parser.declareProduction("E");

        /*


        Exp ->




        // V -> * E
        TEST_V = parser.beginProduction("V");
          parser.productionToken(TEST_ASTERISK);
          parser.productionProduction(TEST_E);
        parser.endProduction();

        // V -> x
        parser.beginProduction("V");
          parser.productionToken(TEST_X);
        parser.endProduction();

        // E -> V
        parser.beginProduction("E");
          parser.productionProduction(TEST_V);
        parser.endProduction();

        // S -> E
        TEST_S = parser.beginProduction("S");
          parser.productionProduction(TEST_E);
        parser.endProduction();

        // S -> V = E
        parser.beginProduction("S");
          parser.productionProduction(TEST_V);
          parser.productionToken(TEST_ASSIGN);
          parser.productionProduction(TEST_E);
        parser.endProduction();

        // S' -> S$
        TEST_SSTART = parser.beginProduction("S\'");
          parser.productionProduction(TEST_S);
        parser.endProduction();

        parser.startProduction(TEST_SSTART);*/

      parser.endLanguage();

    parser.endGrammar();
  }

public:

  virtual void init()
  {
    // Create parser
    parser = OSIX::parserInit();
    parserDbg = (OSIX::ParserDbg*)parser->debugInit();

    //parser->setOutputStream(stdout);

    // Construct grammar
    constructLR1Grammar();

#ifdef _DEBUG
    parserDbg->debugOutputTokens();
    parserDbg->debugOutputProductions();
    parserDbg->debugOutputGrammar();
#endif

    //const char* parseString = "x = ? *x";
    const char* parseString = "x = *x";

    OSobject parseResult = parser->parseString(parseString);

    parserDbg->debugOutputParseResult(parseResult);

    parser->delObject(parseResult);
  }

  virtual bool main()
  {
    return false;
  }

  virtual void end()
  {
  }

  ParserTest() {}

} application;

int main()
{
  application.init();
  while(application.main()) {}
  application.end();
  return 0;
}

/*                              COMPILER MACROS                             */
#pragma warning(pop)
