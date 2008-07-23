#ifdef  __QPARSER_API_H__
#ifndef __QPARSER_API_INL__
#define __QPARSER_API_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    API.INL
//
//    Copyright © 2007-2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              COMPILER MACROS                             */
#define _this (*(QParser::Parser*)this)

#define cast_id     QParser::Parser::cast_id
#define cast_object QParser::Parser::cast_object

/*                              IMPLEMENTATION                              */
OSobject OSI_API_CALL OSIX::Parser::beginGrammar()
{
  if(_this.grammar != null)
    return 0; // error

  QParser::Grammar* grammarObject = _this.grammar = (QParser::Grammar*) _this.beginObject<QParser::GrammarLR1>();
  return cast_object(grammarObject);
}

void OSI_API_CALL OSIX::Parser::endGrammar()
{
  QParser::Grammar* grammarObject = _this.endObject<QParser::Grammar>();
  //QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  if(grammarObject->checkForwardDeclarations())
    grammarObject->constructProductions();
}

void OSI_API_CALL OSIX::Parser::beginRaw()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeTokenType = QParser::Grammar::RAW_TOKEN;
}

void OSI_API_CALL OSIX::Parser::endRaw()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->constructTokens();
  grammarObject->activeTokenType = (QParser::Grammar::TokenType)0;
}

void OSI_API_CALL OSIX::Parser::beginNil()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeTokenType = QParser::Grammar::NIL_TOKEN;
}

void OSI_API_CALL OSIX::Parser::endNil()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->constructTokens();
  grammarObject->activeTokenType = (QParser::Grammar::TokenType)0;
}

void OSI_API_CALL OSIX::Parser::beginLex()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeTokenType = QParser::Grammar::LEX_TOKEN;
}

void OSI_API_CALL OSIX::Parser::endLex()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeTokenType = (QParser::Grammar::TokenType)0;
}

void OSI_API_CALL OSIX::Parser::beginLexSymbols()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeSubTokenType = QParser::Grammar::LEX_SYMBOL;
}

void OSI_API_CALL OSIX::Parser::endLexSymbols()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->constructTokens();
  grammarObject->activeSubTokenType = (QParser::Grammar::SubTokenType)0;
}

void OSI_API_CALL OSIX::Parser::beginLexWords()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeSubTokenType = QParser::Grammar::LEX_WORD;
}

void OSI_API_CALL OSIX::Parser::endLexWords()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->constructTokens();
  grammarObject->activeSubTokenType = (QParser::Grammar::SubTokenType)0;
}

OSid OSI_API_CALL OSIX::Parser::stringToken(const OSchar* tokenName, const OSchar* value)
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  return grammarObject->stringToken(tokenName, value);
}

OSid OSI_API_CALL OSIX::Parser::charToken(const OSchar* tokenName, char value)
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  return grammarObject->charToken(tokenName, value);
}

OSid OSI_API_CALL OSIX::Parser::boundedToken(const OSchar* tokenName, const OSchar* leftBoundingValue, const OSchar* rightBoundingValue, PARSER_BOUNDED_LINETYPE lineType)
{
  // todo: handle line type.....

  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  return grammarObject->boundedToken(tokenName, leftBoundingValue, rightBoundingValue, lineType);
}

void OSI_API_CALL OSIX::Parser::beginLanguage()
{}

void OSI_API_CALL OSIX::Parser::endLanguage()
{
}

OSid OSI_API_CALL OSIX::Parser::beginProduction(const OSchar* productionName)
{
  return _this.getActiveObject<QParser::Grammar>()->beginProduction(productionName);
}

void OSI_API_CALL OSIX::Parser::endProduction()
{
  return _this.getActiveObject<QParser::Grammar>()->endProduction();
}

/*OLD:
void OSI_API_CALL OSIX::Parser::productionProduction(OSid production)
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  return grammarObject->productionNonterminal(production);
}
void OSI_API_CALL OSIX::Parser::productionRaw(OSid token)
{
  return _this.getActiveObject<QParser::Grammar>()->productionTerminal(token);
}*/

void OSI_API_CALL OSIX::Parser::productionToken(OSid token)
{
  _this.getActiveObject<QParser::Grammar>()->productionToken(token);
}

OSid OSI_API_CALL OSIX::Parser::productionToken(const OSchar* tokenName)
{
  return _this.getActiveObject<QParser::Grammar>()->productionToken(tokenName);
}

OSid OSI_API_CALL OSIX::Parser::productionIdentifierDecl(const OSchar* typeName)
{
  return _this.getActiveObject<QParser::Grammar>()->productionIdentifierDecl(typeName);
}

void OSI_API_CALL OSIX::Parser::productionIdentifierRef(OSid type)
{
  _this.getActiveObject<QParser::Grammar>()->productionIdentifierRef(type);
}

OSid OSI_API_CALL OSIX::Parser::productionIdentifierRef(const OSchar* typeName)
{
  return _this.getActiveObject<QParser::Grammar>()->productionIdentifierRef(typeName);
}

void OSI_API_CALL OSIX::Parser::productionLiteralToken(PARSER_LITERAL_TOKEN literalToken)
{
  switch(literalToken)
  {
    case NUMERIC_LITERAL:
      _this.getActiveObject<QParser::Grammar>()->productionToken(QParser::Grammar::ID_CONST_NUM);
      break;
    default:; // error: unkown literal token
  }
}

OSid OSI_API_CALL OSIX::Parser::declareProduction(const OSchar* productionName)
{
  return _this.getActiveObject<QParser::Grammar>()->declareProduction(productionName);
}

void OSI_API_CALL OSIX::Parser::startProduction(OSid production)
{
  _this.getActiveObject<QParser::Grammar>()->grammarStartSymbol(production);
}

OSobject OSI_API_CALL OSIX::Parser::parseFile(const OSchar* fileName)
{
  QParser::Grammar* grammarObject = _this.grammar;

  QParser::Grammar::ParseResult::ParseResult* parseResult = _this.beginObject<QParser::Grammar::ParseResult>();
  grammarObject->parseFile(fileName, *parseResult);
  _this.endObject<QParser::Grammar::ParseResult>();

  return cast_object(parseResult);
}

OSobject OSI_API_CALL OSIX::Parser::parseString(const OSchar* stringBuffer)
{
  QParser::Grammar* grammarObject = _this.grammar;

  QParser::Grammar::ParseResult::ParseResult* parseResult = _this.beginObject<QParser::Grammar::ParseResult>();
  grammarObject->parseString(stringBuffer, *parseResult);
  _this.endObject<QParser::Grammar::ParseResult>();

  return cast_object(parseResult);
}

OSobject OSI_API_CALL OSIX::Parser::getInputStream(OSobject parseResult)
{
  QParser::Grammar::ParseResult& resultObject = *reinterpret_cast<QParser::Grammar::ParseResult*>(parseResult);
  return reinterpret_cast<OSobject>((OSuint32*)&resultObject.inputStream);
}

OSobject OSI_API_CALL OSIX::Parser::getLexStream(OSobject parseResult)
{
  QParser::Grammar::ParseResult& resultObject = *reinterpret_cast<QParser::Grammar::ParseResult*>(parseResult);
  return reinterpret_cast<OSobject>((OSuint32*)&resultObject.lexStream);
}

OSobject OSI_API_CALL OSIX::Parser::getParseStream(OSobject parseResult)
{
  QParser::Grammar::ParseResult& resultObject = *reinterpret_cast<QParser::Grammar::ParseResult*>(parseResult);
  return reinterpret_cast<OSobject>((OSuint32*)&resultObject.parseStream);
}

void OSI_API_CALL OSIX::Parser::precedence(const OSchar* token1Name, const OSchar* token2Name)
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->precedence(token1Name, token2Name);
}

void OSI_API_CALL OSIX::Parser::precedence(OSid token1, OSid token2)
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->precedence(token1, token2);
}

const OSchar* OSI_API_CALL OSIX::Parser::getTokenName(OSid token)
{
  QParser::Grammar* grammarObject = _this.grammar;
  return grammarObject->getTokenName(token).c_str();
}

void OSI_API_CALL OSIX::Parser::delObject(OSobject object)
{
  _this.delObject((Base::Object*)object);
}

void* OSI_API_CALL OSIX::Parser::debugInit()
{
# ifdef _DEBUG
    return &_this.parserDbg;
# else
    return null;
# endif
}

#ifdef _DEBUG
  void OSI_API_CALL OSIX::ParserDbg::debugOutputTokens()
  {
    QParser::Grammar* grammarObject = static_cast<QParser::Parser::ParserDbg*>(this)->getParser().grammar;
    grammarObject->debugOutputTokens();
  }

  void OSIX::ParserDbg::debugOutputProductions()
  {
    QParser::Grammar* grammarObject = ((QParser::Parser::ParserDbg*)this)->getParser().grammar;
    grammarObject->debugOutputProductions();
  }

  void OSIX::ParserDbg::debugOutputGrammar()
  {
    QParser::Grammar* grammarObject = ((QParser::Parser::ParserDbg*)this)->getParser().grammar;
    grammarObject->debugOutputStates();
    grammarObject->debugOutputTable();
  }

  void OSIX::ParserDbg::debugOutputParseResult(OSobject parseResult)
  {
    QParser::Grammar* grammarObject = ((QParser::Parser::ParserDbg*)this)->getParser().grammar;
    grammarObject->debugOutputParseResult(parseResult);
  }
#endif

OSIX::Parser* OSI_API_CALL OSIX::parserInit()
{
  return static_cast<OSIX::Parser*>(new QParser::Parser());
};

#endif
#endif
