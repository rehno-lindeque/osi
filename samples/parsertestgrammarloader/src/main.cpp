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
      OpenParser unit test
*/
/*                                  INCLUDES                                */

#ifdef _WIN32
  #include <windows.h>
#endif

/* Base */
#include <base/common/types.h>

/* OpenParser */
#include <osix/parser/parser.h>
#include <osi/os.hpp>
#include <osix/parser/parserdbg.hpp>
#include <parsergrammarloader.h>

/*                              COMPILER MACROS                             */
#pragma warning(push)
#pragma warning(disable: 4305)

/*                                  CLASSES                                 */
class ParserTest
{
protected:
  //OSIX::Parser* parser;
  //void* parser;
  OSIX::ParserDbg* parserDbg;

  void constructLR1Grammar()
  {
    //OSIX::Parser& parser = *ParserTest::parser;

    /*
    static const OSuint TEST_TOKEN_IDS_SIZE = 4+8+2+2+4;

    union ContextTokenIds
    {
      struct
      {
        //OSid rawString
        OSid nilSpace, nilNewline, nilCarriageReturn, nilTab;
        OSid lexEq, lexLte, lexGte, lexLt, lexGt, lexAndAnd, lexOrOr, lexNot;
        OSid lexAnd, lexLOr;
        OSid lexPlus, lexMinus;
        OSid lexAssign, lexAsterisk, lexColon, lexQMark;
      };

      OSid tokens[CONTEXT_TOKEN_IDS_SIZE];
    };

    /*input_file = fopen(input_filename, "r");
      if(!input_file)
      {
        fprintf(stderr, "Unable to open input file %s\n", input_filename);
        exit(1);
      }*/



    // from Modern Compiler Implementation in Java (pg 65, Grammar 3.26)
    const char grammarStr[] =
      "raw"
      "{"
      "}"

      "nil"
      "{"
      "    space          = \" \""
      "    newline        = \"\\n\""
      "    caraige_return = \"\\r\""
      "    tab            = \"\\t\""
      "}"

      "lex"
      "{"
      "  symbols"
      "  {"
      "    assign   = \"=\""
      "    asterisk = \"*\""
      "    colon    = \":\""
      "    qmark    = \"?\""
      "  }"
      "  words"
      "  {"
      "    if       = \"if\""
      "  }"
      "}"

      "grammar"
      "{"
      "  V { asterisk E }"
      "  V { #var }"
      "  E { V }"
      "  S { E }"
      "  S { V assign E }"
      "  S\' { S }"
      "}";

    parserConstructGrammar(grammarStr, sizeof(grammarStr));
  }

public:

  virtual void init()
  {
    // Create parser
    parserInit();
    //parser = OSIX::parserInit();
    parserDbg = (OSIX::ParserDbg*) debugInit();

    //parser->setOutputStream(stdout);

    // Construct grammar
    constructLR1Grammar();

#ifdef _DEBUG
    parserDbg->debugOutputTokens();
    parserDbg->debugOutputProductions();
    parserDbg->debugOutputGrammar();
#endif

    //const char* parseString = "x = ? *x";
    const char* parseStr = "x = *x";
    OSobject parseResult = parseString(parseStr);

    parserDbg->debugOutputParseResult(parseResult);

    //parser->delObject(parseResult);
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
