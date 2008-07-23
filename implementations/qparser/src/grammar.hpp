#ifdef  __QPARSER_GRAMMAR_H__
#ifndef __QPARSER_GRAMMAR_INL__
#define __QPARSER_GRAMMAR_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMAR.INL
//
//    Copyright © 2007-2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QParser
{
  const char Grammar::SPECIAL_SINGLELINE_BOUNDING_CHAR = '\0';
  const char Grammar::SPECIAL_MULTILINE_BOUNDING_CHAR = '\1';

  Grammar::Grammar() : nextTerminal(ID_FIRST_TERMINAL),
                       nextNonterminal(0),
                       activeSubTokenType((SubTokenType)0),
                       activeProduction(null),
                       startSymbol(-1),
                       errorStream(new stdio_filebuf<char>(stdout, std::ios::out)),
                       warnStream(new stdio_filebuf<char>(stdout, std::ios::out)),
                       infoStream(new stdio_filebuf<char>(stdout, std::ios::out))
  { 
    memset(tokenRootIndices, 0, sizeof(tokenRootIndices)); 
  }

  Grammar::~Grammar()
  {
    errorStream.flush();
    warnStream.flush();
    infoStream.flush();

    delete errorStream.rdbuf(null);
    delete warnStream.rdbuf(null);
    delete infoStream.rdbuf(null);
  }

  OSid Grammar::stringToken(const_cstring tokenName, const_cstring value)
  {
    // Initialize variables
    const uint    bufferLength     = (uint)tokenCharacters.size();
    const uint    valueLength      = (uint)strlen(value) + 1;

    // Preconditions
    OSI_ASSERT(valueLength > 0);

    // Concatenate token value to token (character) storage buffer (Note: null character is also concatenated)
    tokenCharacters.resize(bufferLength + valueLength);
    for(uint c = 0; c < valueLength; ++c)
      tokenCharacters[bufferLength + c] = value[c];

    // Create the terminal token and add it to all the relevant indexes
    return constructTerminal(tokenName, bufferLength, valueLength);
  }

  OSid Grammar::charToken(const_cstring tokenName, char value)
  {
    // Initialize variables
    const uint    bufferLength     = (uint)tokenCharacters.size();

    // Preconditions
    OSI_ASSERT(value != '\0');

    // Concatenate token value to token (character) storage buffer
    tokenCharacters.resize(bufferLength + 2);
    tokenCharacters[bufferLength] = value;
    tokenCharacters[bufferLength + 1] = '\0';
    
    // Create the terminal token and add it to all the relevant indexes
    return constructTerminal(tokenName, bufferLength, 2);
  }

  OSid Grammar::boundedToken(const_cstring tokenName, const_cstring leftBoundingValue, const_cstring rightBoundingValue, OSIX::PARSER_BOUNDED_LINETYPE lineType)
  {
    const uint    leftValueLength  = (uint)strlen(leftBoundingValue)  + 1;
    const uint    rightValueLength = (uint)strlen(rightBoundingValue) + 1;
    const uint    bufferLength     = (uint)tokenCharacters.size();

    // Preconditions
    OSI_ASSERT(leftValueLength > 0);
    OSI_ASSERT(rightValueLength > 0);

    // Concatenate token value to token (character) storage buffer
    tokenCharacters.resize(bufferLength + 1 + leftValueLength + rightValueLength);
    tokenCharacters[bufferLength] = (lineType == OSIX::SINGLE_LINE)? SPECIAL_SINGLELINE_BOUNDING_CHAR : SPECIAL_MULTILINE_BOUNDING_CHAR; // special character (the first character is a boundedness indicator)

    for(uint c = 0; c < leftValueLength; ++c)
      tokenCharacters[bufferLength + 1 + c] = leftBoundingValue[c];

    for(uint c = 0; c < rightValueLength; ++c)
      tokenCharacters[bufferLength + 1 + leftValueLength + c] = rightBoundingValue[c];

    // Create the terminal token and add it to all the relevant indexes
    return constructTerminal(tokenName, bufferLength, 1 + leftValueLength + rightValueLength);
  }

  void Grammar::constructTokens()
  {
    uint&             nTokens                      = Grammar::nTokens[activeTokenType + activeSubTokenType];
    Token*&           activeTokens                 = tokens[activeTokenType + activeSubTokenType];
    //TokenNameSet&     activeTokenNames             = tokenNames[activeTokenType];
    TokenRootIndex(&  activeTokenRootIndices)[256] = tokenRootIndices[activeTokenType + activeSubTokenType];

    //// Consolidate tokens (Copy tokens to a fixed array & construct a root character index)
    // Allocate token array
    nTokens = (uint)constructionTokens.size();
    activeTokens = new Token[nTokens];

    // Clear token root indices
    memset(activeTokenRootIndices, 0, sizeof(TokenRootIndex) * nTokens);

    // Copy tokens to token array (ordered by their parse values)
    {
      uint                           cToken;
      TokenConstructionSet::iterator iToken;

      for(cToken = 0, iToken = constructionTokens.begin(); cToken < nTokens; ++cToken, ++iToken)
      {
        // Copy construction token
        //activeTokens[cToken] = *iToken;

        // Add to token "root character" indexing table (optimization index)
        {
          //OLD: char rootCharacter = tokenCharacters[activeTokens[cToken].valueOffset];
          char rootCharacter = tokenCharacters[(*iToken).valueOffset];
          if(rootCharacter == SPECIAL_MULTILINE_BOUNDING_CHAR
              || rootCharacter == SPECIAL_SINGLELINE_BOUNDING_CHAR)
            //OLD:rootCharacter = tokenCharacters[activeTokens[cToken].valueOffset + 1];
            rootCharacter = tokenCharacters[(*iToken).valueOffset + 1];

          // Copy construction token into active tokens array
          // todo: this is rather ugly and inefficient... we should rewrite this using some indexing library
          TokenRootIndex& tokenRootIndex = activeTokenRootIndices[uint(rootCharacter)];
          if(tokenRootIndex.length == 0)
          {
            tokenRootIndex.offset = cToken;
            activeTokens[tokenRootIndex.offset] = *iToken;
          }
          else
          {
            // (move all tokens up by one)
            //BUG: memcpy(activeTokens + tokenRootIndex.offset + tokenRootIndex.length + 1, activeTokens + tokenRootIndex.offset + tokenRootIndex.length, sizeof(Token[nTokens - (tokenRootIndex.offset + tokenRootIndex.length + 1)]));
            //BUG: memcpy(activeTokens + tokenRootIndex.offset + tokenRootIndex.length + 1, activeTokens + tokenRootIndex.offset + tokenRootIndex.length, sizeof(Token[nTokens - (cToken + 1)]));
            memmove(&activeTokens[tokenRootIndex.offset + tokenRootIndex.length + 1], &activeTokens[tokenRootIndex.offset + tokenRootIndex.length], sizeof(Token[cToken - (tokenRootIndex.offset + tokenRootIndex.length)]));
            for(uint c = 0; c <= MAX_UINT8; ++c)
              if(activeTokenRootIndices[c].offset > tokenRootIndex.offset)
                ++activeTokenRootIndices[c].offset;

            // (inert token into this position)
            activeTokens[tokenRootIndex.offset + tokenRootIndex.length] = *iToken;
          }

          ++tokenRootIndex.length;
        }
      }
    }

    // Clear construction tokens
    constructionTokens.clear();
  }

  // Productions
  OSid Grammar::beginProduction(const_cstring productionName)
  {
    // Construct a nonterminal token for this production (if none exists)
    OSid id = constructNonterminal(productionName);    

    // Get a production set
    ProductionSet *productionSet = null;
    {
      map<OSid, ProductionSet*>::iterator i = productionSets.find(id);
      if(i == productionSets.end() || i->first != id)
      {
        productionSet = new ProductionSet();
        productionSets[id] = productionSet;
        productionSet->productionsOffset = productions.size();
        productionSet->productionsLength = 1;
      }
      else
      {
        productionSet = i->second;
        ++productionSet->productionsLength;
      }
      OSI_ASSERT(productionSet != null);
    }

    // Set as the active production
    //old: activeProduction = new Production;

    // Add production to set
    //old: productionIds[id] = activeProduction;
    {
      productions.push_back(std::make_pair(Production(), id));
      activeProduction = &productions[productions.size()-1].first;
    }

    return id;
  }

  void Grammar::endProduction()
  {
    // Add symbols to production
    if (activeProductionSymbols.size() > 0)
    {
      activeProduction->symbols = new Production::Symbol[activeProductionSymbols.size()];
      activeProduction->symbolsLength = activeProductionSymbols.size();
      memcpy(activeProduction->symbols, &activeProductionSymbols[0], activeProductionSymbols.size() * sizeof(Production::Symbol));
      activeProductionSymbols.clear();
    }
    activeProduction = null;
  }

  /*void Grammar::productionNonterminal(OSid nonterminal)
  {
    OSI_ASSERT(!IsToken(nonterminal));
    activeProductionSymbols.push_back(nonterminal);
  }

  void Grammar::productionTerminal(OSid terminal)
  {
    OSI_ASSERT(IsToken(terminal));
    activeProductionSymbols.push_back(terminal);
  }

  void Grammar::productionBeginScope()
  {
    //todo
  }

  void Grammar::productionEndScope()
  {
    //todo
  }*/

  void Grammar::productionToken(OSid token)
  {
    OSI_ASSERT(IsTerminal(token)? token >= ID_FIRST_TERMINAL && token < nextTerminal : token < nextNonTerminal);
    activeProductionSymbols.push_back(token);
  }

  OSid Grammar::productionToken(const_cstring tokenName)
  {
    OSid id = getTokenId(tokenName);
    if(id == OSid(-1))
      id = declareProduction(tokenName);

    productionToken(id);
    return id;
  }

  OSid Grammar::productionIdentifierDecl(const_cstring typeName)
  {
    /* OLD: OSid typeHash = identifierTypes.insert(typeName).second;*/
    hash_set<const char*>::iterator i = identifierTypes.find(typeName);
    OSid typeHash = identifierTypes.hash_funct()(typeName);
    activeProductionSymbols.push_back(Production::Symbol(ID_IDENTIFIER_DECL, typeHash));
    return typeHash;
  }

  void Grammar::productionIdentifierRef(OSid type)
  {
    activeProductionSymbols.push_back(Production::Symbol(ID_IDENTIFIER_REF, type));
  }

  OSid Grammar::productionIdentifierRef(const_cstring typeName)
  {
    hash_set<const char*>::iterator i = identifierTypes.find(typeName);
    OSid typeHash = identifierTypes.hash_funct()(typeName);
    activeProductionSymbols.push_back(Production::Symbol(ID_IDENTIFIER_REF, typeHash));
    return typeHash;
  }

  OSid Grammar::declareProduction(const_cstring productionName)
  {
    OSid id = -1;

    // Try to find existing an production producing this token name
    TokenIds::const_iterator i = nonterminalIds.find(productionName);
    if(i == nonterminalIds.end())
    {
      // Create a new production id for this production
      id = getNextNonterminalId();
      nonterminalNames[id] = productionName;
      nonterminalIds[productionName] = id;
    }
    else
    {
      // Error? production already exists
      id = i->second;
    }

    return id;
  }

  void Grammar::precedence(const_cstring token1Name, const_cstring token2Name)
  {
    OSid token1 = getTokenId(token1Name);
    OSid token2 = getTokenId(token2Name);
    if(token1 != OSid(-1) && token2 != OSid(-1))
      precedence(token1, token2);
    //else error: incorrect ids
  }

  void Grammar::precedence(OSid token1, OSid token2)
  {
    precedenceMap.insert(std::make_pair(token1, token2));
  }

  void Grammar::grammarStartSymbol(OSid nonterminal)
  {
    OSI_ASSERT(!IsToken(nonterminal));
    startSymbol = nonterminal;
  }

  bool Grammar::checkForwardDeclarations() const
  {
    bool success = true;

    // Check for the existance of all forward declarations
    for(OSid nonterminal = 0; nonterminal < nextNonterminal; nonterminal += 2)
    {
      if(!getProductionSet(nonterminal))
      {
        cout << "Error: Token "<< nonterminal << "\'" << getTokenName(nonterminal) << "\' was not declared." << endl;
        success = false;
      }
    }

    return success;
  }

  OSid Grammar::getNextTerminalId()
  {
    OSid id = nextTerminal;
    nextTerminal += 2;
    return id;
  }

  OSid Grammar::getNextNonterminalId()
  {
    OSid id = nextNonterminal;
    nextNonterminal += 2;
    return id;
  }
  
  OSid Grammar::constructTerminal(const_cstring tokenName, uint bufferLength, uint valueLength)
  {
    OSid id = -1;
    
    // Try to find an existing terminal token with this name
    TokenIds::const_iterator i = terminalIds.find(tokenName);
    if(i == terminalIds.end())
    {
      // Create a token id for this terminal
      id = getNextTerminalId();
      terminalNames[id] = tokenName;
      terminalIds[tokenName] = id;

      // Add a temporary token to the temporary construction set    
      constructionTokens.push_back(Token(id, bufferLength, valueLength));
    }
    else
    {
      // Get existing terminal id
      id = i->second;
    }

    return id;
  }
  
  OSid Grammar::constructNonterminal(const_cstring tokenName)
  {
    OSid id = -1;
    
    // Try to find an existing nonterminal token with this name
    TokenIds::const_iterator i = nonterminalIds.find(tokenName);
    if(i == nonterminalIds.end())
    {
      // Create a new token id for this nonterminal
      id = getNextNonterminalId();
      nonterminalNames[id] = tokenName;
      nonterminalIds[tokenName] = id;
    }
    else
    {
      // Get existing production id
      id = i->second;
    }
    
    return id;
  }

  bool Grammar::isTerminal(OSid id) const
  {
    return (id & 1) == 1;
  }

  /*bool Grammar::isSilent(const ProductionSet& productionSet) const
  {
    for(uint cProduction = 0; cProduction < productionSet.productionsLength; ++cProduction)
    {
        const Production& production = productions[productionSet.productionsOffset + cProduction].first;
        if (production.symbolsLength != 1 || isTerminal(production.symbols[0].id))
          return false;
    }
    
    return true;
  }*/
  
  bool Grammar::isSilent(const Production& production) const
  {
    return production.symbolsLength == 1 && !isTerminal(production.symbols[0].id);
  }

  bool Grammar::isSilent(OSid id) const
  {
    return isTerminal(id) && silentTerminals.find(id) != silentTerminals.end();
  }

  void Grammar::parseFile(const_cstring fileName, ParseResult& parseResult)
  {
    //// Load file
    char* fileData;
    int   fileStreamLength;

    // Open file stream
    ifstream fileStream(fileName, ios_base::in | ios_base::binary | ios_base::ate);

    if(fileStream.fail())
      return; // error

    fileStreamLength = fileStream.tellg();

    // Load file data
    fileData = new char[fileStreamLength]; // todo: perhaps + 1 for an eof character

    try
    {
      fileStream.seekg(0, ios_base::beg);
      fileStream.read(fileData, fileStreamLength);

      if(fileStream.fail())
        throw; // error

      // Close file stream
      fileStream.close();
    }
    catch(...)
    {
      delete[] fileData;
      fileStream.close();
      return; // error
    }

    //// Parse file data
    parseResult.inputStream.data   = fileData;
    parseResult.inputStream.length = fileStreamLength;
    parseResult.inputStream.elementSize = sizeof(char);

    // Lexical analysis (Tokenize the character data)
    parseResult.lexStream.elementSize = sizeof(ParseMatch);
    lexicalAnalysis(parseResult);    

    // Parse
    parseResult.parseStream.elementSize = sizeof(ParseMatch);
    parse(parseResult);
  }

  void Grammar::parseString(const_cstring stringBuffer, ParseResult& parseResult)
  {
    //// Parse file data
    parseResult.inputStream.data       = stringBuffer;
    parseResult.inputStream.length      = strlen(stringBuffer);
    parseResult.inputStream.elementSize = sizeof(char);

    // Lexical analysis (Tokenize the character data)
    parseResult.lexStream.elementSize = sizeof(ParseMatch);
    lexicalAnalysis(parseResult);

    // Parse
    parseResult.parseStream.elementSize = sizeof(ParseMatch);
    parse(parseResult);
  }

  void Grammar::lexicalAnalysis(ParseResult& parseResult)
  {
    vector<ParseMatch>& tokenMatches         = constructMatches;
    const_cstring       parsePosition        = parseResult.inputStream.data;
    const_cstring       lexWordStartPosition = parseResult.inputStream.data;

    while(parsePosition < &parseResult.inputStream.data[parseResult.inputStream.length])
    {
      //bug: const uint remainingLength  = (uint)(&parseResult.inputData[parseResult.inputLength] - parsePosition);
      const uint remainingLength  = (uint)(&parseResult.inputStream.data[parseResult.inputStream.length] - parsePosition + 1);

      // Match raw token, nil token or lex symbol token (symbolic tokens that do not need to be seperated, such as operators)
      ParseMatch tokenSymbolMatch;
      tokenSymbolMatch.offset = (uint)(parsePosition - parseResult.inputStream.data);

      if(parseSymbolToken(RAW_TOKEN, parsePosition, remainingLength, tokenSymbolMatch))
      {
        // Parse all unparsed characters into lex word
        if(lexWordStartPosition != parsePosition)
        {
          ParseMatch tokenWordMatch;
          tokenWordMatch.offset = (uint16)(lexWordStartPosition - parseResult.inputStream.data);
          tokenWordMatch.length = (uint8)(parsePosition - lexWordStartPosition);

          // Parse word token
          parseWordToken(lexWordStartPosition, tokenWordMatch);

          // Add word token to token matches
          tokenMatches.push_back(tokenWordMatch);
        }

        // Add token to token matches
        tokenMatches.push_back(tokenSymbolMatch);

        // Go to next parse position
        parsePosition += tokenSymbolMatch.length;

        // Reset word start position
        lexWordStartPosition = parsePosition;
      }
      else if(parseSymbolToken(NIL_TOKEN, parsePosition, remainingLength, tokenSymbolMatch))
      {
        // Parse all unparsed characters into lex word
        if(lexWordStartPosition != parsePosition)
        {
          ParseMatch tokenWordMatch;
          tokenWordMatch.offset = (uint16)(lexWordStartPosition - parseResult.inputStream.data);
          tokenWordMatch.length = (uint8)(parsePosition - lexWordStartPosition);

          // Parse word token
          parseWordToken(lexWordStartPosition, tokenWordMatch);

          // Add word token to token matches
          tokenMatches.push_back(tokenWordMatch);
        }

        // Ignore nil tokens... (don't add to token matches)

        // Go to next parse position
        parsePosition += tokenSymbolMatch.length;

        // Reset word start position
        lexWordStartPosition = parsePosition;
      }
      else if(parseSymbolToken(LEX_TOKEN, parsePosition, remainingLength, tokenSymbolMatch))
      {
        // Parse all unparsed characters into lex word
        if(lexWordStartPosition != parsePosition)
        {
          ParseMatch tokenWordMatch;
          tokenWordMatch.offset = (uint16)(lexWordStartPosition - parseResult.inputStream.data);
          tokenWordMatch.length = (uint8)(parsePosition - lexWordStartPosition);

          // Parse word token
          parseWordToken(lexWordStartPosition, tokenWordMatch);

          // Add word token to token matches
          tokenMatches.push_back(tokenWordMatch);
        }

        // Add token to token matches
        tokenMatches.push_back(tokenSymbolMatch);

        // Go to next parse position
        parsePosition += tokenSymbolMatch.length;

        // Reset word start position
        lexWordStartPosition = parsePosition;
      }
      else
      {
        // Ignore unparsed character (current character will be evaluated as part of a lex word later)
        ++parsePosition;
      }
    }

    // Parse the final unparsed characters into lex word
    if(lexWordStartPosition != parsePosition)
    {
      ParseMatch tokenWordMatch;
      tokenWordMatch.offset = (uint16)(lexWordStartPosition - parseResult.inputStream.data);
      tokenWordMatch.length = (uint8)(parsePosition - lexWordStartPosition);

      // Parse word token
      parseWordToken(lexWordStartPosition, tokenWordMatch);

      // Add word token to token matches
      tokenMatches.push_back(tokenWordMatch);
    }

    parseResult.lexStream.length = (uint)tokenMatches.size();
    parseResult.lexStream.data = new ParseMatch[parseResult.lexStream.length];
    memcpy(parseResult.lexStream.data, &tokenMatches[0], sizeof(ParseMatch)*parseResult.lexStream.length);
  }

  bool Grammar::parseSymbolToken(TokenType tokenType, const_cstring inputPosition, uint inputLength, ParseMatch& tokenMatch) const
  {
    const TokenRootIndex* const& tokenRootIndices = Grammar::tokenRootIndices[tokenType];
    Token* const&                tokens           = Grammar::tokens[tokenType];
    const char                   rootCharacter    = *inputPosition;

    // Lookup token root character
    const TokenRootIndex& tokenRootIndex = tokenRootIndices[rootCharacter];

    // Parse possible characters
    for(uint cToken = 0; cToken < tokenRootIndex.length; ++cToken)
    {
      const Token& token = tokens[tokenRootIndex.offset + cToken];

      if(matchSymbolToken(token, inputPosition, inputLength, tokenMatch.length))
      {
        tokenMatch.id = token.id;
        return true; // token match found
      }
    }

    return false; // no token match found
  }

  bool Grammar::matchSymbolToken(const Token& token, const_cstring inputPosition, uint inputLength, uint16& matchLength) const
  {
    // Precondition:
    OSI_ASSERT(token.valueLength >= 1);

    if(tokenCharacters[token.valueOffset] == SPECIAL_SINGLELINE_BOUNDING_CHAR || tokenCharacters[token.valueOffset] == SPECIAL_MULTILINE_BOUNDING_CHAR)
      return matchBoundingToken(token, inputPosition, inputLength, matchLength);

    uint c;
    for(c = 0; c < (uint)(token.valueLength - 1); ++c)
    {
      if(c >= inputLength)
        return false; // no match

      if(inputPosition[c] != tokenCharacters[token.valueOffset + c])
        return false; // no match
    }

    matchLength = c;
    return true;
  }

  bool Grammar::matchBoundingToken(const Token& token, const_cstring inputPosition, uint inputLength, uint16& matchLength) const
  {
    //todo: refactor a little? (store lengths of boundary strings during matches)

    /*const_cstring tokenValue = token.value + 1;*/
    const_cstring const tokenValue = &tokenCharacters[token.valueOffset];
    const_cstring tokenValuePosition = &tokenValue[1];

    // Match starting boundary
    uint cInput = 0; // input counter

    while(true)
    {
      // Test whether remaining characters can contain token boundary strings
      //BUG: if(cInput >= int(inputLength) - (token.valueLength - 3))
      if((int) cInput > int(inputLength) - (token.valueLength - 3))
        return false; // no match

      // Test whether end of the first boundary string has been reached
      if(tokenValuePosition[cInput] == '\0')
      {
        tokenValuePosition += cInput + 1; // skip closing '\0' character
        break; // end of boundary string
      }

      if(inputPosition[cInput] != tokenValuePosition[cInput])
        return false; // no match

      ++cInput;
    }

    // Find end-of-line if the token type is SINGLE_LINE
    // todo: test
    if((tokenValuePosition[0] == PARSER_TOKEN_VALUE_EOF[0] // todo: remove the EOF concept in favour of \0??
        || tokenValuePosition[0] == '\0'))
    {
      while(cInput < inputLength)
      {
        if(tokenCharacters[token.valueOffset] == SPECIAL_SINGLELINE_BOUNDING_CHAR && inputPosition[cInput] == '\n')
          break;
        else // todo: perhaps make multi-line empty boundary illegal???
          OSI_ASSERT(tokenCharacters[token.valueOffset] == SPECIAL_MULTILINE_BOUNDING_CHAR);

        ++cInput;
      }

      matchLength = cInput;
      return true;
    }

    // Find ending boundary
    bool match;

    while(true)
    {
      // Test whether remaining characters can contain the length of the token's closing boundary string
      //BUG: if(cInput >= int(inputLength) - (token.valueLength - (uint)(tokenValuePosition - tokenValue)))
      if(cInput > int(inputLength) - (token.valueLength - (uint)(tokenValuePosition - tokenValue)))
        return false;

      // Match the token's ending boundary
      uint cTokenValue; // token value counter (token characters)
      match = true;

      for(cTokenValue = 0; cTokenValue < (uint)(token.valueLength - (tokenValuePosition - tokenValue) - 1); ++cTokenValue)
      {
        if(inputPosition[cInput + cTokenValue] != tokenValuePosition[cTokenValue])
        {
          match = false;
          break;
        }
      }

      if(match)
      {
        matchLength = cInput + cTokenValue;
        return true;
      }

      // Test for end-of-line and return an error if the bounding token is a single-line type
      // (Note: if the \n char is part of the end boundary, the lexer will first try to match this
      //  before returning a single-line mismatch here)
      // todo: test
      if(inputPosition[cInput] == '\n'
          && tokenCharacters[token.valueOffset] == SPECIAL_SINGLELINE_BOUNDING_CHAR)
        return false; // error: single-line mismatch (no ending boundary string found for boundary token)

      ++cInput;
    }

    return false;
  }

  void Grammar::parseWordToken(const_cstring inputPosition, ParseMatch& tokenMatch) const
  {
    const TokenRootIndex* const& tokenRootIndices = lexWordTokenRootIndices;
    Token* const&                tokens           = lexWordTokens;
    const char                   rootCharacter    = *inputPosition;

    // Lookup token root character
    const TokenRootIndex& tokenRootIndex = tokenRootIndices[rootCharacter];

    // Parse possible characters
    for(uint cToken = 0; cToken < tokenRootIndex.length; ++cToken)
    {
      const Token& token = tokens[tokenRootIndex.offset + cToken];

      // Use input length to quickly discard token words
      if(token.valueLength - 1 != tokenMatch.length)
        continue;

      // Match token
      if(matchWordToken(token, inputPosition))
      {
        tokenMatch.id = token.id;
        return; // token match found
      }
    }

    // Test whether word is a numeric constant or an identifier
    // Notes: Negative numerals are separated from their unary - operator, so no need to parse for '-' characters.
    //        Whether identifier is a reference or a delcaration is only determined during the parsing stage...
    //        The identifier merging extension also occurs in the parsing stage.
    // Todo:  Periods are also separated into tokens, hence it's the parser's task to put together real numbers...
    //        (But is this correct?)
    if(rootCharacter >= '0' && rootCharacter <= '9')
      tokenMatch.id = ID_CONST_NUM;
    else
      tokenMatch.id = ID_IDENTIFIER;
  }

  bool Grammar::matchWordToken(const Token& token, const_cstring inputPosition) const
  {
    for(uint c = 0; c < (uint)(token.valueLength - 1); ++c)
      /*if(token.value[c] != inputPosition[c])*/
      if(tokenCharacters[token.valueOffset + c] != inputPosition[c])
        return false;

    return true;
  }

  const string& Grammar::getTokenName(OSid tokenId) const
  {
    static const string tokenUnknownTerminal("[Unknown Terminal]");
    static const string tokenUnknownNonterminal("[Unknown Nonterminal]");
    static const string tokenSpecial("[Special (EOF?)]");
    static const string tokenIdentifier("[Identifier]");
    static const string tokenIdentifierDecl("[Identifier Decl]");
    static const string tokenIdentifierRef("[Identifier Ref]");
    static const string tokenConstNumeric("[Numeric Const]");

    if(isTerminal(tokenId))
    {
      if(int(tokenId) < ID_FIRST_TERMINAL)
      {
        switch(tokenId)
        {
        case ID_IDENTIFIER:      return tokenIdentifier;
        case ID_CONST_NUM:       return tokenConstNumeric;
        case ID_SPECIAL:         return tokenSpecial;
        case ID_IDENTIFIER_DECL: return tokenIdentifierDecl;
        case ID_IDENTIFIER_REF:  return tokenIdentifierRef;
        default:
          OSI_ASSERT(false);
          return tokenUnknownTerminal;
        }
      }

      TokenNames::const_iterator i = terminalNames.find(tokenId);
      if(i != terminalNames.end() && i->first == tokenId)
        return i->second;
      
      return tokenUnknownTerminal;
    }
    else
    {
      TokenNames::const_iterator i = nonterminalNames.find(tokenId);
      if(i != nonterminalNames.end() && i->first == tokenId)  
        return i->second;
      
      return tokenUnknownNonterminal;
    }
  }
  const Grammar::ProductionSet* Grammar::getProductionSet(OSid nonterminal) const
  {
    map<OSid, ProductionSet*>::const_iterator i = productionSets.find(nonterminal);
    if(i == productionSets.end() || i->first != nonterminal)
      return null;
    return i->second;
  }

  Grammar::ProductionSet* Grammar::getProductionSet(OSid nonterminal)
  {
    map<OSid, ProductionSet*>::iterator i = productionSets.find(nonterminal);
    if(i == productionSets.end() || i->first != nonterminal)
      return null;
    return i->second;
  }

  OSid Grammar::getTokenId(const_cstring tokenName) const
  {
    // Try to find a terminal token with this name
    TokenIds::const_iterator i = terminalIds.find(tokenName);
    if(i != terminalIds.end())
      return i->second;

    // Try to find a nonterminal token with this name
    i = nonterminalIds.find(tokenName);
    if(i != nonterminalIds.end())
      return i->second;
    
    return OSid(-1);  // no matching token id found
  }

#ifdef _DEBUG
  void Grammar::debugOutputTokens() const
  {
    //todo: const const_cstring tokenTypeHeadings[] = { "Raw Tokens", "Nil Tokens", "Lex Tokens" };

    cout << endl
         << "Tokens" << endl
         << "------" << endl;

    /* OLD: This should be replaced
    for(uint cTokenType = 0; cTokenType < 3; ++cTokenType)
    {
      // Output token type heading
      cout << endl << tokenTypeHeadings[cTokenType] << ':' << endl;

      // Output tokens
      const TokenNames& tokenNames = Grammar::[cTokenType];

      for(TokenNames::iterator i = tokenNames.begin(); i != tokenNames.end(); ++i)
        cout << ' ' << i->name << endl;
    }*/
    
    for(TokenNames::const_iterator i = terminalNames.begin(); i != terminalNames.end(); ++i)
        cout << ' ' << i->second << endl;
  }

  void Grammar::debugOutputProduction(const Production& production) const
  {
    for(uint c = 0; c < production.symbolsLength; ++c)
    {
      debugOutputSymbol(production.symbols[c].id);
      if(int(c) < production.symbolsLength-1)
        cout << ' ';
    }
  }

  void Grammar::debugOutputProduction(OSid id, const Production& production) const
  {
    debugOutputSymbol(id);
    cout << " -> ";
    debugOutputProduction(production);
  }

  void Grammar::debugOutputProductions() const
  {
    cout << endl
         << "Productions" << endl
         << "-----------" << endl;
    for(map<OSid, ProductionSet*>::const_iterator i = productionSets.begin(); i != productionSets.end(); ++i)
    {
      OSid productionId = i->first;

      for(uint cProduction = 0; cProduction < i->second->productionsLength; ++cProduction)
      {
        const Production& production = productions[i->second->productionsOffset + cProduction].first;
        debugOutputProduction(productionId, production);
        cout << endl;
      }
    }
  }

  void Grammar::outputStatementMatch(ParseResult& result, uint& index) const
  {
    ParseMatch& match = result.parseStream.data[index];
    if(isTerminal(match.id))
      cout << getTokenName(match.id);
    else
    {
      cout << getTokenName(match.id) << " { ";

      /*OLD:
      for(uint c = match.offset; c < (uint)(match.offset + match.length); ++c)
      {
        outputStatementMatch(result, c);
        cout << ' ';
      }*/

      for(uint c = 0; c < (uint)match.length; ++c)
      {
        ++index;
        outputStatementMatch(result, index);
        cout << ' ';
      }

      cout << "} ";
    }
  }

  void Grammar::debugOutputParseResult(OSobject& parseResult) const
  {
    ParseResult& result = *(ParseResult*)parseResult;

    cout << "Parse Result:" << endl << endl;

    // Output lex tokens
    cout << "Lex tokens:" << endl;
    for(uint c = 0; c < result.lexStream.length; ++c)
      cout << getTokenName(result.lexStream.data[c].id) << ' ';
    cout << endl << endl;

    // Output global statements
    cout << "Statements:" << endl;
    //for(uint c = 0; c < result.globalMatchesLength; ++c)
    if(result.parseStream.length > 0)
    {
      //outputStatementMatch(result, c);
      uint index = 0;
      outputStatementMatch(result, index);
      cout << endl << endl;
    }

    for(uint c = 0; c < result.parseStream.length; ++c)
    {
      debugOutputSymbol(result.parseStream.data[c].id);
      cout << ' ';
    }
    cout << endl;
    cout.flush();
  }

  void Grammar::debugOutputSymbol(OSid symbol) const
  {
      cout << getTokenName(symbol);
  }
#endif

  void Grammar::setErrorStream(FILE* stream)
  {
    errorStream.flush();
    delete errorStream.rdbuf(new stdio_filebuf<char>(stream, std::ios::out));
  }

  void Grammar::setWarningStream(FILE* stream)
  {
    warnStream.flush();
    delete warnStream.rdbuf(new stdio_filebuf<char>(stream, std::ios::out));
  }

  void Grammar::setInfoStream(FILE* stream)
  {
    infoStream.flush();
    delete infoStream.rdbuf(new stdio_filebuf<char>(stream, std::ios::out));
  }

}

#endif
#endif
