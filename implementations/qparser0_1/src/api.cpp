//////////////////////////////////////////////////////////////////////////////
//
//    API.CPP
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    IMPLEMENTATION:
      + Parsing happens in packets of characters. Packet size must be a 
        power of two.

    TODO:
      + Optimize using SSE and binary operations
*/

/*                              COMPILER MACROS                             */
#pragma warning(push)

/*                                 INCLUDES                                 */
#include "api.h"

/*                              COMPILER MACROS                             */
#define _this (*(QParser::Parser*)this)

#define cast_id     QParser::Parser::cast_id
#define cast_object QParser::Parser::cast_object

/*                              IMPLEMENTATION                              */
OSobject OSI_API_CALL Parser::beginGrammar()
{
  if(_this.grammar != null)
    return 0; // error

  QParser::Grammar* grammarObject = _this.grammar = _this.beginObject<QParser::Grammar>();
  return cast_object(grammarObject); 
}

void OSI_API_CALL Parser::endGrammar()
{
  QParser::Grammar* grammarObject = _this.endObject<QParser::Grammar>();
}

void OSI_API_CALL Parser::beginRaw()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeTokenType = QParser::Grammar::RAW_TOKEN;
}

void OSI_API_CALL Parser::endRaw()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->constructTokens();
  grammarObject->activeTokenType = (QParser::Grammar::TokenType)0;
}

void OSI_API_CALL Parser::beginNil()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeTokenType = QParser::Grammar::NIL_TOKEN;
}

void OSI_API_CALL Parser::endNil()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->constructTokens();
  grammarObject->activeTokenType = (QParser::Grammar::TokenType)0;
}

void OSI_API_CALL Parser::beginLex()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeTokenType = QParser::Grammar::LEX_TOKEN;
}

void OSI_API_CALL Parser::endLex()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeTokenType = (QParser::Grammar::TokenType)0;
}

void OSI_API_CALL Parser::beginLexSymbols()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeSubTokenType = QParser::Grammar::LEX_SYMBOL; 
}

void OSI_API_CALL Parser::endLexSymbols()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->constructTokens();
  grammarObject->activeSubTokenType = (QParser::Grammar::SubTokenType)0;
}

void OSI_API_CALL Parser::beginLexWords()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeSubTokenType = QParser::Grammar::LEX_WORD;
}

void OSI_API_CALL Parser::endLexWords()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->constructTokens();
  grammarObject->activeSubTokenType = (QParser::Grammar::SubTokenType)0;
}

void OSI_API_CALL Parser::beginGlobal()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->beginGlobal();
}

void OSI_API_CALL Parser::endGlobal()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->constructGlobalStatements();
}

OSid OSI_API_CALL Parser::stringToken(const char* tokenName, const char* value)
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  return grammarObject->stringToken(tokenName, value);
}

OSid OSI_API_CALL Parser::charToken(const char* tokenName, char value)
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  return grammarObject->charToken(tokenName, value);
}

OSid OSI_API_CALL Parser::boundedToken(const char* tokenName, const char* leftBoundingValue, const char* rightBoundingValue, PARSER_BOUNDED_LINETYPE lineType)
{

  // todo: handle line type.....

  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  return grammarObject->boundedToken(tokenName, leftBoundingValue, rightBoundingValue);
}

void OSI_API_CALL Parser::beginStatements()
{}

void OSI_API_CALL Parser::endStatements()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->constructStatements();
}

OSid OSI_API_CALL Parser::beginSequence(const char* statementName)
{
  return _this.getActiveObject<QParser::Grammar>()->beginSequence(statementName);
}

void OSI_API_CALL Parser::endSequence()
{
  return _this.getActiveObject<QParser::Grammar>()->endSequence();
}

void OSI_API_CALL Parser::sequenceStatement(OSid statement, PARSER_SEQUENCE_MULTIPLIER multiplier)
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  return grammarObject->sequenceElement(statement, multiplier);
}

void OSI_API_CALL Parser::sequenceToken(OSid token, PARSER_SEQUENCE_MULTIPLIER multiplier)
{
  return _this.getActiveObject<QParser::Grammar>()->sequenceElement(token, multiplier);
}

void OSI_API_CALL Parser::sequenceIdentifier()
{
  return _this.getActiveObject<QParser::Grammar>()->sequenceElement(QParser::Grammar::ID_UNKNOWN_WORD_TOKEN, SINGLE_REQUIRED);
}

void OSI_API_CALL Parser::sequenceRaw(OSid token)
{
  return _this.getActiveObject<QParser::Grammar>()->sequenceElement(token, SINGLE_REQUIRED);
}

OSid OSI_API_CALL Parser::beginSwitch(const char* statementName)
{
  return _this.getActiveObject<QParser::Grammar>()->beginSwitch(statementName);
}

void OSI_API_CALL Parser::endSwitch()
{
  return _this.getActiveObject<QParser::Grammar>()->endSwitch();
}

void OSI_API_CALL Parser::switchStatement(OSid statement)
{
  _this.getActiveObject<QParser::Grammar>()->switchElement(statement);
}

void OSI_API_CALL Parser::switchToken(OSid token)
{
  _this.getActiveObject<QParser::Grammar>()->switchElement(token);
}

OSobject OSI_API_CALL Parser::parseFile(const char* fileName)
{
  QParser::Grammar* grammarObject = _this.grammar;

  return grammarObject->parseFile(fileName);
}

OSid OSI_API_CALL Parser::getParseTokens(OSobject parseResult)
{
  // todo
  return 0;
}

void* OSI_API_CALL Parser::debugInit()
{
# ifdef _DEBUG
    return &_this.parserDbg;
# else
    return null;
# endif
}

#ifdef _DEBUG
  void OSI_API_CALL QParser::ParserDbg::debugOutputTokens()
  {
    QParser::Grammar* grammarObject = _this.grammar;
    grammarObject->debugOutputTokens();
  }

  void QParser::ParserDbg::debugOutputStatements()
  {
    QParser::Grammar* grammarObject = _this.grammar;
    grammarObject->debugOutputStatements();
  }

  void QParser::ParserDbg::debugOutputParseResult(OSobject parseResult)
  {
    QParser::Grammar* grammarObject = _this.grammar;
    grammarObject->debugOutputParseResult(parseResult);
  }
#endif

Parser* OSI_API_CALL parserInit()
{
  return (::Parser*)new QParser::Parser();
};

/*                                  MACROS                                  */
#pragma warning(pop)
