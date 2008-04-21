#ifdef __QPARSER_GRAMMAR_H__
#ifndef __QPARSER_GRAMMAR_HXX__
#define __QPARSER_GRAMMAR_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMAR.HXX
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QParser
{
  OSid Grammar::stringToken(const char* tokenName, const char* value)
  {
    // Initialize variables
    const uint    valueLength      = (uint)strlen(value) + 1;
    const uint    bufferLength     = (uint)tokenCharacters.size();
    TokenNameSet& activeTokenNames = tokenNames[activeTokenType];

    // Preconditions
    OSI_ASSERT(valueLength > 0);

    // Concatenate token value to token (character) storage buffer (Note: null character is also concatenated)
    tokenCharacters.resize(bufferLength + valueLength);
    for(uint c = 0; c < valueLength; ++c)
      tokenCharacters[bufferLength + c] = value[c];

    // Determine token id
    const OSid tokenId = getNextTokenId();

    // Add a temporary token to the temporary construction set
    constructionTokens.push_back(Token(tokenId, bufferLength, valueLength));

    // Add token name to the names indexing table
    activeTokenNames.insert(TokenName(tokenName, tokenId));
    allTokenNames.insert(TokenName(tokenName, tokenId));

    return tokenId;
  }

  OSid Grammar::charToken(const char* tokenName, char value)
  {
    // Initialize variables
    const uint    bufferLength     = (uint)tokenCharacters.size();
    TokenNameSet& activeTokenNames = tokenNames[activeTokenType];

    // Preconditions
    OSI_ASSERT(value != '\0');

    // Concatenate token value to token (character) storage buffer
    tokenCharacters.resize(bufferLength + 2);
    tokenCharacters[bufferLength] = value;
    tokenCharacters[bufferLength + 1] = '\0';

    // Determine token id

    //todo: NB! Busy here. Change this so token ids are sequenced rather than
    //      hashed. Tokens and statements share the same id space, hence a token
    //      can't have the same id as a statement. Another lookup table will have to
    //      be created for token \ statement names, but this is fine since
    //      it isn't performance critical.

    //const OSid tokenId = (OSid)hash<const string>()(tokenName);
    const OSid tokenId = getNextTokenId();

    // Add a temporary token to the temporary construction set
    constructionTokens.push_back(Token(tokenId, bufferLength, 2));

    // Add token name to the names indexing table
    activeTokenNames.insert(TokenName(tokenName, tokenId));
    allTokenNames.insert(TokenName(tokenName, tokenId));

    return tokenId;
  }

  OSid Grammar::boundedToken(const char* tokenName, const char* leftBoundingValue, const char* rightBoundingValue)
  {
    const uint    leftValueLength  = (uint)strlen(leftBoundingValue)  + 1;
    const uint    rightValueLength = (uint)strlen(rightBoundingValue) + 1;
    const uint    bufferLength     = (uint)tokenCharacters.size();
    TokenNameSet& activeTokenNames = tokenNames[activeTokenType];

    // Preconditions
    OSI_ASSERT(leftValueLength > 0);
    OSI_ASSERT(rightValueLength > 0);

    // Concatenate token value to token (character) storage buffer
    tokenCharacters.resize(bufferLength + 1 + leftValueLength + rightValueLength);
    tokenCharacters[bufferLength] = '\0'; // special character (the first character is a boundedness indicator)

    for(uint c = 0; c < leftValueLength; ++c)
      tokenCharacters[bufferLength + 1 + c] = leftBoundingValue[c];

    for(uint c = 0; c < rightValueLength; ++c)
      tokenCharacters[bufferLength + 1 + leftValueLength + c] = rightBoundingValue[c];

    // Determine token id
    //const OSid tokenId = (OSid)hash<const string>()(tokenName);
    const OSid tokenId = getNextTokenId();

    // Add a temporary token to the temporary construction set
    constructionTokens.push_back(Token(tokenId, bufferLength, 1 + leftValueLength + rightValueLength));

    // Add to token ids (temporarily until the Token is moved to nilTokens, rawTokens or lexTokens)
    activeTokenNames.insert(TokenName(tokenName, tokenId));
    allTokenNames.insert(TokenName(tokenName, tokenId));

    return tokenId;
  }

  void Grammar::constructTokens()
  {
    uint&             nTokens                      = Grammar::nTokens[activeTokenType + activeSubTokenType];
    Token*&           activeTokens                 = tokens[activeTokenType + activeSubTokenType];
    TokenNameSet&     activeTokenNames             = tokenNames[activeTokenType];
    TokenRootIndex(&  activeTokenRootIndices)[255] = tokenRootIndices[activeTokenType + activeSubTokenType];

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
        activeTokens[cToken] = *iToken;

        // Add to token "root character" indexing table (optimization index)
        {
          char rootCharacter = tokenCharacters[activeTokens[cToken].valueOffset];
          if(rootCharacter == '\0')
            rootCharacter = tokenCharacters[activeTokens[cToken].valueOffset + 1];

          TokenRootIndex& tokenRootIndex = activeTokenRootIndices[rootCharacter];
          if(tokenRootIndex.length == 0)
            tokenRootIndex.offset = cToken;
          ++tokenRootIndex.length;
        }
      }
    }

    // Clear construction tokens
    constructionTokens.clear();
  }

  OSid Grammar::beginStatement(StatementType statementType, const_cstring statementName)
  {
    //OLD: vector<Statement>& activeStatements = statements[statementType];

    // Determine statement id

    //todo: NB! Busy here. Change this so token ids are sequenced rather than
    //      hashed. Tokens and statements share the same id space, hence a token
    //      can't have the same id as a statement. Another lookup table will have to
    //      be created for token \ statement names, but this is fine since
    //      it isn't performance critical.

    //OSid statementId = (OSid)activeStatements.size();
    const OSid statementId = getNextStatementId();

    // Push the old active statement onto the stack
    parentStatementStates.push(activeStatementState);

    // Add to statements
    //OLD: activeStatements.push_back(Statement());

    // Set active statement
    activeStatementState.type      = statementType;
    //OLD: activeStatementState.statement = &activeStatements.back();
    activeStatementState.statement = new Statement();

    // Add to statement id & types map (Temporary??)
    //statementIds.insert(pair<OSid, Statement*>(statementId, activeStatementState.statement));
    statementIds[statementId]   = activeStatementState.statement;
    statementTypes[statementId] = statementType;
    statementNames[statementId] = statementName;

    return statementId;
  }

  void Grammar::endSequence()
  {
    Statement& activeStatement = *activeStatementState.statement;

    //// Consolidate elements (Copy construction elements to fixed array)
    // Create elements array
    OSI_ASSERT(activeStatement.elementsLength <= constructionSequenceElements.size());
    activeStatement.elements        = new SequenceElement[activeStatement.elementsLength];
    uint elementsConstructionOffset = (uint)(constructionSequenceElements.size() - activeStatement.elementsLength);

    // Copy elements to array
    uint cElement;

    for(cElement = 0; cElement < activeStatement.elementsLength; ++cElement)
    {
      ((SequenceElement*)activeStatement.elements)[cElement] = constructionSequenceElements[elementsConstructionOffset + cElement];

      // Post-condition: test for data corruption
      OSI_ASSERT(((SequenceElement*)activeStatement.elements)[cElement].multiplier >= SINGLE_REQUIRED
              && ((SequenceElement*)activeStatement.elements)[cElement].multiplier <= MULTI_OPTIONAL);
    }

    // Clear construction elements
    constructionSequenceElements.erase(constructionSequenceElements.begin() + elementsConstructionOffset, constructionSequenceElements.end());

    // Pop old active statement from stack
    activeStatementState = parentStatementStates.top();
    parentStatementStates.pop();
  }

  void Grammar::endSwitch()
  {
    Statement& activeStatement = *activeStatementState.statement;

    //// Consolidate elements (Copy construction elements to fixed array)
    // Create elements array
    OSI_ASSERT(activeStatement.elementsLength <= constructionSwitchElements.size());
    //activeStatement.elementsLength = (uint8)constructionSwitchElements.size();
    activeStatement.elements        = new StatementElement[activeStatement.elementsLength];
    uint elementsConstructionOffset = (uint)(constructionSwitchElements.size() - activeStatement.elementsLength);

    // Copy elements to array
    uint cElement;

    for(cElement = 0; cElement < activeStatement.elementsLength; ++cElement)
      activeStatement.elements[cElement] = constructionSwitchElements[elementsConstructionOffset + cElement];

    // Clear construction elements
    constructionSwitchElements.erase(constructionSwitchElements.begin() + elementsConstructionOffset, constructionSwitchElements.end());

    // Pop old active statement from stack
    activeStatementState = parentStatementStates.top();
    parentStatementStates.pop();
  }

  void Grammar::sequenceElement(OSid id, PARSER_SEQUENCE_MULTIPLIER multiplier)
  {
    if(global == true)
      globalStatement(id);
    else
    {
      ++activeStatementState.statement->elementsLength;
      constructionSequenceElements.push_back(SequenceElement(id, multiplier));
    }
  }

  void Grammar::switchElement(OSid id)
  {
    ++activeStatementState.statement->elementsLength;
    constructionSwitchElements.push_back(StatementElement(id));
  }

  void Grammar::constructStatements()
  {
    // Construct a deterministic statement root index
    constructStatementRootIndex();

    //// Construct branching graphs (finite state automata?) for non-deterministic statements...
    // Note: We could also construct a state table, but hopefully this
    //       will be more efficient for small low-branching sets...?
    // Todo: Perhaps make a reduction table based on levels instead, but
    //       this will requirement some work-around (perhaps a flag) for
    //       cyclic statement sets
    // todo....


  }

  void Grammar::constructStatementRootIndex()
  {
    /*
        ALGORITHM:
          All sequence statements that start with an id that can only have
          one possible outcome (namely the statement itself) are added into
          an indexing table, indexed by the starting id.
          This indexing table will be used to quickly parse the initial set
          of tokens. Switch statements are not indexed as they are branching
          statements by their very nature.

        TODO:
          + Review technical terminology
          + Fix!!!!!!!
    */

    //struct StatementDeterminant // Element that determines whether a statement root id is deterministic or not
    //{
    //  OSid rootId;
    //  const Statement* statementReference;

    //  struct SetStatementReference
    //  {
    //    const Statement* statementReference;

    //    INLINE SetStatementReference(const Statement* statementReference) : statementReference(statementReference) {}
    //    INLINE void operator()(StatementDeterminant& statementDeterminant) { statementDeterminant.statementReference = statementReference; }
    //  };
    //
    //  INLINE StatementDeterminant(OSid rootId, const Statement* statementReference) : rootId(rootId), statementReference(statementReference) {}
    //};
    //
    //typedef multi_index_container
    //<
    //  StatementDeterminant,
    //  indexed_by< ordered_unique< member<StatementDeterminant, OSid, &StatementDeterminant::rootId> > >
    //> StatementDeterminantSet;

    //typedef StatementDeterminantSet::nth_index_iterator<0>::type DeterminantIterator;

    //StatementDeterminantSet                      statementDeterminants;
    //StatementDeterminantSet::nth_index<0>::type& indexDeterminants = statementDeterminants.get<0>();
    //
    //uint nDeterministicStatements = 0;

    ////// Find all deterministic statements
    //for(vector<Statement>::iterator i = sequenceStatements.begin(); i != sequenceStatements.end(); ++i)
    //{
    //  const Statement& statement = *i;

    //  // Determine whether the statement is deterministic
    //  // Find the first (root) element amongst the statement determinants (and mark as non-deterministic)
    //  OSid rootId = statement.elements[0].id;
    //
    //  if(indexDeterminants.count(rootId) == 0)
    //  {
    //    // Add deterministic statement root to determinants
    //    ++nDeterministicStatements;
    //    statementDeterminants.insert(StatementDeterminant(rootId, &statement));
    //  }
    //  else
    //  {
    //    // Mark existing determinant as non-deterministic
    //    DeterminantIterator iDeterminant = indexDeterminants.find(rootId);
    //    if(iDeterminant->statementReference != null)
    //    {
    //      --nDeterministicStatements;
    //      indexDeterminants.modify(iDeterminant, StatementDeterminant::SetStatementReference(null));
    //    }
    //  }

    //  // Mark all the other elements in the statement as non-deterministic
    //  for(uint cElement = 1; cElement < statement.elementsLength; ++cElement)
    //  {
    //    SequenceElement& element = (SequenceElement&)statement.elements[cElement];
    //
    //    if(indexDeterminants.count(element.id) == 0)
    //    {
    //      // Add non-deterministic determinant to the set
    //      statementDeterminants.insert(StatementDeterminant(element.id, null));
    //    }
    //    else
    //    {
    //      // Mark existing determinant as non-deterministic
    //      DeterminantIterator iDeterminant = indexDeterminants.find(rootId);
    //      if(iDeterminant->statementReference != null)
    //      {
    //        --nDeterministicStatements;
    //        indexDeterminants.modify(iDeterminant, StatementDeterminant::SetStatementReference(null));
    //      }
    //    }
    //  }
    //}

    ////// Construct root index from deterministic statements
    //uint cStatementRootIndex = 0;
    //uint maxOSid = std::max(nextStatementId, nextTokenId);

    //statementRootIndex = new const Statement*[maxOSid];
    //memset(statementRootIndex, 0, sizeof(const Statement*)*maxOSid);
    //
    //for(DeterminantIterator i = indexDeterminants.begin(); i != indexDeterminants.end(); ++i)
    //  statementRootIndex[i->rootId] = i->statementReference;
  }

  void Grammar::beginGlobal()
  {
    global = true;
  }

  void Grammar::globalStatement(OSid statementId)
  {
    // temporary:
    globalStatements.push_back(statementId);
  }

  void Grammar::constructGlobalStatements()
  {
    //todo: Consolidate global statements to a flat array
    //      and build suitable indexes
    global = false;
  }

  OSid Grammar::getNextTokenId()
  {
    OSid id = nextTokenId;
    nextTokenId += 2;

    return id;
  }

  OSid Grammar::getNextStatementId()
  {
    OSid id = nextStatementId;
    nextStatementId += 2;

    return id;
  }

  INLINE bool Grammar::isTokenId(OSid id) const
  {
    return (id & 1) == 1;
  }


  OSobject Grammar::parseFile(const_cstring fileName)
  {
    //// Load file
    char* fileData;
    int   fileStreamLength;

    // Open file stream
    ifstream fileStream(fileName, ios_base::in | ios_base::binary | ios_base::ate);

    if(fileStream.fail())
      return 0; // error

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
      return 0; // error
    }

    //// Parse file data
    ParseResult* const parseResult = new ParseResult;
    parseResult->inputData   = fileData;
    parseResult->inputLength = fileStreamLength;

    // Lexical analysis (Tokenize the character data)
    lexicalAnalysis(*parseResult);

    // Statement reduction
    //statementReduction(*parseResult);

    /* Temporary: */
    deepStatementReduction(*parseResult);

    // Consolidate parse matches
    /*parseResult->matchesLength = (uint)constructMatches.size();
    parseResult->matches       = new ParseMatch[parseResult->matchesLength];

    memcpy(parseResult->matches, &(constructMatches[0]), parseResult->matchesLength * sizeof(ParseMatch));

    constructMatches.clear();*/

    return (OSobject)parseResult;
  }

  void Grammar::lexicalAnalysis(ParseResult& parseResult)
  {
    vector<ParseMatch>& tokenMatches         = constructMatches;
    const char*         parsePosition        = parseResult.inputData;
    const char*         lexWordStartPosition = parseResult.inputData;

    while(parsePosition < &parseResult.inputData[parseResult.inputLength])
    {
      const uint remainingLength  = (uint)(&parseResult.inputData[parseResult.inputLength] - parsePosition);

      // Match raw token, nil token or lex symbol token (symbolic tokens that do not need to be seperated, such as operators)
      ParseMatch tokenSymbolMatch;
      tokenSymbolMatch.offset = (uint)(parsePosition - parseResult.inputData);

      if(parseSymbolToken(RAW_TOKEN, parsePosition, remainingLength, tokenSymbolMatch))
      {
        // Parse all unparsed characters into lex word
        if(lexWordStartPosition != parsePosition)
        {
          ParseMatch tokenWordMatch;
          tokenWordMatch.offset = (uint16)(lexWordStartPosition - parseResult.inputData);
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
          tokenWordMatch.offset = (uint16)(lexWordStartPosition - parseResult.inputData);
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
          tokenWordMatch.offset = (uint16)(lexWordStartPosition - parseResult.inputData);
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

    parseResult.tokenMatchesLength = (uint)tokenMatches.size();
    parseResult.tokenMatches = new ParseMatch[parseResult.tokenMatchesLength];
    memcpy(parseResult.tokenMatches, &tokenMatches[0], sizeof(ParseMatch)*parseResult.tokenMatchesLength);
  }

  bool Grammar::parseSymbolToken(TokenType tokenType, const char* inputPosition, uint inputLength, ParseMatch& tokenMatch) const
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

  bool Grammar::matchSymbolToken(const Token& token, const char* inputPosition, uint inputLength, uint16& matchLength) const
  {
    // Precondition:
    OSI_ASSERT(token.valueLength >= 1);

    /*if(token.value[0] == '\0')*/
    if(tokenCharacters[token.valueOffset] == '\0')
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

  bool Grammar::matchBoundingToken(const Token& token, const char* inputPosition, uint inputLength, uint16& matchLength) const
  {
    //todo: refactor a little? (store lengths of boundary strings during matches)

    /*const char* tokenValue = token.value + 1;*/
    const char* const tokenValue = &tokenCharacters[token.valueOffset];
    const char* tokenValuePosition = &tokenValue[1];

    // Match starting boundary
    uint cInput = 0; // input counter

    while(true)
    {
      // Test whether remaining characters can contain token boundary strings
      if(cInput >= inputLength - (token.valueLength - 3))
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

    // Find ending boundary
    bool match;

    if(tokenValuePosition[0] == PARSER_TOKEN_VALUE_EOF[0])
      return true;

    while(true)
    {
      // Test whether remaining characters can contain the length of the token's closing boundary string
      if(cInput >= inputLength - (token.valueLength - (uint)(tokenValuePosition - tokenValue)))
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

      ++cInput;
    }

    return false;
  }

  void Grammar::parseWordToken(const char* inputPosition, ParseMatch& tokenMatch) const
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

    // Return unknown token (no token match found)
    tokenMatch.id = ID_UNKNOWN_WORD_TOKEN;
  }

  bool Grammar::matchWordToken(const Token& token, const char* inputPosition) const
  {
    for(uint c = 0; c < (uint)(token.valueLength - 1); ++c)
      /*if(token.value[c] != inputPosition[c])*/
      if(tokenCharacters[token.valueOffset + c] != inputPosition[c])
        return false;

    return true;
  }

  /* Todo: In future we will do it like this (flat parsing for better performance and with
           different indexes.

  void Grammar::statementReduction(ParseResult& parseResult)
  {
    vector<ParseMatch> statementMatches; // construction set for statement matches

    // Todo: token reduction on the determinant tokens only

    //// Reduce statements
    uint cParsePosition = 0;

    while(true)
    {
      if(cParsePosition >= parseResult.tokenMatchesLength)
        break;

      // Get the current parse id (at the parse position)
      OSid id = parseResult.tokenMatches[cParsePosition].id;

      // Reduce determinant statement
      if(statementRootIndex[id] != null)
        reduceStatement(parseResult, statementRootIndex[id], cParsePosition);

      ++cParsePosition;
    }
  }

  bool Grammar::reduceStatement(ParseResult& parseResult, const Statement* statement, uint parsePosition)
  {
    /*
        TODO:
          + Parse child statements. These should also technically be 1 level lower than the current statement...
            (Not exactly sure how this is going to work yet)
          + NB! We need to know whether the statement type is a sequence or a switch.
            For parsing determinant statements this is pretty easy because the statement type
            will always be a sequence statement. (Unfortunately the same cannot be said for
            child statements).
    *

    uint matchPosition = (uint)constructMatches.size();

    constructMatches.push_back(ParseMatch());
    ParseMatch& match = constructMatches.back();

    // ASSERT element 0 = id at parsePosition

    for(uint cElement = 1; cElement < statement->elementsLength; ++cElement)
    {
      // todo: NB! ARE THESE STATEMENT ELEMENTS OR SEQUENCE ELEMENTS?
      // todo: parse child statement too........
      if(((StatementElement*)statement->elements)[cElement].id != parseResult.tokenMatches[parsePosition+cElement])
      {
        // Remove child statements from construction set
        constructMatches.erase(constructMatches.begin() + matchPosition, constructMatches.end());
        return false;
      }
    }

    //todo....
    //match.id = statement->id;
    //match.offset
    //match.length

    return true;
  }*/

  void Grammar::deepStatementReduction(ParseResult& parseResult)
  {
    /*
        DESCRIPTION:
          + This is a deep statement reduction algorithm.
            It is intended as a prototype of the future "shallow reduction"
            algorithm and is also to be used for performance comparison.
    */

    vector<ParseMatch> statementMatches; // construction set for statement matches

    //// Reduce statements
    uint cParsePosition = 0;

    while(cParsePosition <= parseResult.tokenMatchesLength)
    {
      // Reduce statement
      if(!deepParseStatement(parseResult, statementMatches, cParsePosition))
        break;
    }

    //// Consolidate parse matches
    consolidateStatementMatches(parseResult, statementMatches.begin(), statementMatches.end(), (uint) 0);

    // Copy to parse result
    parseResult.matchesLength = (uint)statementMatches.size();
    if(parseResult.matchesLength > 0)
    {
      parseResult.globalMatchesLength = (uint)constructStatementMatches[0].size();

      parseResult.matches = new ParseMatch[parseResult.matchesLength];

      uint matchesOffset = 0;
      for(vector< vector<ParseMatch> >::iterator iLevel = constructStatementMatches.begin(); iLevel != constructStatementMatches.end(); ++iLevel)
      {
        uint statementLevelLength = (uint)iLevel->size();

        for(uint c = 0; c < statementLevelLength; ++c)
        {
          // Copy matches
          parseResult.matches[matchesOffset + c] = (*iLevel)[c];

          // Adjust matches' offsets (to the new flat array's addressing)
          if(!isTokenId(parseResult.matches[matchesOffset + c].id))
            parseResult.matches[matchesOffset + c].offset += statementLevelLength + matchesOffset;
        }

        matchesOffset += statementLevelLength;
        OSI_ASSERT(matchesOffset <= parseResult.matchesLength);
      }
    }
  }

  bool Grammar::deepParseStatement(const ParseResult& parseResult, vector<ParseMatch>& statementMatches, uint& cParsePosition)
  {
    //// Parse global statements
    for(vector<OSid>::const_iterator i = globalStatements.begin(); i != globalStatements.end(); ++i)
    {
      //temp:
#     ifdef _DEBUG
        std::string statementName = statementNames[*i];
#     endif

      // Parse using the global statement
      if(deepReduceStatement(parseResult, *i, statementMatches, cParsePosition))
        return true;
    }

    return false;
  }

  bool Grammar::deepReduceStatement(const ParseResult& parseResult, OSid statementId, vector<ParseMatch>& statementMatches, uint& cParsePosition)
  {

    /*
        TODO:
          + (NB) Add multipliers to sequence reduction
    */

    StatementType    type               = statementTypes[statementId];
    const Statement& statement          = *statementIds[statementId];
    const uint       startParsePosition = cParsePosition;
    const uint       nStartMatches      = (uint)statementMatches.size();

    //temp:
#   ifdef _DEBUG
      std::string statementName = statementNames[statementId];
#   endif

    //// Add statement to matches (temporarily)
    // Note: Set the parse match offset to the current offset in the lexical tokens
    //       Later this will be propagated downwards
    statementMatches.push_back(ParseMatch(startParsePosition, 0, statementId));

    //// Attempt to match statement elements
    try
    {
      if(type == SEQUENCE_STATEMENT)
      {
        for(uint cElement = 0; cElement < statement.elementsLength; ++cElement)
        {
          SequenceElement& element          = ((SequenceElement*)statement.elements)[cElement];
          Statement*       elementStatement = null;

          // Test whether element is a statement or a token
          try
          {
            elementStatement = statementIds[element.id];
          }
          catch(...)
          {
            elementStatement = null;
          }

          // If element is a statement, reduce the statement
          if(elementStatement)
          {
            switch(element.multiplier)
            {
            case SINGLE_REQUIRED:
              if(!deepReduceStatement(parseResult, element.id, statementMatches, cParsePosition))
                throw 0; // Could not reduce statement
              break;

            case SINGLE_OPTIONAL:
              deepReduceStatement(parseResult, element.id, statementMatches, cParsePosition);
              break;

            case MULTI_REQUIRED:
              if(!deepReduceStatement(parseResult, element.id, statementMatches, cParsePosition))
                throw 0; // Could not reduce statement

              while(deepReduceStatement(parseResult, element.id, statementMatches, cParsePosition));
              break;

            case MULTI_OPTIONAL:
              while(deepReduceStatement(parseResult, element.id, statementMatches, cParsePosition));
              break;

            default:
              OSI_ASSERT(false); // Post-condition: test for data corruption
            }
          }

          // Else if element is a token, match the token
          else
          {
            if(cParsePosition >= parseResult.tokenMatchesLength)
              throw 0; // End of token matches found

#           ifdef _DEBUG
              string elementTokenName = getTokenName(element.id);
#           endif

            switch(element.multiplier)
            {
            case SINGLE_REQUIRED:
              {
                ParseMatch& tokenMatch = parseResult.tokenMatches[cParsePosition];

#               ifdef _DEBUG
                  string tokenMatchName = getTokenName(tokenMatch.id);
#               endif

                if(element.id == tokenMatch.id)
                {
                  // token match
                  statementMatches.push_back(tokenMatch);
                  ++cParsePosition;
                }
                else
                  throw 0; // no token match

                break;
              }

            case SINGLE_OPTIONAL:
              {
                ParseMatch& tokenMatch = parseResult.tokenMatches[cParsePosition];

#               ifdef _DEBUG
                  string tokenMatchName = getTokenName(tokenMatch.id);
#               endif

                if(element.id == tokenMatch.id)
                {
                  // token match
                  statementMatches.push_back(tokenMatch);
                  ++cParsePosition;
                }

                break;
              }

            case MULTI_REQUIRED:
              {
                bool result = false;

                while(true)
                {
                  ParseMatch& tokenMatch = parseResult.tokenMatches[cParsePosition];

#                 ifdef _DEBUG
                    string tokenMatchName = getTokenName(tokenMatch.id);
#                 endif

                  if(element.id == tokenMatch.id)
                  {
                    // token match
                    result |= true;
                    statementMatches.push_back(tokenMatch);
                    ++cParsePosition;
                  }
                }

                if(!result)
                  throw 0; // no token match

                break;
              }

            case MULTI_OPTIONAL:
              {
                while(true)
                {
                  ParseMatch& tokenMatch = parseResult.tokenMatches[cParsePosition];

#                 ifdef _DEBUG
                    string tokenMatchName = getTokenName(tokenMatch.id);
#                 endif

                  if(element.id == tokenMatch.id)
                  {
                    // token match
                    statementMatches.push_back(tokenMatch);
                    ++cParsePosition;
                  }
                }

                break;
              }

            default:
              OSI_ASSERT(false); // Post-condition: test for data corruption
            }
          }
        }
      }
      else // type == SWITCH_STATEMENT
      {
        bool match = false;

        for(uint cElement = 0; cElement < statement.elementsLength; ++cElement)
        {
          StatementElement& element          = (StatementElement&)statement.elements[cElement];
          Statement*        elementStatement = null;

          // Test whether element is a statement or a token
          try
          {
            elementStatement = statementIds[element.id];
          }
          catch(...)
          {
            elementStatement = null;
          }

          // If element is a statement, reduce the statement
          if(elementStatement)
          {
            if(deepReduceStatement(parseResult, element.id, statementMatches, cParsePosition))
            {
              match = true;
              break; // Switch statement evaluated successfully
            }
            else
              continue; // Could not reduce statement try the next one
          }

          // Else if element is a token, match the token
          else
          {
            if(cParsePosition >= parseResult.tokenMatchesLength)
              throw 0; // End of token matches found

            ParseMatch& tokenMatch = parseResult.tokenMatches[cParsePosition];

            if(element.id == tokenMatch.id)
            {
              // token match
              statementMatches.push_back(tokenMatch);
              ++cParsePosition;

              match = true;
              break;
            }
            else
              continue; // no token match
          }
        }

        if(!match)
          throw 0; // No match found in switch
      }
    }
    catch(...)
    {
      statementMatches.erase(statementMatches.begin() + nStartMatches, statementMatches.end()); // Reset matches
      OSI_ASSERT(statementMatches.size() == nStartMatches);

      cParsePosition = startParsePosition; // Reset parse position
      return false;
    }

    //// Set statement length
    // Note: Set the match length to the length of the statemenet in terms of the number of parse matches
    //       Later this will be changed to the number of direct children
    ParseMatch& parseMatch = statementMatches[nStartMatches];
    parseMatch.length      = (uint16)(statementMatches.size() - nStartMatches); // Note: length is inclusive (it includes the statement's own id)
    OSI_ASSERT(nStartMatches + parseMatch.length == statementMatches.size()); // Post-condition: (Prevent overflow)
    return true;
  }

  void Grammar::consolidateStatementMatches(ParseResult& parseResult, const vector<ParseMatch>::iterator& beginParseMatches, const vector<ParseMatch>::iterator& endParseMatches, uint statementLevel)
  {
    // Test whether the max level has been reached
    if(statementLevel == constructStatementMatches.size())
      constructStatementMatches.resize(statementLevel + 1);
    else
      OSI_ASSERT(statementLevel < constructMatches.size());

    for(vector<ParseMatch>::iterator i = beginParseMatches; i != endParseMatches;)
    {
      // Add to this statement's matches
      constructStatementMatches[statementLevel].push_back(*i);
      vector<ParseMatch>::reverse_iterator iNew = constructStatementMatches[statementLevel].rbegin();

      // Determine whether parse match is a statement or a token
      if(!isTokenId(i->id))
      {
        // Recalculate statement offset to refer to the next statement level
        if(statementLevel == constructStatementMatches.size() - 1)
          iNew->offset = 0;
        else
          iNew->offset = (uint16)constructStatementMatches[statementLevel+1].size();

        // Add children to the next level
        OSI_ASSERT(i->length > 0);
        consolidateStatementMatches(parseResult, i + 1, i + i->length, statementLevel + 1);

        // Recalculate statement length to refer to the next statement level
        iNew->length = (uint16)constructStatementMatches[statementLevel+1].size() - iNew->offset;

        // Step over statement
        i += i->length;
      }
      else
      {
        // Step over token
        ++i;
      }
    }
  }

  const string& Grammar::getTokenName(OSid tokenId) const
  {
    static const string tokenNotFound("[Token Not Found]");
    static const string unknown("[unknown]");

    if(tokenId == ID_UNKNOWN_WORD_TOKEN)
      return unknown;

    try
    {
      const TokenNameSet::index<OSid>::type& IdIndex    = allTokenNames.get<OSid>();
      const TokenNameSet::index<OSid>::type::iterator i = IdIndex.find(tokenId);
      return i->name;
    }
    catch(...)
    {
      return tokenNotFound;
    }
  }

  const string& Grammar::getStatementName(OSid statementId) const
  {
    static const string statementNotFound("[Statement Not Found]");

    try
    {
      return statementNames.find(statementId)->second;
    }
    catch(...)
    {
      return statementNotFound;
    }
  }

#ifdef _DEBUG
  void Grammar::debugOutputTokens() const
  {
    const const_cstring tokenTypeHeadings[] = { "Raw Tokens", "Nil Tokens", "Lex Tokens" };

    cout << endl
         << "Tokens" << endl
         << "------" << endl;

    for(uint cTokenType = 0; cTokenType < 3; ++cTokenType)
    {
      // Output token type heading
      cout << endl << tokenTypeHeadings[cTokenType] << ':' << endl;

      // Output tokens
      const TokenNameSet& tokenNames = Grammar::tokenNames[cTokenType];

      for(TokenNameSet::iterator i = tokenNames.begin(); i != tokenNames.end(); ++i)
        cout << i->name << endl;
    }
  }

  void Grammar::debugOutputStatements() const
  {
    cout << endl
         << "Statements" << endl
         << "----------" << endl;
  }

  void Grammar::outputStatementMatch(ParseResult& result, uint c) const
  {
    ParseMatch& match = result.matches[c];
    if(isTokenId(match.id))
      cout << getTokenName(match.id);
    else
    {
      cout << getStatementName(match.id) << " { ";

      for(uint c = match.offset; c < (uint)(match.offset + match.length); ++c)
      {
        outputStatementMatch(result, c);
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
    for(uint c = 0; c < result.tokenMatchesLength; ++c)
      cout << getTokenName(result.tokenMatches[c].id) << ' ';

    cout << endl << endl;

    // Output global statements
    cout << "Statements:" << endl;
    for(uint c = 0; c < result.globalMatchesLength; ++c)
    {
      outputStatementMatch(result, c);
      cout << endl << endl;
    }
  }
#endif

}

#endif
#endif
