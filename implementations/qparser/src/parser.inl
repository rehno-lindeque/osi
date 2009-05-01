#ifdef  __QPARSER_PARSER_H__
#ifndef __QPARSER_PARSER_INL__
#define __QPARSER_PARSER_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    PARSER.INL
//
//    Copyright © 2007-2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QParser
{
  ParserImplementation::ParserImplementation() : //activeTokenType(static_cast<Lexer::TokenType>(0)),
                       //activeSubTokenType(static_cast<Lexer::SubTokenType>(0)),
                       //lexer(tokenRegistry),
                       errorStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out)),
                       warnStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out)),
                       infoStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out))
  {
  }

  ParserImplementation::~ParserImplementation()
  {
    errorStream.flush();
    warnStream.flush();
    infoStream.flush();

    delete errorStream.rdbuf(null);
    delete warnStream.rdbuf(null);
    delete infoStream.rdbuf(null);
  }

  /*void ParserImplementation::ConstructTokens()
  {
    uint&             nTokens                      = ParserImplementation::nTokens[activeTokenType + activeSubTokenType];
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

  void ParserImplementation::ParseFile(const_cstring fileName, ParseResult& parseResult)
  {
    /*using std::ios_base;
    
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
    ReshuffleResult(parseResult);*/
  }

  void ParserImplementation::ParseString(const_cstring stringBuffer, ParseResult& parseResult)
  {
    /*//// Parse file data
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
    ReshuffleResult(parseResult);*/
  }

  /*INLINE void ParserImplementation::ReshuffleResult(ParseResult& parseResult)
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
  }*/

/*#ifdef _DEBUG
  void ParserImplementation::OutputStatementMatch(ParseResult& result, uint index) const
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
 
  void ParserImplementation::DebugOutputTokens() const
  {
    std::cout << std::endl
         << "Tokens" << std::endl
         << "------" << std::endl;
    for(TokenNames::const_iterator i = terminalNames.begin(); i != terminalNames.end(); ++i)
        std::cout << ' ' << i->second << std::endl;
  }

  void ParserImplementation::DebugOutputProduction(const Production& production) const
  {
    for(uint c = 0; c < production.symbolsLength; ++c)
    {
      debugOutputSymbol(production.symbols[c].token);
      if(int(c) < production.symbolsLength-1)
        std::cout << ' ';
    }
  }

  void ParserImplementation::DebugOutputProduction(OSid id, const Production& production) const
  {
    debugOutputSymbol(id);
    std::cout << " -> ";
    debugOutputProduction(production);
  }

  void ParserImplementation::DebugOutputProductions() const
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

  void ParserImplementation::DebugOutputParseResult(OSobject& parseResult) const
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

  void ParserImplementation::DebugOutputSymbol(OSid symbol) const
  {
      std::cout << getTokenName(symbol);
  }
#endif*/

  void ParserImplementation::SetErrorStream(FILE* stream)
  {
    errorStream.flush();
    delete errorStream.rdbuf(new STDEXT_NAMESPACE::stdio_filebuf<char>(stream, std::ios::out));
  }

  void ParserImplementation::SetWarningStream(FILE* stream)
  {
    warnStream.flush();
    delete warnStream.rdbuf(new STDEXT_NAMESPACE::stdio_filebuf<char>(stream, std::ios::out));
  }

  void ParserImplementation::SetInfoStream(FILE* stream)
  {
    infoStream.flush();
    delete infoStream.rdbuf(new STDEXT_NAMESPACE::stdio_filebuf<char>(stream, std::ios::out));
  }

}

#endif
#endif
