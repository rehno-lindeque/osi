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
  if(_this.parser != null)
    return 0; // error

  auto parserObject = _this.parser = _this.BeginObject<QParser::ParserLD>();
  _this.lexer = _this.BeginObject<QParser::Lexer, QParser::TokenRegistry&>(_this->GetTokenRegistry());

  return cast_object(parserObject);
}

void OSI_API_CALL OSIX::Parser::endGrammar()
{
  _this.EndObject<QParser::Lexer>();
  _this.EndObject<QParser::ParserLD>();
  
  if(_this.grammar->CheckForwardDeclarations())
    _this->ConstructParser(_this.grammar);
}

void OSI_API_CALL OSIX::Parser::beginRaw()
{
}

void OSI_API_CALL OSIX::Parser::endRaw()
{
  _this.lexer->Build(QParser::Lexer::TOKENTYPE_RAW);
}

void OSI_API_CALL OSIX::Parser::beginNil()
{
}

void OSI_API_CALL OSIX::Parser::endNil()
{
  _this.lexer->Build(QParser::Lexer::TOKENTYPE_NIL);
}

void OSI_API_CALL OSIX::Parser::beginLex()
{
}

void OSI_API_CALL OSIX::Parser::endLex()
{
}

void OSI_API_CALL OSIX::Parser::beginLexSymbols()
{
}

void OSI_API_CALL OSIX::Parser::endLexSymbols()
{
  _this.lexer->Build(QParser::Lexer::TOKENTYPE_LEX_SYMBOL);
}

void OSI_API_CALL OSIX::Parser::beginLexWords()
{
}

void OSI_API_CALL OSIX::Parser::endLexWords()
{
  _this.lexer->Build(QParser::Lexer::TOKENTYPE_LEX_WORD);
}

OSid OSI_API_CALL OSIX::Parser::stringToken(const OSchar* tokenName, const OSchar* value)
{
  return _this.lexer->StringToken(tokenName, value);
}

OSid OSI_API_CALL OSIX::Parser::charToken(const OSchar* tokenName, char value)
{
  return _this.lexer->CharToken(tokenName, value);
}

OSid OSI_API_CALL OSIX::Parser::boundedToken(const OSchar* tokenName, const OSchar* leftBoundingValue, const OSchar* rightBoundingValue, PARSER_BOUNDED_LINETYPE lineType)
{
  return _this.lexer->BoundedToken(tokenName, leftBoundingValue, rightBoundingValue, lineType);
}

void OSI_API_CALL OSIX::Parser::beginLanguage()
{
  _this.grammar = static_cast<QParser::Grammar*>(_this.BeginObject<QParser::GrammarLD, QParser::TokenRegistry&>(_this->GetTokenRegistry()));
}

void OSI_API_CALL OSIX::Parser::endLanguage()
{
  _this.EndObject<QParser::Grammar>();  
}

OSid OSI_API_CALL OSIX::Parser::beginProduction(const OSchar* productionName)
{
  return _this.grammar->BeginProduction(productionName);
}

void OSI_API_CALL OSIX::Parser::endProduction()
{
  return _this.grammar->EndProduction();
}

void OSI_API_CALL OSIX::Parser::productionToken(OSid token)
{
  _this.grammar->ProductionToken(token);
}

OSid OSI_API_CALL OSIX::Parser::productionToken(const OSchar* tokenName)
{
  return _this.grammar->ProductionToken(tokenName);
}

OSid OSI_API_CALL OSIX::Parser::productionIdentifierDecl(const OSchar* typeName)
{
  return _this.grammar->ProductionIdentifierDecl(typeName);
}

/*void OSI_API_CALL OSIX::Parser::productionIdentifierRef(OSid type)
{
  _this.grammar->ProductionIdentifierRef(type);
}*/

OSid OSI_API_CALL OSIX::Parser::productionIdentifierRef(const OSchar* typeName)
{
  return _this.grammar->ProductionIdentifierRef(typeName);
}

void OSI_API_CALL OSIX::Parser::productionLiteralToken(PARSER_LITERAL_TOKEN literalToken)
{
  switch(literalToken)
  {
    case NUMERIC_LITERAL:
      _this.grammar->ProductionToken(QParser::TOKEN_TERMINAL_LITERAL);
      break;
    default:; // error: unkown literal token
  }
}

OSid OSI_API_CALL OSIX::Parser::declareProduction(const OSchar* productionName)
{
  return _this.grammar->DeclareProduction(productionName);
}

void OSI_API_CALL OSIX::Parser::startProduction(OSid production)
{
  _this.grammar->GrammarStartSymbol(production);
}

OSobject OSI_API_CALL OSIX::Parser::parseFile(const OSchar* fileName)
{
  auto parseResult = _this.BeginObject<QParser::ParseResult>();
  _this->ParseFile(fileName, *parseResult);
  _this.EndObject<QParser::ParseResult>();

  return cast_object(parseResult);
}

OSobject OSI_API_CALL OSIX::Parser::parseString(const OSchar* stringBuffer)
{
  auto parseResult = _this.BeginObject<QParser::ParseResult>();
  _this->ParseString(stringBuffer, *parseResult);
  _this.EndObject<QParser::ParseResult>();

  return cast_object(parseResult);
}

OSobject OSI_API_CALL OSIX::Parser::getInputStream(OSobject parseResult)
{
  auto& resultObject = *reinterpret_cast<QParser::ParseResult*>(parseResult);
  return reinterpret_cast<OSobject>((OSuint32*)&resultObject.inputStream);
}

OSobject OSI_API_CALL OSIX::Parser::getLexStream(OSobject parseResult)
{
  auto& resultObject = *reinterpret_cast<QParser::ParseResult*>(parseResult);
  return reinterpret_cast<OSobject>((OSuint32*)&resultObject.lexStream);
}

OSobject OSI_API_CALL OSIX::Parser::getParseStream(OSobject parseResult)
{
  auto& resultObject = *reinterpret_cast<QParser::ParseResult*>(parseResult);
  return reinterpret_cast<OSobject>((OSuint32*)&resultObject.parseStream);
}

void OSI_API_CALL OSIX::Parser::precedence(const OSchar* token1Name, const OSchar* token2Name)
{
  _this.grammar->Precedence(token1Name, token2Name);
}

void OSI_API_CALL OSIX::Parser::precedence(OSid token1, OSid token2)
{
  _this.grammar->Precedence(token1, token2);
}

const OSchar* OSI_API_CALL OSIX::Parser::getTokenName(OSid token)
{
  return _this->GetTokenRegistry().GetTokenName(token).c_str();
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
  auto& resultObject = *reinterpret_cast<QParser::ParseResult*>(parseResult);
  auto& lexMatch = resultObject.lexStream.data[match.offset];
  memcpy(text, reinterpret_cast<const OSuint8*>(resultObject.inputStream.data) + lexMatch.offset, lexMatch.length);
  text[lexMatch.length] = '\0';
}

void OSI_API_CALL OSIX::Parser::delObject(OSobject object)
{
  _this.DeleteObject((Base::Object*)object);
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
    /*QParser::Grammar* grammarObject = static_cast<QParser::Parser::ParserDbg*>(this)->GetParser().grammar;
    grammarObject->DebugOutputTokens();*/
  }

  void OSIX::ParserDbg::debugOutputProductions()
  {
    /*QParser::Grammar* grammarObject = ((QParser::Parser::ParserDbg*)this)->GetParser().grammar;
    grammarObject->DebugOutputProductions();*/
  }

  void OSIX::ParserDbg::debugOutputGrammar()
  {
    /*QParser::Grammar* grammarObject = ((QParser::Parser::ParserDbg*)this)->GetParser().grammar;
    grammarObject->DebugOutputStates();
    grammarObject->DebugOutputTable();*/
  }

  void OSIX::ParserDbg::debugOutputParseResult(OSobject parseResult)
  {
    /*QParser::Grammar* grammarObject = static_cast<QParser::Parser::ParserDbg*>(this)->GetParser().grammar;
    grammarObject->DebugOutputParseResult(parseResult);*/
  }
#endif

OSIX::Parser* OSI_API_CALL OSIX::parserInit()
{
  return static_cast<OSIX::Parser*>(new QParser::Parser());
}

#endif
#endif
