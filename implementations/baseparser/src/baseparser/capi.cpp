//////////////////////////////////////////////////////////////////////////////
//
//    CAPI.CPP
//
//    Copyright © 2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              IMPLEMENTATION                              */
#if defined(OSI_C_STATIC_BUILD) || defined(OSI_C_DYNAMIC_BUILD)

#include <osix/parser/parser.hpp>
#include <osix/parser/parser.h>
#include <base/common/types.h>

OSIX::Parser *parser = null;

OSobject OSI_API_C_CALL beginGrammar() { return parser->beginGrammar(); }
void OSI_API_C_CALL endGrammar() { parser->endGrammar(); }

/* raw string */
void OSI_API_C_CALL beginRaw() { parser->beginRaw(); }
void OSI_API_C_CALL endRaw() { parser->endRaw(); }

/* nil tokens */
void OSI_API_C_CALL beginNil() { parser->beginNil(); }
void OSI_API_C_CALL endNil() { parser->endNil(); }

/* lexical tokens */
void OSI_API_C_CALL beginLex() { parser->beginLex(); }
void OSI_API_C_CALL endLex() { parser->endLex(); }

void OSI_API_C_CALL beginLexSymbols() { parser->beginLexSymbols(); }
void OSI_API_C_CALL endLexSymbols() { parser->endLexSymbols(); }

void OSI_API_C_CALL beginLexWords() { parser->beginLexWords(); }
void OSI_API_C_CALL endLexWords() { parser->endLexWords(); }

/* lexical & nill tokens */
OSid OSI_API_C_CALL stringToken(const OSchar* tokenName, const OSchar* value) { return parser->stringToken(tokenName, value); }
OSid OSI_API_C_CALL charToken(const OSchar* tokenName, char value) { return parser->charToken(tokenName, value); }
OSid OSI_API_C_CALL boundedToken(const OSchar* tokenName, const OSchar* leftBoundingValue, const OSchar* rightBoundingValue, OSIX_PARSER_BOUNDED_LINETYPE lineType) { return parser->boundedToken(tokenName, leftBoundingValue, rightBoundingValue, (OSIX::PARSER_BOUNDED_LINETYPE)lineType); }

/* productions */
void OSI_API_C_CALL beginLanguage() { parser->beginLanguage(); }
void OSI_API_C_CALL endLanguage() { parser->endLanguage(); }

OSid OSI_API_C_CALL beginProduction(const OSchar* productionName) { return parser->beginProduction(productionName); }
void OSI_API_C_CALL endProduction() { parser->endProduction(); }

//void OSI_API_C_CALL productionProduction(OSid production) { parser->productionProduction(production); }
//void OSI_API_C_CALL productionRaw(OSid token) { parser->productionRaw(token); }
void OSI_API_C_CALL productionToken(OSid token) { parser->productionToken(token); }
OSid OSI_API_C_CALL productionTokenName(const OSchar* tokenName) { return parser->productionToken(tokenName); }
OSid OSI_API_C_CALL productionIdentifierDecl(const OSchar* typeName) { return parser->productionIdentifierDecl(typeName); }
void OSI_API_C_CALL productionIdentifierRef(OSid type) { parser->productionIdentifierRef(type); }
OSid OSI_API_C_CALL productionIdentifierRefName(const OSchar* typeName) { return parser->productionIdentifierRef(typeName); }
void OSI_API_C_CALL productionLiteralToken(enum OSIX_PARSER_LITERAL_TOKEN literalToken) { parser->productionLiteralToken((OSIX::PARSER_LITERAL_TOKEN)literalToken); }

OSid OSI_API_C_CALL declareProduction(const OSchar* productionName) { return parser->declareProduction(productionName); }

void OSI_API_C_CALL startProduction(OSid production) { parser->startProduction(production); }

/* parse */
OSobject OSI_API_C_CALL parseFile(const OSchar* fileName) { return parser->parseFile(fileName); }
OSobject OSI_API_C_CALL parseString(const OSchar* stringBuffer) { return parser->parseString(stringBuffer); }
OSid OSI_API_C_CALL getParseTokens(OSobject parseResult) { return parser->getParseTokens(parseResult); }

/* precedence */
void OSI_API_C_CALL tokenNamePrecedence(const OSchar* token1Name, const OSchar* token2Name) { return parser->precedence(token1Name, token2Name); }
void OSI_API_C_CALL tokenPrecedence(OSid token1, OSid token2) { return parser->precedence(token1, token2); }

/* miscelaneous */
const OSchar* OSI_API_C_CALL getTokenName(OSid token) { return parser->getTokenName(token); }
void OSI_API_C_CALL delObject(OSobject object) { parser->delObject(object); }

/* debug */
void* OSI_API_C_CALL debugInit() { return parser->debugInit(); }

void OSI_API_C_CALL parserInit() { parser = OSIX::parserInit(); }

#endif
