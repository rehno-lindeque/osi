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
 
    TODO:
      This needs a C port for testing the C interface
*/
/*                                  INCLUDES                                */

#ifdef _WIN32
  #include <windows.h>
#endif

/* Base */
#include <base/common/types.h>

/* OpenParser */
#if defined(__cplusplus)
# include <parsergrammarloader.hpp>
# include <osix/parser/parserdbg.hpp>
#else
# include <osix/parser/parser.h>
# include <parsergrammarloader.h>
#endif

/*                              COMPILER MACROS                             */
#pragma warning(push)
#pragma warning(disable: 4305)

/*                                  CLASSES                                 */
class ParserTest
{
protected:
  OSIX::Parser* parser;
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

    using namespace Parser;
    GrammarLoader::constructGrammar(*parser, grammarStr, sizeof(grammarStr), null, 0);
  }

public:

  virtual void init()
  {
    // Create parser
#if defined(__cplusplus)    
    parser = OSIX::parserInit();
    parserDbg = (OSIX::ParserDbg*) parser->debugInit();
#else
    parserInit();
    parserDbg = (OSIX::ParserDbg*) debugInit();
#endif
    

    //parser->setOutputStream(stdout);

    // Construct grammar
    constructLR1Grammar();

#if defined(__cplusplus) && defined(_DEBUG)
    parserDbg->debugOutputTokens();
    parserDbg->debugOutputProductions();
    parserDbg->debugOutputGrammar();
#endif

    //const char* parseString = "x = ? *x";
    const char* parseStr = "x = *x";
#if defined(__cplusplus)
    OSobject parseResult = parser->parseString(parseStr);
#else
    OSobject parseResult = parseString(parseStr);
#endif
    
#if defined(__cplusplus)
    parserDbg->debugOutputParseResult(parseResult);
    //parser->delObject(parseResult);
#endif
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
