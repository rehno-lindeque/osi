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

  //QParser::Grammar* grammarObject = _this.grammar = (QParser::Grammar*) _this.beginObject< QParser::GrammarLRK<2> >();
  QParser::Grammar* grammarObject = _this.grammar = (QParser::Grammar*) _this.beginObject< QParser::GrammarLD >();
  return cast_object(grammarObject);
}

void OSI_API_CALL OSIX::Parser::endGrammar()
{
  QParser::Grammar* grammarObject = _this.endObject<QParser::Grammar>();
  //QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  if(grammarObject->CheckForwardDeclarations())
    grammarObject->ConstructProductions();
}

void OSI_API_CALL OSIX::Parser::beginRaw()
{
  /*QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->ActiveTokenType = QParser::Lexer::TOKENTYPE_RAW;*/
}

void OSI_API_CALL OSIX::Parser::endRaw()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->GetLexer().Build(QParser::Lexer::TOKENTYPE_RAW);
  //grammarObject->constructTokens();
  //grammarObject->activeTokenType = (QParser::Grammar::TokenType)0;
}

void OSI_API_CALL OSIX::Parser::beginNil()
{
  /*QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeTokenType = QParser::Grammar::NIL_TOKEN;*/
}

void OSI_API_CALL OSIX::Parser::endNil()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->GetLexer().Build(QParser::Lexer::TOKENTYPE_NIL);
  //grammarObject->constructTokens();
  //grammarObject->activeTokenType = (QParser::Grammar::TokenType)0;
}

void OSI_API_CALL OSIX::Parser::beginLex()
{
  /*QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeTokenType = QParser::Grammar::LEX_TOKEN;*/
}

void OSI_API_CALL OSIX::Parser::endLex()
{
  /*QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeTokenType = (QParser::Grammar::TokenType)0;*/
}

void OSI_API_CALL OSIX::Parser::beginLexSymbols()
{
  /*QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeSubTokenType = QParser::Grammar::LEX_SYMBOL;*/
}

void OSI_API_CALL OSIX::Parser::endLexSymbols()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->GetLexer().Build(QParser::Lexer::TOKENTYPE_LEX_SYMBOL);
  //grammarObject->constructTokens();
  //grammarObject->activeSubTokenType = (QParser::Grammar::SubTokenType)0;
}

void OSI_API_CALL OSIX::Parser::beginLexWords()
{
  /*QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->activeSubTokenType = QParser::Grammar::LEX_WORD;*/
}

void OSI_API_CALL OSIX::Parser::endLexWords()
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->GetLexer().Build(QParser::Lexer::TOKENTYPE_LEX_WORD);
  //grammarObject->constructTokens();
  //grammarObject->activeSubTokenType = (QParser::Grammar::SubTokenType)0;
}

OSid OSI_API_CALL OSIX::Parser::stringToken(const OSchar* tokenName, const OSchar* value)
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  return grammarObject->GetLexer().StringToken(tokenName, value);
}

OSid OSI_API_CALL OSIX::Parser::charToken(const OSchar* tokenName, char value)
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  return grammarObject->GetLexer().CharToken(tokenName, value);
}

OSid OSI_API_CALL OSIX::Parser::boundedToken(const OSchar* tokenName, const OSchar* leftBoundingValue, const OSchar* rightBoundingValue, PARSER_BOUNDED_LINETYPE lineType)
{
  // todo: handle line type.....

  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  return grammarObject->GetLexer().BoundedToken(tokenName, leftBoundingValue, rightBoundingValue, lineType);
}

void OSI_API_CALL OSIX::Parser::beginLanguage()
{}

void OSI_API_CALL OSIX::Parser::endLanguage()
{
}

OSid OSI_API_CALL OSIX::Parser::beginProduction(const OSchar* productionName)
{
  return _this.getActiveObject<QParser::Grammar>()->BeginProduction(productionName);
}

void OSI_API_CALL OSIX::Parser::endProduction()
{
  return _this.getActiveObject<QParser::Grammar>()->EndProduction();
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
  _this.getActiveObject<QParser::Grammar>()->ProductionToken(token);
}

OSid OSI_API_CALL OSIX::Parser::productionToken(const OSchar* tokenName)
{
  return _this.getActiveObject<QParser::Grammar>()->ProductionToken(tokenName);
}

OSid OSI_API_CALL OSIX::Parser::productionIdentifierDecl(const OSchar* typeName)
{
  return _this.getActiveObject<QParser::Grammar>()->ProductionIdentifierDecl(typeName);
}

/*void OSI_API_CALL OSIX::Parser::productionIdentifierRef(OSid type)
{
  _this.getActiveObject<QParser::Grammar>()->ProductionIdentifierRef(type);
}*/

OSid OSI_API_CALL OSIX::Parser::productionIdentifierRef(const OSchar* typeName)
{
  return _this.getActiveObject<QParser::Grammar>()->ProductionIdentifierRef(typeName);
}

void OSI_API_CALL OSIX::Parser::productionLiteralToken(PARSER_LITERAL_TOKEN literalToken)
{
  switch(literalToken)
  {
    case NUMERIC_LITERAL:
      //_this.getActiveObject<QParser::Grammar>()->ProductionToken(QParser::Grammar::ID_CONST_NUM);
      _this.getActiveObject<QParser::Grammar>()->ProductionToken(QParser::TOKEN_TERMINAL_LITERAL);
      break;
    default:; // error: unkown literal token
  }
}

OSid OSI_API_CALL OSIX::Parser::declareProduction(const OSchar* productionName)
{
  return _this.getActiveObject<QParser::Grammar>()->DeclareProduction(productionName);
}

void OSI_API_CALL OSIX::Parser::startProduction(OSid production)
{
  _this.getActiveObject<QParser::Grammar>()->GrammarStartSymbol(production);
}

OSobject OSI_API_CALL OSIX::Parser::parseFile(const OSchar* fileName)
{
  QParser::Grammar* grammarObject = _this.grammar;

  QParser::ParseResult::ParseResult* parseResult = _this.beginObject<QParser::ParseResult>();
  grammarObject->ParseFile(fileName, *parseResult);
  _this.endObject<QParser::ParseResult>();

  return cast_object(parseResult);
}

OSobject OSI_API_CALL OSIX::Parser::parseString(const OSchar* stringBuffer)
{
  QParser::Grammar* grammarObject = _this.grammar;

  QParser::ParseResult* parseResult = _this.beginObject<QParser::ParseResult>();
  grammarObject->ParseString(stringBuffer, *parseResult);
  _this.endObject<QParser::ParseResult>();

  return cast_object(parseResult);
}

OSobject OSI_API_CALL OSIX::Parser::getInputStream(OSobject parseResult)
{
  QParser::ParseResult& resultObject = *reinterpret_cast<QParser::ParseResult*>(parseResult);
  return reinterpret_cast<OSobject>((OSuint32*)&resultObject.inputStream);
}

OSobject OSI_API_CALL OSIX::Parser::getLexStream(OSobject parseResult)
{
  QParser::ParseResult& resultObject = *reinterpret_cast<QParser::ParseResult*>(parseResult);
  return reinterpret_cast<OSobject>((OSuint32*)&resultObject.lexStream);
}

OSobject OSI_API_CALL OSIX::Parser::getParseStream(OSobject parseResult)
{
  QParser::ParseResult& resultObject = *reinterpret_cast<QParser::ParseResult*>(parseResult);
  return reinterpret_cast<OSobject>((OSuint32*)&resultObject.parseStream);
}

void OSI_API_CALL OSIX::Parser::precedence(const OSchar* token1Name, const OSchar* token2Name)
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->Precedence(token1Name, token2Name);
}

void OSI_API_CALL OSIX::Parser::precedence(OSid token1, OSid token2)
{
  QParser::Grammar* grammarObject = _this.getActiveObject<QParser::Grammar>();
  grammarObject->Precedence(token1, token2);
}

const OSchar* OSI_API_CALL OSIX::Parser::getTokenName(OSid token)
{
  QParser::Grammar* grammarObject = _this.grammar;
  return grammarObject->GetTokenRegistry().GetTokenName(token).c_str();
}

OSbool OSI_API_CALL OSIX::Parser::isIdentifier(OSid token)
{
  return token == OSid(QParser::TOKEN_TERMINAL_IDENTIFIER);
}

OSbool OSI_API_CALL OSIX::Parser::isNumeric(OSid token)
{
  return token == OSid(QParser::TOKEN_TERMINAL_LITERAL);
}

void OSI_API_CALL OSIX::Parser::getMatchText(OSobject parseResult, const ParseMatch& match, char* text)
{
  QParser::ParseResult& resultObject = *reinterpret_cast<QParser::ParseResult*>(parseResult);
  ParseMatch &lexMatch = resultObject.lexStream.data[match.offset];
  memcpy(text, reinterpret_cast<const OSuint8*>(resultObject.inputStream.data) + lexMatch.offset, lexMatch.length);
  text[lexMatch.length] = '\0';
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
    /*QParser::Grammar* grammarObject = static_cast<QParser::Parser::ParserDbg*>(this)->getParser().grammar;
    grammarObject->DebugOutputTokens();*/
  }

  void OSIX::ParserDbg::debugOutputProductions()
  {
    /*QParser::Grammar* grammarObject = ((QParser::Parser::ParserDbg*)this)->getParser().grammar;
    grammarObject->DebugOutputProductions();*/
  }

  void OSIX::ParserDbg::debugOutputGrammar()
  {
    /*QParser::Grammar* grammarObject = ((QParser::Parser::ParserDbg*)this)->getParser().grammar;
    grammarObject->DebugOutputStates();
    grammarObject->DebugOutputTable();*/
  }

  void OSIX::ParserDbg::debugOutputParseResult(OSobject parseResult)
  {
    /*QParser::Grammar* grammarObject = static_cast<QParser::Parser::ParserDbg*>(this)->getParser().grammar;
    grammarObject->DebugOutputParseResult(parseResult);*/
  }
#endif

OSIX::Parser* OSI_API_CALL OSIX::parserInit()
{
  return static_cast<OSIX::Parser*>(new QParser::Parser());
}

#endif
#endif
