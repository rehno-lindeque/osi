//////////////////////////////////////////////////////////////////////////////
//
//    PARSERGRAMMARLOADER.CPP
//
//    Copyright © 2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    TODO: 
      This file needs quite a bit of clean up. A more elegant separation 
      between C and C++ code is need...   
*/

/*                                 INCLUDES                                 */
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
# include "parsergrammarloader.hpp"
  using namespace OSIX;
#else
# include "parsergrammarloader.h"
#endif

#include <iostream>

using namespace std;

/*                                  MACROS                                  */
#define REMAININGCHARACTERS (bufferSize - (bufferPos - buffer))
#define BUFFEREMPTY (REMAININGCHARACTERS <= 0)

/*                                 GLOBALS                                  */
const OSchar* buffer = 0;
const OSchar* bufferPos = 0;
OSuint bufferSize = 0;

OSid* ids = 0;
OSid* idsPos = 0;
OSuint idsLength = 0;

#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
OSIX::Parser *parser = 0;
#endif

/*                              IMPLEMENTATION                              */
void storeId(OSid id, const char* idName)
{
  if(idsPos - ids < idsLength)
  {
    *idsPos = id;
    ++idsPos;
  }
  else
    cout << "Warning: not enough space to store id \"" << idName << '\"' << endl;
}

inline void skipLineComment()
{
  bufferPos += 2;
  while(!BUFFEREMPTY)
  {
    ++bufferPos;
    if(bufferPos[-1] == '\n')
      return;
  }
}

inline void skipBlockComment()
{
  bufferPos += 2;
  while(!BUFFEREMPTY)
  {
    if(*bufferPos == '*' && REMAININGCHARACTERS > 1 && bufferPos[1] == '/')
    {
      bufferPos += 2;
      return;
    }
    ++bufferPos;
  }
}

inline bool parseComments()
{
  const OSchar* oldPos = bufferPos;

  // Parse /* or //
  while(REMAININGCHARACTERS > 1 && *bufferPos == '/' && (bufferPos[1] == '/' || bufferPos[1] == '*'))
  {
    if(bufferPos[1] == '/') skipLineComment();
    else skipBlockComment();
  }

  return oldPos != bufferPos;
}

inline bool parseWhiteComments()
{
  const OSchar* oldPos = bufferPos;
  while(!BUFFEREMPTY)
  {
    if (*bufferPos != ' ' && *bufferPos != '\t' && *bufferPos != '\r' && *bufferPos != '\n')
    {
      if(!parseComments())
        return oldPos != bufferPos;
    }
    else
    {
      ++bufferPos;
    }
  }

  return oldPos != bufferPos;
}

inline bool parseStr(const OSchar* str)
{
  OSuint c = 0;
  while(REMAININGCHARACTERS > c && str[c] != '\0' && bufferPos[c] == str[c])
    ++c;

  if(str[c] == '\0')
  {
    bufferPos += c;
    return true;
  }

  return false;
}

inline bool parseIdentifier(OSchar* identifierStr)
{
  OSuint c = 0;
  while(REMAININGCHARACTERS > c
    && bufferPos[c] != ' '
    && bufferPos[c] != '\t'
    && bufferPos[c] != '\n'
    && bufferPos[c] != '\r'
    && bufferPos[c] != '{'
    && bufferPos[c] != '}') // todo: what else?
  {
    identifierStr[c] = bufferPos[c];
    ++c;
  }

  identifierStr[c] = '\0';
  bufferPos += c;
  return c > 0;
}

inline bool parseStringString(OSchar* str)
{
  if(*bufferPos != '"')
    return false;
  ++bufferPos;

  OSuint c = 0;
  while(REMAININGCHARACTERS > c && bufferPos[c] != '"')
  {
    // Parse escape character
    if (bufferPos[c] != '\\')
      str[c] = bufferPos[c];
    else if(REMAININGCHARACTERS > c+1)
    {
      switch(bufferPos[c+1])
      {
        case 'n': str[c] = '\n'; break;
        case 'r': str[c] = '\r'; break;
        case 't': str[c] = '\t'; break;
        default: str[c] = bufferPos[c+1];
      }
      ++c;
    }
    ++c;
  }

  if(bufferPos[c] == '"')
  {
    str[c] = '\0';
    bufferPos += c + 1; // eat string + ending '"'
    return true;
  }

  --bufferPos; // return the first '"' to the buffer...
  str[0] = '\0';
  return false;
}

inline bool parseStringTokenDeclPart(const OSchar* name)
{
  const OSchar* oldPos = bufferPos;
  OSchar str1[255];
  if(!parseStringString(str1))
    return false;
  parseWhiteComments();

  // Parse for a normal string or character token
  if(BUFFEREMPTY || *bufferPos != '_')
  {
    if(str1[0] == '\0')
      return false; // error: expected string with at least 1 character (empty string is not a valid token)

#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
    storeId(str1[1] == '\0'? parser->charToken(name, str1[0]) : parser->stringToken(name, str1), name);
#else    
    storeId(str1[1] == '\0'? charToken(name, str1[0]) : stringToken(name, str1), name);
#endif
    return true;
  }

  // skip '_' or '__'
  OSuint nUnderscores = 0;
  while(!BUFFEREMPTY && *bufferPos == '_')
  {
    ++nUnderscores;
    ++bufferPos;
  }

  // Test whether string starting with _ or __ is also an identifier (in that case, we also have a normal string/character token).
  if(nUnderscores > 2
    || (REMAININGCHARACTERS > nUnderscores
      && (bufferPos[nUnderscores] == ' '
        || bufferPos[nUnderscores] == '\t'
        || bufferPos[nUnderscores] == '\n'
        || bufferPos[nUnderscores] == '\r'
        || bufferPos[nUnderscores] == '{'
        || bufferPos[nUnderscores] == '}'))) // todo: what else?
  {
    if(str1[0] == '\0')
      return false; // error: expected string with at least 1 character (empty string is not a valid token)
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
    storeId(str1[1] == '\0'? parser->charToken(name, str1[0]) : parser->stringToken(name, str1), name);
#else
    storeId(str1[1] == '\0'? charToken(name, str1[0]) : stringToken(name, str1), name);
#endif
    return true;
  }

  // PRE-CONDITION: nUnderscores == 1 || nUnderscores == 2 (otherwise it's a token and should be caught by the earlier lookahead)
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  PARSER_BOUNDED_LINETYPE boundedLineType = (nUnderscores == 1)? SINGLE_LINE : MULTI_LINE;
#else
  enum OSIX_PARSER_BOUNDED_LINETYPE boundedLineType = (nUnderscores == 1)? SINGLE_LINE : MULTI_LINE;
#endif

  // Parse the bounded token
  parseWhiteComments();

  OSchar str2[255];
  if(BUFFEREMPTY || !parseStringString(str2))
  {
    bufferPos = oldPos;
    return false; // Error: Expected string after '_'
  }

  /* Empty strings ARE valid ONLY for single line bounded tokens (but only on one side of the '_' */
  if((str1[0] == '\0' && str2[0] == '\0')
    || (boundedLineType != SINGLE_LINE && (str1[0] == '\0' || str2[0] == '\0')))
    return false; // error: expected string with at least 1 character (empty string is not a valid token)*/

#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  storeId(parser->boundedToken(name, str1, str2, boundedLineType), name);
#else
  storeId(boundedToken(name, str1, str2, boundedLineType), name);
#endif
  return true;
}

inline bool parseTerminalTokenDecl()
{
  OSchar identifierStr[255];
  if(BUFFEREMPTY || !parseIdentifier(identifierStr))
    return false;

  parseWhiteComments();

  if(BUFFEREMPTY || *bufferPos != '=')
    return false;
  ++bufferPos;

  parseWhiteComments();

  if(!BUFFEREMPTY && parseStringTokenDeclPart(identifierStr))
    return true;
  return false;
}

bool parseRaw()
{
  if(BUFFEREMPTY || !parseStr("raw"))
    return false;

  parseWhiteComments();

  if(BUFFEREMPTY || *bufferPos != '{')
    return false; // error: expected '{'
  ++bufferPos;

#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  parser->beginRaw();
#else
  beginRaw();
#endif
  {
    // Parse raw tokens
    do { parseWhiteComments(); } while(parseTerminalTokenDecl());
  }
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  parser->endRaw();
#else
  endRaw();
#endif

  parseWhiteComments();

  if(BUFFEREMPTY || *bufferPos != '}')
    return false; // error: expected '}'
  ++bufferPos;
  return true;
}

bool parseNil()
{
  if(BUFFEREMPTY || !parseStr("nil"))
    return false;

  parseWhiteComments();

  if(BUFFEREMPTY || *bufferPos != '{')
    return false; // error: expected '{'
  ++bufferPos;

#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  parser->beginNil();
#else
  beginNil();
#endif
  {
    // Parse nil tokens
    do { parseWhiteComments(); } while(parseTerminalTokenDecl());
  }
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  parser->endNil();
#else
  endNil();
#endif

  parseWhiteComments();

  if(BUFFEREMPTY || *bufferPos != '}')
    return false; // error: expected '}'
  ++bufferPos;
  return true;
}

inline bool parseLexSymbols()
{
  if(BUFFEREMPTY || !parseStr("symbols"))
    return false;

  parseWhiteComments();

  if(BUFFEREMPTY || *bufferPos != '{')
    return false; // error: expected '{'
  ++bufferPos;

#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  parser->beginLexSymbols();
#else
  beginLexSymbols();
#endif
  {
    // Parse lex symbol tokens
    do { parseWhiteComments(); } while(parseTerminalTokenDecl());
  }
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  parser->endLexSymbols();
#else
  endLexSymbols();
#endif

  parseWhiteComments();

  if(BUFFEREMPTY || *bufferPos != '}')
    return false; // error: expected '}'
  ++bufferPos;
  return true;
}

inline bool parseLexWords()
{
  if(BUFFEREMPTY || !parseStr("words"))
    return false;

  parseWhiteComments();

  if(BUFFEREMPTY || *bufferPos != '{')
    return false; // error: expected '{'
  ++bufferPos;

  parseWhiteComments();

#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  parser->beginLexWords();
#else
  beginLexWords();
#endif
  {
    // Parse lex word tokens
    do { parseWhiteComments(); } while(parseTerminalTokenDecl());
  }
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  parser->endLexWords();
#else
  endLexWords();
#endif

  parseWhiteComments();

  if(BUFFEREMPTY || *bufferPos != '}')
    return false; // error: expected '}'
  ++bufferPos;
  return true;
}

bool parseLex()
{
  if(BUFFEREMPTY || !parseStr("lex"))
    return false;

  parseWhiteComments();

  if(BUFFEREMPTY || *bufferPos != '{')
    return false; // error: expected '{'
  ++bufferPos;

  parseWhiteComments();

#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  parser->beginLex();
#else
  beginLex();
#endif
  {
    // Parse lex tokens
    parseLexSymbols();
    parseWhiteComments();
    parseLexWords();
  }
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  parser->endLex();
#else
  endLex();
#endif

  parseWhiteComments();

  if(BUFFEREMPTY || *bufferPos != '}')
    return false; // error: expected '}'
  ++bufferPos;
  return true;
}

inline bool parseProductionToken()
{
  const OSchar* oldPos = bufferPos;

  // Parse special tokens
  bool isIdentifierRef = (*bufferPos == '&');
  bool isIdentifierDecl = (*bufferPos == '#');

  if(isIdentifierRef || isIdentifierDecl)
    ++bufferPos;
  else
  {
    if(parseStr("numeric"))
    {
      if(*bufferPos == ' '
        || *bufferPos == '\t'
        || *bufferPos == '\n'
        || *bufferPos == '\r'
        || *bufferPos == '{'
        || *bufferPos == '}') // todo: what else?)
      {
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
        parser->productionLiteralToken(NUMERIC_LITERAL);
#else
        productionLiteralToken(NUMERIC_LITERAL);
#endif
        return true;
      }
    }

    bufferPos = oldPos;
  }

  OSchar identifierStr[255];
  if(!parseIdentifier(identifierStr))
    return false;

#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  if(isIdentifierRef) parser->productionIdentifierRef(identifierStr);
  //todo:? else if(isIdentifierDecl) { *idsPos = parser->productionIdentifierDecl(identifierStr); ++idsPos; }
  else if(isIdentifierDecl) { parser->productionIdentifierDecl(identifierStr); }
  else parser->productionToken(identifierStr);
#else
  if(isIdentifierRef) productionIdentifierRefName(identifierStr);
  //todo:? else if(isIdentifierDecl) { *idsPos = productionIdentifierDecl(identifierStr); ++idsPos; }
  else if(isIdentifierDecl) { productionIdentifierDecl(identifierStr); }
  else productionTokenName(identifierStr);
#endif
  
  return oldPos != bufferPos;
}

inline bool parseProduction()
{
  const OSchar* oldPos = bufferPos;
  OSchar identifierStr[255];

  if(!parseIdentifier(identifierStr))
    return false;

  //while(!BUFFEREMPTY && parseIdentifier(identifierStr))
  //{
    parseWhiteComments();

    if(!BUFFEREMPTY && *bufferPos == '{')
    {
      ++bufferPos;
      parseWhiteComments();

      static OSid prevId = -1;
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
      OSid id = parser->beginProduction(identifierStr);
#else
      OSid id = parser->beginProduction(identifierStr);
#endif

        if(id != prevId)
        {
          // (Store id only if it hasn't been defined already)
          storeId(id, identifierStr);
          prevId = id;
        }

        do { parseWhiteComments(); } while (!BUFFEREMPTY && parseProductionToken());

#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
        if(BUFFEREMPTY || *bufferPos != '}') { parser->endProduction(); return false; } // error: expected '}'
#else
        if(BUFFEREMPTY || *bufferPos != '}') { endProduction(); return false; } // error: expected '}'
#endif
        ++bufferPos;

#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
      parser->endProduction();
#else
      endProduction();
#endif
    }
    else
    {
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
      parser->declareProduction(identifierStr);
#else
      declareProduction(identifierStr);
#endif
    }

    parseWhiteComments();
  //}

  return oldPos != bufferPos;
}

bool parseProductions()
{
  if(BUFFEREMPTY || !parseStr("grammar"))
    return false;

  parseWhiteComments();

  if(BUFFEREMPTY || *bufferPos != '{')
    return false; // error: expected '{'
  ++bufferPos;

  parseWhiteComments();

#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  parser->beginLanguage();
#else
  beginLanguage();
#endif
  {
    // Parse productions
    do { parseWhiteComments(); } while(!BUFFEREMPTY && parseProduction());
  }
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  parser->endLanguage();
#else
  endLanguage();
#endif

  //parseWhiteComments();

  if(BUFFEREMPTY || *bufferPos != '}')
    return false; // error: expected '}'
  ++bufferPos;
  return true;
}

inline bool parsePrecedenceDirective()
{
  const OSchar* oldPos = bufferPos;
  OSchar identifierStr1[255];
  OSchar identifierStr2[255];

  if(!parseIdentifier(identifierStr1))
    return false;

  parseWhiteComments();

  if(BUFFEREMPTY || (*bufferPos != '<' && *bufferPos != '>'))
  {
    bufferPos = oldPos;
    return false;
  }

  OSchar operatorChar = *bufferPos;
  ++bufferPos;
  parseWhiteComments();

  if(!parseIdentifier(identifierStr2))
  {
    bufferPos = oldPos;
    return false;
  }

  switch(operatorChar)
  {
    case '<':
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
      parser->precedence(identifierStr1, identifierStr2);
#else
      tokenNamePrecedence(identifierStr1, identifierStr2);
#endif
      break;
    case '>':
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
      parser->precedence(identifierStr2, identifierStr1);
#else
      tokenNamePrecedence(identifierStr2, identifierStr1);
#endif
      break;
    default:
      bufferPos = oldPos;
      return false;
  }

  return oldPos != bufferPos;
}

bool parsePrecedence()
{
  if(BUFFEREMPTY || !parseStr("precedence"))
    return false;

  parseWhiteComments();

  if(BUFFEREMPTY || *bufferPos != '{')
    return false; // error: expected '{'
  ++bufferPos;

  do { parseWhiteComments(); } while(!BUFFEREMPTY && parsePrecedenceDirective());

  if(BUFFEREMPTY || *bufferPos != '}')
    return false; // error: expected '}'
  ++bufferPos;
  return true;
}

#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
namespace Parser {
namespace GrammarLoader {
void constructGrammar(OSIX::Parser &parser, const OSchar* buffer, OSuint bufferSize, OSid* ids, OSuint idsBufferSize)
{
  ::buffer = buffer;
  ::bufferSize = bufferSize;
  bufferPos = buffer;

  ::ids = ids;
  ::idsLength = idsBufferSize / sizeof(OSid);
  idsPos = ids;
  
  ::parser = &parser;

  parseWhiteComments();

  parser.beginGrammar();
  {
    parseRaw(); parseWhiteComments();
    parseNil(); parseWhiteComments();
    parseLex(); parseWhiteComments();
    parseProductions(); parseWhiteComments();
    parsePrecedence();
  }
  parser.endGrammar();
}
}}
#else
void parserConstructGrammar(const OSchar* buffer, OSuint bufferSize, OSid* ids, OSuint idsBufferSize)
{
  ::buffer = buffer;
  ::bufferSize = bufferSize;
  bufferPos = buffer;

  ::ids = ids;
  ::idsLength = idsBufferSize / sizeof(OSid);
  idsPos = ids;

  parseWhiteComments();

#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  parser->beginGrammar();
#else
  beginGrammar();
#endif
  {
    parseRaw(); parseWhiteComments();
    parseNil(); parseWhiteComments();
    parseLex(); parseWhiteComments();
    parseProductions(); parseWhiteComments();
    parsePrecedence();
  }
#if defined(OSI_CPP_STATIC_BUILD) || defined(OSI_CPP_SHARED_BUILD)
  parser->endGrammar();
#else
  endGrammar();
#endif
}
#endif
