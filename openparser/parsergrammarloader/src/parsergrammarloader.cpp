//////////////////////////////////////////////////////////////////////////////
//
//    PARSERGRAMMARLOADER.CPP
//
//    Copyright © 2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                                 INCLUDES                                 */
#include "parsergrammarloader.h"
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

    storeId(str1[1] == '\0'? charToken(name, str1[0]) : stringToken(name, str1), name);
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

    storeId(str1[1] == '\0'? charToken(name, str1[0]) : stringToken(name, str1), name);
    return true;
  }

  // PRE-CONDITION: nUnderscores == 1 || nUnderscores == 2 (otherwise it's a token and should be caught by the earlier lookahead)
  enum OSIX_PARSER_BOUNDED_LINETYPE boundedLineType = (nUnderscores == 1)? SINGLE_LINE : MULTI_LINE;

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

  storeId(boundedToken(name, str1, str2, boundedLineType), name);
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

  beginRaw();
  {
    // Parse raw tokens
    do { parseWhiteComments(); } while(parseTerminalTokenDecl());
  }
  endRaw();

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

  beginNil();
  {
    // Parse nil tokens
    do { parseWhiteComments(); } while(parseTerminalTokenDecl());
  }
  endNil();

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

  beginLexSymbols();
  {
    // Parse lex symbol tokens
    do { parseWhiteComments(); } while(parseTerminalTokenDecl());
  }
  endLexSymbols();

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

  beginLexWords();
  {
    // Parse lex word tokens
    do { parseWhiteComments(); } while(parseTerminalTokenDecl());
  }
  endLexWords();

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

  beginLex();
  {
    // Parse lex tokens
    parseLexSymbols();
    parseWhiteComments();
    parseLexWords();
  }
  endLex();

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
        productionLiteralToken(NUMERIC_LITERAL);
        return true;
      }
    }

    bufferPos = oldPos;
  }

  OSchar identifierStr[255];
  if(!parseIdentifier(identifierStr))
    return false;

  if(isIdentifierRef) productionIdentifierRefName(identifierStr);
  //todo:? else if(isIdentifierDecl) { *idsPos = productionIdentifierDecl(identifierStr); ++idsPos; }
  else if(isIdentifierDecl) { productionIdentifierDecl(identifierStr); }
  else productionTokenName(identifierStr);

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
      OSid id = beginProduction(identifierStr);

        if(id != prevId)
        {
          // (Store id only if it hasn't been defined already)
          storeId(id, identifierStr);
          prevId = id;
        }

        do { parseWhiteComments(); } while (!BUFFEREMPTY && parseProductionToken());

        if(BUFFEREMPTY || *bufferPos != '}') { endProduction(); return false; } // error: expected '}'
        ++bufferPos;

      endProduction();
    }
    else
    {
      declareProduction(identifierStr);
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

  beginLanguage();
  {
    // Parse productions
    do { parseWhiteComments(); } while(!BUFFEREMPTY && parseProduction());
  }
  endLanguage();

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
      tokenNamePrecedence(identifierStr1, identifierStr2);
      break;
    case '>':
      tokenNamePrecedence(identifierStr2, identifierStr1);
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

void parserConstructGrammar(const OSchar* buffer, OSuint bufferSize, OSid* ids, OSuint idsBufferSize)
{
  ::buffer = buffer;
  ::bufferSize = bufferSize;
  bufferPos = buffer;

  ::ids = ids;
  ::idsLength = idsBufferSize / sizeof(OSid);
  idsPos = ids;

  parseWhiteComments();

  beginGrammar();
  {
    parseRaw(); parseWhiteComments();
    parseNil(); parseWhiteComments();
    parseLex(); parseWhiteComments();
    parseProductions(); parseWhiteComments();
    parsePrecedence();
  }
  endGrammar();
}
