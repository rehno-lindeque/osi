#ifndef __OSIX_PARSER_H__
#define __OSIX_PARSER_H__
//////////////////////////////////////////////////////////////////////////////
//
//    PARSER.H
//
//    Ver 0.2 (2007-11-10)
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Main OpenParser C interface.
*/
/*                                 INCLUDES                                 */
#include "../../osi/os.h"

/*                                 CONSTANTS                                */
#define PARSER_VERSION "0.2"
#define PARSER_TOKEN_VALUE_EOF "\x01"

/*                                   TYPES                                  */
struct OSIX_ParseMatch
{
  OSuint16  offset;
  OSuint16  length;
  OSid id;
};

/*enum OSIX_PARSER_SEQUENCE_MULTIPLIER
{
  SINGLE_REQUIRED,
  SINGLE_OPTIONAL,
  MULTI_REQUIRED,
  MULTI_OPTIONAL
};*/

enum OSIX_PARSER_BOUNDED_LINETYPE
{
  SINGLE_LINE,
  MULTI_LINE
};

enum OSIX_PARSER_LITERAL_TOKEN
{
  NUMERIC_LITERAL
};

#ifdef __cplusplus
extern "C" {
#endif

/*                                     API                                  */
/* grammar */
OSobject OSI_API_C_CALL beginGrammar();
void OSI_API_C_CALL endGrammar();

/* raw string */
void OSI_API_C_CALL beginRaw();
void OSI_API_C_CALL endRaw();

/* nil tokens */
void OSI_API_C_CALL beginNil();
void OSI_API_C_CALL endNil();

/* lexical tokens */
void OSI_API_C_CALL beginLex();
void OSI_API_C_CALL endLex();

void OSI_API_C_CALL beginLexSymbols();
void OSI_API_C_CALL endLexSymbols();

void OSI_API_C_CALL beginLexWords();
void OSI_API_C_CALL endLexWords();

/* lexical & nill tokens */
OSid OSI_API_C_CALL stringToken(const OSchar* tokenName, const OSchar* value);
OSid OSI_API_C_CALL charToken(const OSchar* tokenName, char value);
inline OSid OSI_API_C_CALL keywordToken(const OSchar* keyword) { return stringToken(keyword, keyword); }
OSid OSI_API_C_CALL boundedToken(const OSchar* tokenName, const OSchar* leftBoundingValue, const OSchar* rightBoundingValue, OSIX_PARSER_BOUNDED_LINETYPE lineType);

/* productions */
void OSI_API_C_CALL beginLanguage(); // todo: rename to beginGrammar()
void OSI_API_C_CALL endLanguage();

OSid OSI_API_C_CALL beginProduction(const OSchar* productionName);
void OSI_API_C_CALL endProduction();

//void OSI_API_C_CALL productionProduction(OSid production); // todo: merge production, token, raw to symbol (and do something with identifier to give it "type"?)
//void OSI_API_C_CALL productionRaw(OSid token);
void OSI_API_C_CALL productionToken(OSid token);
OSid OSI_API_C_CALL productionTokenName(const OSchar* tokenName);
OSid OSI_API_C_CALL productionIdentifierDecl(const OSchar* typeName);
void OSI_API_C_CALL productionIdentifierRef(OSid type);
OSid OSI_API_C_CALL productionIdentifierRefName(const OSchar* typeName);
void OSI_API_C_CALL productionLiteralToken(enum OSIX_PARSER_LITERAL_TOKEN literalToken);

OSid OSI_API_C_CALL declareProduction(const OSchar* productionName);

void OSI_API_C_CALL startProduction(OSid production);

/* parse */
OSobject OSI_API_C_CALL parseFile(const OSchar* fileName);
OSobject OSI_API_C_CALL parseString(const OSchar* stringBuffer);
OSid OSI_API_C_CALL getParseTokens(OSobject parseResult);

/* precedence */
void OSI_API_C_CALL tokenNamePrecedence(const OSchar* token1Name, const OSchar* token2Name);
void OSI_API_C_CALL tokenPrecedence(OSid token1, OSid token2);

/* miscelaneous */
void OSI_API_C_CALL delObject(OSobject object);

/* debug */
void* OSI_API_C_CALL debugInit();
void OSI_API_C_CALL parserInit();

#ifdef __cplusplus
}
#endif

#endif
