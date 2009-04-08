#ifndef __OSIX_PARSER_HPP__
#define __OSIX_PARSER_HPP__
//////////////////////////////////////////////////////////////////////////////
//
//    PARSER.HPP
//
//    Ver 0.3 (2009-04-08)
//
//    Copyright © 2007-2009, Rehno Lindeque. All rights reserved.
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
#define PARSER_VERSION "0.3"
#define PARSER_TOKEN_VALUE_EOF "\x01"

namespace OSIX
{
  /*                                   TYPES                                  */
  struct ParseMatch
  {
    OSuint16  offset;
    OSuint16  length;
    OSid id;
  };

  /*enum PARSER_SEQUENCE_MULTIPLIER
  {
    SINGLE_REQUIRED,
    SINGLE_OPTIONAL,
    MULTI_REQUIRED,
    MULTI_OPTIONAL
  };*/

  enum PARSER_BOUNDED_LINETYPE
  {
    SINGLE_LINE,
    MULTI_LINE
  };

  enum PARSER_LITERAL_TOKEN
  {
    NUMERIC_LITERAL
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

    /* lexical & nill tokens */
    OSI_DYNAMIC_METHOD OSid OSI_API_CALL stringToken(const OSchar* tokenName, const OSchar* value);
    OSI_DYNAMIC_METHOD OSid OSI_API_CALL charToken(const OSchar* tokenName, char value);
    OSI_INLINE_METHOD  OSid OSI_API_CALL keywordToken(const OSchar* keyword) { return stringToken(keyword, keyword); }
    OSI_DYNAMIC_METHOD OSid OSI_API_CALL boundedToken(const OSchar* tokenName, const OSchar* leftBoundingValue, const OSchar* rightBoundingValue, enum PARSER_BOUNDED_LINETYPE lineType = MULTI_LINE);

    /* productions */
    OSI_DYNAMIC_METHOD void OSI_API_CALL beginLanguage(); // todo: rename to beginGrammar()
    OSI_DYNAMIC_METHOD void OSI_API_CALL endLanguage();

    OSI_DYNAMIC_METHOD OSid OSI_API_CALL beginProduction(const OSchar* productionName);
    OSI_DYNAMIC_METHOD void OSI_API_CALL endProduction();

    OSI_DYNAMIC_METHOD void OSI_API_CALL productionToken(OSid token);
    OSI_DYNAMIC_METHOD OSid OSI_API_CALL productionToken(const OSchar* tokenName);
    OSI_DYNAMIC_METHOD OSid OSI_API_CALL productionIdentifierDecl(const OSchar* typeName);
    OSI_DYNAMIC_METHOD void OSI_API_CALL productionIdentifierRef(OSid type);
    OSI_DYNAMIC_METHOD OSid OSI_API_CALL productionIdentifierRef(const OSchar* typeName);
    OSI_DYNAMIC_METHOD void OSI_API_CALL productionLiteralToken(PARSER_LITERAL_TOKEN literalToken = NUMERIC_LITERAL);

    OSI_DYNAMIC_METHOD OSid OSI_API_CALL declareProduction(const OSchar* productionName);

    OSI_DYNAMIC_METHOD void OSI_API_CALL startProduction(OSid production);

    /* parse */
    OSI_DYNAMIC_METHOD OSobject OSI_API_CALL parseFile(const OSchar* fileName);
    OSI_DYNAMIC_METHOD OSobject OSI_API_CALL parseString(const OSchar* stringBuffer);
    OSI_DYNAMIC_METHOD OSobject OSI_API_CALL getInputStream(OSobject parseResult);
    OSI_DYNAMIC_METHOD OSobject OSI_API_CALL getLexStream(OSobject parseResult);
    OSI_DYNAMIC_METHOD OSobject OSI_API_CALL getParseStream(OSobject parseResult);

    /* precedence */
    OSI_DYNAMIC_METHOD void OSI_API_CALL precedence(const OSchar* token1Name, const OSchar* token2Name);
    OSI_DYNAMIC_METHOD void OSI_API_CALL precedence(OSid token1, OSid token2);

    /* miscelaneous */
    OSI_DYNAMIC_METHOD const OSchar* OSI_API_CALL getTokenName(OSid token);
    OSI_DYNAMIC_METHOD OSbool OSI_API_CALL isIdentifier(OSid token);
    OSI_DYNAMIC_METHOD OSbool OSI_API_CALL isNumeric(OSid token);
    OSI_DYNAMIC_METHOD void OSI_API_CALL getMatchText(OSobject parseResult, const ParseMatch& match, OSchar* text);
    OSI_DYNAMIC_METHOD void OSI_API_CALL delObject(OSobject object);
    
    /* debug */
    OSI_DYNAMIC_METHOD void* OSI_API_CALL debugInit();
  };

  Parser* OSI_API_CALL parserInit();
}

#endif
