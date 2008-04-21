#ifndef __OSIX_PARSER_HPP__
#define __OSIX_PARSER_HPP__
//////////////////////////////////////////////////////////////////////////////
//
//    PARSER.HPP
//
//    Ver 0.1 (2007-06-11)
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Main OpenParser C++ interface.
*/
/*                                 INCLUDES                                 */
#include "../../osi/os.hpp"

/*                                 CONSTANTS                                */
#define PARSER_VERSION "0.1"
#define PARSER_TOKEN_VALUE_EOF "\x01"

/*                                   TYPES                                  */
struct ParseMatch
{
  OSuint16  offset;
  OSuint16  length;
  OSid id;
};

enum PARSER_SEQUENCE_MULTIPLIER
{
  SINGLE_REQUIRED,
  SINGLE_OPTIONAL,
  MULTI_REQUIRED,
  MULTI_OPTIONAL
};

enum PARSER_BOUNDED_LINETYPE
{
  SINGLE_LINE,
  MULTI_LINE
};

/*                                     API                                  */
class Parser
{
protected:
  inline Parser() {}
  inline Parser(const Parser&) {}

public:
  /* grammar */
  OSI_DYNAMIC_METHOD OSobject OSI_API_CALL beginGrammar();
  OSI_DYNAMIC_METHOD void OSI_API_CALL endGrammar();

  /* raw string */
  OSI_DYNAMIC_METHOD void OSI_API_CALL beginRaw();
  OSI_DYNAMIC_METHOD void OSI_API_CALL endRaw();

  /* nil tokens */
  OSI_DYNAMIC_METHOD void OSI_API_CALL beginNil();
  OSI_DYNAMIC_METHOD void OSI_API_CALL endNil();

  /* lexical tokens */
  OSI_DYNAMIC_METHOD void OSI_API_CALL beginLex();
  OSI_DYNAMIC_METHOD void OSI_API_CALL endLex();

  OSI_DYNAMIC_METHOD void OSI_API_CALL beginLexSymbols();
  OSI_DYNAMIC_METHOD void OSI_API_CALL endLexSymbols();

  OSI_DYNAMIC_METHOD void OSI_API_CALL beginLexWords();
  OSI_DYNAMIC_METHOD void OSI_API_CALL endLexWords();

  /* global */
  OSI_DYNAMIC_METHOD void OSI_API_CALL beginGlobal();
  OSI_DYNAMIC_METHOD void OSI_API_CALL endGlobal();

  /* lexical & nill tokens */
  OSI_DYNAMIC_METHOD OSid OSI_API_CALL stringToken(const char* tokenName, const char* value);
  OSI_DYNAMIC_METHOD OSid OSI_API_CALL charToken(const char* tokenName, char value);
  OSI_INLINE_METHOD  OSid OSI_API_CALL keywordToken(const char* keyword) { return stringToken(keyword, keyword); }
  OSI_DYNAMIC_METHOD OSid OSI_API_CALL boundedToken(const char* tokenName, const char* leftBoundingValue, const char* rightBoundingValue, PARSER_BOUNDED_LINETYPE lineType = MULTI_LINE);

  /* statements */
  OSI_DYNAMIC_METHOD void OSI_API_CALL beginStatements();
  OSI_DYNAMIC_METHOD void OSI_API_CALL endStatements();

  /* sequence statement */
  OSI_DYNAMIC_METHOD OSid OSI_API_CALL beginSequence(const char* statementName);
  OSI_DYNAMIC_METHOD void OSI_API_CALL endSequence();

  OSI_DYNAMIC_METHOD void OSI_API_CALL sequenceStatement(OSid statement, PARSER_SEQUENCE_MULTIPLIER multiplier = SINGLE_REQUIRED);
  OSI_DYNAMIC_METHOD void OSI_API_CALL sequenceToken(OSid token, PARSER_SEQUENCE_MULTIPLIER multiplier = SINGLE_REQUIRED);
  OSI_DYNAMIC_METHOD void OSI_API_CALL sequenceIdentifier();
  OSI_DYNAMIC_METHOD void OSI_API_CALL sequenceRaw(OSid token);

  /* switch statement */
  OSI_DYNAMIC_METHOD OSid OSI_API_CALL beginSwitch(const char* statementName);
  OSI_DYNAMIC_METHOD void OSI_API_CALL endSwitch();

  OSI_DYNAMIC_METHOD void OSI_API_CALL switchStatement(OSid statement);
  OSI_DYNAMIC_METHOD void OSI_API_CALL switchToken(OSid token);

  /* parse */
  OSI_DYNAMIC_METHOD OSobject OSI_API_CALL parseFile(const char* fileName);
  OSI_DYNAMIC_METHOD OSid OSI_API_CALL getParseTokens(OSobject parseResult);

  /* debug */
  OSI_DYNAMIC_METHOD void* OSI_API_CALL debugInit();
};

Parser* OSI_API_CALL parserInit();

#endif
