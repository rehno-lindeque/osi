#ifdef  __QPARSER_GRAMMAR_H__
#ifndef __QPARSER_GRAMMAR_INL__
#define __QPARSER_GRAMMAR_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMAR.INL
//
//    Copyright © 2007-2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QParser
{
  Grammar::Grammar() : //activeTokenType(static_cast<Lexer::TokenType>(0)),
                       //activeSubTokenType(static_cast<Lexer::SubTokenType>(0)),
                       lexer(tokenRegistry),
                       errorStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out)),
                       warnStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out)),
                       infoStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out)),
                       activeProduction(null),
                       startSymbol(-1)
  {
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

  /*void Grammar::ConstructTokens()
  {
    uint&             nTokens                      = Grammar::nTokens[activeTokenType + activeSubTokenType];
    LexMatch*&        activeTokens                 = tokens[activeTokenType + activeSubTokenType];
    TokenRootIndex(&  activeTokenRootIndices)[256] = tokenRootIndices[activeTokenType + activeSubTokenType];

    //// Consolidate tokens (Copy tokens to a fixed array & construct a root character index)
    // Allocate token array
    nTokens = (uint)constructionTokens.size();
    activeTokens = new LexMatch[nTokens];

    // Clear token root indices
    memset(activeTokenRootIndices, 0, sizeof(TokenRootIndex) * nTokens);

    // Copy tokens to token array (ordered by their parse values)
    {
      uint                           cToken;
      TokenConstructionSet::iterator iToken;

      for(cToken = 0, iToken = constructionTokens.begin(); cToken < nTokens; ++cToken, ++iToken)
      {
        // Add to token "root character" indexing table (optimization index)
        {
          char rootCharacter = tokenCharacters[(*iToken).valueOffset];
          if(rootCharacter == SPECIAL_MULTILINE_BOUNDING_CHAR
              || rootCharacter == SPECIAL_SINGLELINE_BOUNDING_CHAR)
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
            memmove(&activeTokens[tokenRootIndex.offset + tokenRootIndex.length + 1], &activeTokens[tokenRootIndex.offset + tokenRootIndex.length], sizeof(LexMatch[cToken - (tokenRootIndex.offset + tokenRootIndex.length)]));
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
  }*/

  // Productions
  ParseToken Grammar::BeginProduction(const_cstring productionName)
  {
    // Construct a nonterminal token for this production (if none exists)
    ParseToken token = ConstructNonterminal(productionName);

    // Get a production set
    ProductionSet *productionSet = null;
    {
      std::map<ParseToken, ProductionSet*>::iterator i = productionSets.find(token);
      if(i == productionSets.end() || i->first != token)
      {
        productionSet = new ProductionSet();
        productionSets[token] = productionSet;
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

    // Add production to set
    {
      productions.push_back(std::make_pair(Production(), token));
      activeProduction = &productions[productions.size()-1].first;
    }

    return token;
  }

  void Grammar::EndProduction()
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

  void Grammar::ProductionToken(ParseToken token)
  {
    OSI_ASSERT(tokenRegistry.IsTemporaryToken(token) || tokenRegistry.IsTokenValid(token));
    activeProductionSymbols.push_back(token);
  }

  ParseToken Grammar::ProductionToken(const_cstring tokenName)
  {
    ParseToken token = tokenRegistry.GetToken(tokenName);
    if(token == ParseToken(-1))
      token = DeclareProduction(tokenName);
    ProductionToken(token);
    return token;
  }

  ParseToken Grammar::ProductionIdentifierDecl(const_cstring typeName)
  {
    /*hash_set<const char*>::iterator i = identifierTypes.find(typeName);
    ParseToken typeHash = identifierTypes.hash_funct()(typeName);
    activeProductionSymbols.push_back(Production::Symbol(ID_IDENTIFIER_DECL, typeHash));
    return typeHash;*/
    
    // todo: BUSY HERE... (REWRITE)
    activeProductionSymbols.push_back(TOKEN_TERMINAL_IDENTIFIER);
    return TOKEN_TERMINAL_IDENTIFIER;
  }

  /*void Grammar::ProductionIdentifierRef(ParseToken type)
  {
    activeProductionSymbols.push_back(Production::Symbol(ID_IDENTIFIER_REF, type));
    
    // todo: BUSY HERE... (REWRITE)
    activeProductionSymbols.push_back(TOKEN_TERMINAL_IDENTIFIER);
  }*/

  ParseToken Grammar::ProductionIdentifierRef(const_cstring typeName)
  {
    /*hash_set<const char*>::iterator i = identifierTypes.find(typeName);
    OSid typeHash = identifierTypes.hash_funct()(typeName);
    activeProductionSymbols.push_back(Production::Symbol(ID_IDENTIFIER_REF, typeHash));
    return typeHash;*/
    
    // todo: BUSY HERE... (REWRITE)
    activeProductionSymbols.push_back(TOKEN_TERMINAL_IDENTIFIER);
    return TOKEN_TERMINAL_IDENTIFIER;
  }

  ParseToken Grammar::DeclareProduction(const_cstring productionName)
  {
    return tokenRegistry.FindOrGenerateTemporaryNonterminal(productionName);
  }

  void Grammar::Precedence(const_cstring token1Name, const_cstring token2Name)
  {
    ParseToken token1 = tokenRegistry.GetToken(token1Name);
    ParseToken token2 = tokenRegistry.GetToken(token2Name);
    if(token1 != ParseToken(-1) && token2 != ParseToken(-1))
      Precedence(token1, token2);
    //else error: incorrect ids
  }

  void Grammar::Precedence(ParseToken token1, ParseToken token2)
  {
    precedenceMap.insert(std::make_pair(token1, token2));
  }

  void Grammar::GrammarStartSymbol(ParseToken nonterminal)
  {
    OSI_ASSERT(!TokenRegistry::IsTerminal(nonterminal));
    startSymbol = nonterminal;
  }

  bool Grammar::CheckForwardDeclarations() const
  {
    bool success = true;

    // Check for the existence of all forward declarations
    for(ParseToken nonterminal = TOKEN_NONTERMINAL_FIRST; nonterminal < tokenRegistry.GetNextAvailableNonterminal(); ++nonterminal)
    {
      if(!GetProductionSet(nonterminal))
      {
        std::cout << "Error: Token "<< nonterminal << "\'" << tokenRegistry.GetTokenName(nonterminal) << "\' was not declared." << std::endl;
        success = false;
      }
    }

    return success;
  }

  /*ParseToken Grammar::ConstructTerminal(const_cstring tokenName, uint bufferLength, uint valueLength)
  {
    / *ParseToken token = -1;
    
    // Try to find an existing terminal token with this name
    TokenIds::const_iterator i = terminalIds.find(tokenName);
    if(i == terminalIds.end())
    {
      // Create a token id for this terminal
      token = getNextAvailableTerminal();
      terminalNames[id] = tokenName;
      terminalIds[tokenName] = id;

      // Add a temporary token to the temporary construction set    
      constructionTokens.push_back(LexMatch(id, bufferLength, valueLength));
    }
    else
    {
      // Get existing terminal id
      id = i->second;
    }

    return id;* /
    
    // todo: BUSY HERE... (REWRITE)
    
    return token;
  }*/
  
  ParseToken Grammar::ConstructNonterminal(const_cstring tokenName)
  {
    /*ParseToken token = -1;
    // Try to find an existing nonterminal token with this name
    TokenIds::const_iterator i = nonterminalIds.find(tokenName);
    if(i == nonterminalIds.end())
    {
      // Create a new token id for this nonterminal
      token = getNextAvailableNonterminal();
      nonterminalNames[token] = tokenName;
      nonterminalValues[tokenName] = token;
    }
    else
    {
      // Get the existing production id
      token = i->second;
      
      // If the id was forward declared we need to substitute this id (and all uses there of) with the new correct id
      if(token > nextNonterminal)
      {
        ParseToken newToken = getNextAvailableNonterminal();
        nonterminalNames[newToken] = tokenName;
        nonterminalIds[tokenName] = newToken;
        replaceAllTokens(token, newToken);
        return newToken;
      }
    }
    
    return token;*/
    
    // TODO: BUSY HERE (rewriting this)
    
    ParseToken token = tokenRegistry.GetToken(tokenName);
    
    // If the token is a temporary token, we need to substitute it with a new
    // token and replace all instances where the token was used with the proper
    // value
    if(tokenRegistry.IsTemporaryToken(token))
    {
      ParseToken newToken = tokenRegistry.GenerateNonterminal(tokenName);
      ReplaceAllTokens(token, newToken);
      return newToken;
    }
    // If the token does not exist yet, we generate one
    else if(token == ParseToken(-1))
    {
      token = tokenRegistry.GenerateNonterminal(tokenName);
    }    
    return token;    
  }
  
  void Grammar::ReplaceAllTokens(ParseToken oldToken, ParseToken newToken)
  {
    for(std::vector< std::pair<Production, ParseToken> >::iterator i = productions.begin(); i != productions.end(); ++i)
    {
      Production& production = i->first;
      
      for(uint c = 0; c < production.symbolsLength; ++c)
      {
        if(production.symbols[c].token == oldToken)
          production.symbols[c].token = newToken;
      }
    }
  }

  bool Grammar::IsSilent(const Production& production) const
  {
    return production.symbolsLength == 1 && !TokenRegistry::IsTerminal(production.symbols[0].token);
  }

  bool Grammar::IsSilent(ParseToken token) const
  {
    return TokenRegistry::IsTerminal(token) && silentTerminals.find(token) != silentTerminals.end();
  }

  void Grammar::ParseFile(const_cstring fileName, ParseResult& parseResult)
  {
    using std::ios_base;
    
    //// Load file
    char* fileData;
    int   fileStreamLength;

    // Open file stream
    std::ifstream fileStream(fileName, ios_base::in | ios_base::binary | ios_base::ate);

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
    lexer.LexicalAnalysis(parseResult);    

    // Parse
    parseResult.parseStream.elementSize = sizeof(ParseMatch);
    Parse(parseResult);
    
    // Reshuffle the result so that it is easier to traverse
    ReshuffleResult(parseResult);
  }

  void Grammar::ParseString(const_cstring stringBuffer, ParseResult& parseResult)
  {
    //// Parse file data
    parseResult.inputStream.data       = stringBuffer;
    parseResult.inputStream.length      = strlen(stringBuffer);
    parseResult.inputStream.elementSize = sizeof(char);

    // Lexical analysis (Tokenize the character data)
    parseResult.lexStream.elementSize = sizeof(ParseMatch);
    lexer.LexicalAnalysis(parseResult);

    // Parse
    parseResult.parseStream.elementSize = sizeof(ParseMatch);
    Parse(parseResult);
    
    // Reshuffle the result so that it is easier to traverse
    ReshuffleResult(parseResult);
  }

  INLINE void Grammar::ReshuffleResult(ParseResult& parseResult)
  {
    using std::vector;
    
    // Reshuffle the tokens so that we can more easily traverse the parse tree (breadth-first traversal)
    // ALGORITHM: We put all tokens on the same level next to each other. We shall traverse the current 
    //            parse tree in a depth-first manner, counting the number of tokens at each level.
    //            This will allow us to build a table of offsets into the final buffer. From here we 
    //            can simply copy each element, updating (and keeping track of) each level's current index.
    //
    
    //// Build a table of indices corresponding to parse levels
    vector<uint> indices;
    indices.push_back(0);      // The first level's index starts at 0
    int level = 0;
    std::stack<int> lengthCounters; // Length counters will be used at every level to determine when to decrement the current level
    int lengthCounter = 1;
    for (uint c = 0; c < parseResult.parseStream.length; ++c)
    {
      // Decrement the current level if we reach the end of a production
      while (lengthCounter == 0)
      {
        // Pre-condition: We should never be able to reach this if we're at the top level.
        //                Otherwise the parseStream is corrupt.
        OSI_ASSERT(level != 0 && lengthCounters.size() != 0);
        
        --level;
        lengthCounter = lengthCounters.top();
        lengthCounters.pop();
        
        OSI_DEBUG(std::cout << level << "} ");
        OSI_DEBUG(std::cout.flush());
      }
      
      // Increment the next level's index
      if(indices.size() <= static_cast<size_t>(level+1))
        indices.push_back(1);
      else
        ++indices[level+1];
      
      // Decrement the remaining length in this production
      --lengthCounter;
      
      // Get the match we are traversing
      const ParseMatch& currentMatch = parseResult.parseStream.data[c];
      
      OSI_DEBUG(std::cout << tokenRegistry.GetTokenName(currentMatch.token) << ' ');
      OSI_DEBUG(std::cout.flush());
      
      // Increment current level if we reach a non-terminal node
      // (I.e. we've reached the beginning of a production)
      if(!TokenRegistry::IsTerminal(currentMatch.token) && currentMatch.length > 0)
      {
        OSI_DEBUG(std::cout << "{" << level << ' ');
        OSI_DEBUG(std::cout.flush());
        ++level;
        lengthCounters.push(lengthCounter);
        lengthCounter = currentMatch.length;
      }
    }
    
    OSI_DEBUG(std::cout << std::endl << std::endl);
    
    // Clear loop variables (counters etc)
    // Note: The above algorithm will not traverse up to the top-level at the end because it 
    //       ends as soon as it runs out of tokens - this is not a bug.
    while(!lengthCounters.empty())
      lengthCounters.pop();
    level = 0;    
    lengthCounter = 1;
    
    // Finally add up the lengths sequentially to find the final offsets for each table
    uint currentIndex = 0;
    for(vector<uint>::iterator i = indices.begin(); i != indices.end(); ++i)
    {
      *i += currentIndex;
      currentIndex = *i;
    }
    
    //// Copy each element over to the new buffer using indices+offsets to find the correct position each time
    ParseMatch shuffleData[parseResult.parseStream.length];
    vector<uint> offsets;
    offsets.resize(indices.size(), 0);//TODO: how do we set all elements to 0? IS THIS CORRECT?
    for (uint c = 0; c < parseResult.parseStream.length; ++c)
    {
      // Decrement the current level if we reach the end of a production
      while (lengthCounter == 0)
      {
        // Pre-condition: We should never be able to reach this if we're at the top level.
        //                Otherwise the parseStream is corrupt.
        OSI_ASSERT(level != 0 && lengthCounters.size() != 0);
        
        --level;
        lengthCounter = lengthCounters.top();
        lengthCounters.pop();
        
        OSI_DEBUG(std::cout << level << "} ");
        OSI_DEBUG(std::cout.flush());
      }
      
      // Decrement the remaining length in this production
      --lengthCounter;
      
      // Get the match we are traversing
      ParseMatch& currentMatch = parseResult.parseStream.data[c];
      
      OSI_DEBUG(std::cout << tokenRegistry.GetTokenName(currentMatch.token) << ' ');
      OSI_DEBUG(std::cout.flush());
      
      // Update the offset of this node since the (future) index of its children is now known
      if(!TokenRegistry::IsTerminal(currentMatch.token))
        currentMatch.offset = indices[level+1] + offsets[level+1];
      
      // Copy the match data to the shuffle buffer
      OSI_ASSERT(indices.size() > static_cast<size_t>(level));
      uint shuffleIndex = indices[level] + offsets[level];
      shuffleData[shuffleIndex] = currentMatch;
      
      // Increment the current level's offset since we've copied a token
      ++offsets[level];
      
      // Increment current level if we reach a non-terminal node
      // (I.e. we've reached the beginning of a production)
      if(!TokenRegistry::IsTerminal(currentMatch.token) && currentMatch.length > 0)
      {
        OSI_DEBUG(std::cout << "{" << level << ' ');
        OSI_DEBUG(std::cout.flush());
        
        ++level;
        lengthCounters.push(lengthCounter);
        lengthCounter = currentMatch.length;
      }
    }
    
    OSI_DEBUG(std::cout << std::endl << std::endl);
    
    // Copy the shuffle data back into the parse stream buffer
    memcpy(parseResult.parseStream.data, shuffleData, parseResult.parseStream.length * parseResult.parseStream.elementSize);
  }

  const Grammar::ProductionSet* Grammar::GetProductionSet(ParseToken nonterminal) const
  {
    std::map<ParseToken, ProductionSet*>::const_iterator i = productionSets.find(nonterminal);
    if(i == productionSets.end() || i->first != nonterminal)
      return null;
    return i->second;
  }

  Grammar::ProductionSet* Grammar::GetProductionSet(ParseToken nonterminal)
  {
    std::map<ParseToken, ProductionSet*>::iterator i = productionSets.find(nonterminal);
    if(i == productionSets.end() || i->first != nonterminal)
      return null;
    return i->second;
  }

  std::multimap<ParseToken, ParseToken>::const_iterator Grammar::FindPrecedenceDirective(ParseToken token1, ParseToken token2) const
  {
    std::multimap<ParseToken, ParseToken>::const_iterator i = precedenceMap.lower_bound(token1);
    while(i != precedenceMap.end() && i->first == token1)
    {
      if(i->second == token2)
        return i;
      ++i;
    }
    return precedenceMap.end();
  }

/*#ifdef _DEBUG
  void Grammar::OutputStatementMatch(ParseResult& result, uint index) const
  {
    ParseMatch& match = result.parseStream.data[index];
    if(TokenRegistry::IsTerminal(match.token))
      std::cout << getTokenName(match.token);
    else
    {
      std::cout << getTokenName(match.token) << " { ";

      for(uint c = 0; c < (uint)match.length; ++c)
      {
        outputStatementMatch(result, match.offset+c);
        std::cout << ' ';
      }

      std::cout << "} ";
    }
  }
 
  void Grammar::DebugOutputTokens() const
  {
    std::cout << std::endl
         << "Tokens" << std::endl
         << "------" << std::endl;
    for(TokenNames::const_iterator i = terminalNames.begin(); i != terminalNames.end(); ++i)
        std::cout << ' ' << i->second << std::endl;
  }

  void Grammar::DebugOutputProduction(const Production& production) const
  {
    for(uint c = 0; c < production.symbolsLength; ++c)
    {
      debugOutputSymbol(production.symbols[c].token);
      if(int(c) < production.symbolsLength-1)
        std::cout << ' ';
    }
  }

  void Grammar::DebugOutputProduction(OSid id, const Production& production) const
  {
    debugOutputSymbol(id);
    std::cout << " -> ";
    debugOutputProduction(production);
  }

  void Grammar::DebugOutputProductions() const
  {
    std::cout << std::endl
              << "Productions" << std::endl
              << "-----------" << std::endl;
    for(map<OSid, ProductionSet*>::const_iterator i = productionSets.begin(); i != productionSets.end(); ++i)
    {
      OSid productionId = i->first;

      for(uint cProduction = 0; cProduction < i->second->productionsLength; ++cProduction)
      {
        const Production& production = productions[i->second->productionsOffset + cProduction].first;
        debugOutputProduction(productionId, production);
        std::cout << std::endl;
      }
    }
  }

  void Grammar::DebugOutputParseResult(OSobject& parseResult) const
  {
    ParseResult& result = *(ParseResult*)parseResult;

    std::cout << "Parse Result:" << std::endl << std::endl;

    // Output lex tokens
    std::cout << "Lex tokens:" << std::endl;
    for(uint c = 0; c < result.lexStream.length; ++c)
      std::cout << getTokenName(result.lexStream.data[c].token) << ' ';
    std::cout << std::endl << std::endl;

    // Output global statements
    std::cout << "Statements:" << std::endl;
    if(result.parseStream.length > 0)
    {
      uint index = 0;
      outputStatementMatch(result, index);
      std::cout << std::endl << std::endl;
    }

    for(uint c = 0; c < result.parseStream.length; ++c)
    {
      debugOutputSymbol(result.parseStream.data[c].token);
      std::cout << ' ';
    }
    std::cout << std::endl;
    std::cout.flush();
  }

  void Grammar::DebugOutputSymbol(OSid symbol) const
  {
      std::cout << getTokenName(symbol);
  }
#endif*/

  void Grammar::SetErrorStream(FILE* stream)
  {
    errorStream.flush();
    delete errorStream.rdbuf(new STDEXT_NAMESPACE::stdio_filebuf<char>(stream, std::ios::out));
  }

  void Grammar::SetWarningStream(FILE* stream)
  {
    warnStream.flush();
    delete warnStream.rdbuf(new STDEXT_NAMESPACE::stdio_filebuf<char>(stream, std::ios::out));
  }

  void Grammar::SetInfoStream(FILE* stream)
  {
    infoStream.flush();
    delete infoStream.rdbuf(new STDEXT_NAMESPACE::stdio_filebuf<char>(stream, std::ios::out));
  }

}

#endif
#endif
