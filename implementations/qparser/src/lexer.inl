#ifdef  __QPARSER_LEXER_H__
#ifndef __QPARSER_LEXER_INL__
#define __QPARSER_LEXER_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    LEXER.INL
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QParser
{
  const char Lexer::SPECIAL_SINGLELINE_BOUNDING_CHAR = '\0';
  const char Lexer::SPECIAL_MULTILINE_BOUNDING_CHAR = '\1';
  
  INLINE Lexer::Lexer(TokenRegistry& tokenRegistry) : tokenRegistry(tokenRegistry)
  {
    memset(tokenRootIndices, 0, sizeof(tokenRootIndices)); 
  }
  
  ParseToken Lexer::StringToken(const_cstring tokenName, const_cstring value)
  {
    // Initialize variables
    const uint bufferLength = (uint)tokenCharacters.size();
    const uint valueLength  = (uint)strlen(value) + 1;

    // Preconditions
    OSI_ASSERT(valueLength > 0);

    // Concatenate token value to token (character) storage buffer (Note: null character is also concatenated)
    tokenCharacters.resize(bufferLength + valueLength);
    for(uint c = 0; c < valueLength; ++c)
      tokenCharacters[bufferLength + c] = value[c];

    // Create the terminal token and add it to all the relevant indexes
    ParseToken token = tokenRegistry.GenerateTerminal(tokenName);
    AddLexToken(token, bufferLength, valueLength);
    return token;
  }

  ParseToken Lexer::CharToken(const_cstring tokenName, char value)
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
    ParseToken token = tokenRegistry.GenerateTerminal(tokenName);
    AddLexToken(token, bufferLength, 2);
    return token;
  }

  ParseToken Lexer::BoundedToken(const_cstring tokenName, const_cstring leftBoundingValue, const_cstring rightBoundingValue, OSIX::PARSER_BOUNDED_LINETYPE lineType)
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
    ParseToken token = tokenRegistry.GenerateTerminal(tokenName);
    AddLexToken(token, bufferLength, 1 + leftValueLength + rightValueLength);
    return token;
  }
  
  INLINE void Lexer::Build(TokenType tokenType)
  {
    uint&             nTokens                      = Lexer::nTokens[tokenType];
    LexMatch*&        activeTokens                 = tokens[tokenType];
    TokenRootIndex(&  activeTokenRootIndices)[256] = tokenRootIndices[tokenType];

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
  }
  
  INLINE void Lexer::AddLexToken(ParseToken token, uint bufferLength, uint valueLength)
  {
    // Test whether the token already exists
    if(lexTokens.find(token) != lexTokens.end())
      return;

    // Add the token to the lexer
    lexTokens.insert(token);
    constructionTokens.push_back(LexMatch(token, bufferLength, valueLength));
  }
  
  INLINE void Lexer::LexicalAnalysis(ParseResult& parseResult)
  {
    std::vector<ParseMatch>& tokenMatches = constructMatches;
    const_cstring parsePosition        = parseResult.inputStream.data;
    const_cstring lexWordStartPosition = parseResult.inputStream.data;

    while(parsePosition < &parseResult.inputStream.data[parseResult.inputStream.length])
    {
      //bug: const uint remainingLength  = (uint)(&parseResult.inputData[parseResult.inputLength] - parsePosition);
      const uint remainingLength  = (uint)(&parseResult.inputStream.data[parseResult.inputStream.length] - parsePosition + 1);

      // Match raw token, nil token or lex symbol token (symbolic tokens that do not need to be seperated, such as operators)
      ParseMatch tokenSymbolMatch;
      tokenSymbolMatch.offset = (uint)(parsePosition - parseResult.inputStream.data);

      if(ParseSymbolToken(TOKENTYPE_RAW, parsePosition, remainingLength, tokenSymbolMatch))
      {
        // Parse all unparsed characters into lex word
        if(lexWordStartPosition != parsePosition)
        {
          ParseMatch tokenWordMatch;
          tokenWordMatch.offset = (uint16)(lexWordStartPosition - parseResult.inputStream.data);
          tokenWordMatch.length = (uint8)(parsePosition - lexWordStartPosition);

          // Parse word token
          ParseWordToken(lexWordStartPosition, tokenWordMatch);

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
      else if(ParseSymbolToken(TOKENTYPE_NIL, parsePosition, remainingLength, tokenSymbolMatch))
      {
        // Parse all unparsed characters into lex word
        if(lexWordStartPosition != parsePosition)
        {
          ParseMatch tokenWordMatch;
          tokenWordMatch.offset = (uint16)(lexWordStartPosition - parseResult.inputStream.data);
          tokenWordMatch.length = (uint8)(parsePosition - lexWordStartPosition);

          // Parse word token
          ParseWordToken(lexWordStartPosition, tokenWordMatch);

          // Add word token to token matches
          tokenMatches.push_back(tokenWordMatch);
        }

        // Ignore nil tokens... (don't add to token matches)

        // Go to next parse position
        parsePosition += tokenSymbolMatch.length;

        // Reset word start position
        lexWordStartPosition = parsePosition;
      }
      else if(ParseSymbolToken(TOKENTYPE_LEX, parsePosition, remainingLength, tokenSymbolMatch))
      {
        // Parse all unparsed characters into lex word
        if(lexWordStartPosition != parsePosition)
        {
          ParseMatch tokenWordMatch;
          tokenWordMatch.offset = (uint16)(lexWordStartPosition - parseResult.inputStream.data);
          tokenWordMatch.length = (uint8)(parsePosition - lexWordStartPosition);

          // Parse word token
          ParseWordToken(lexWordStartPosition, tokenWordMatch);

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
      ParseWordToken(lexWordStartPosition, tokenWordMatch);

      // Add word token to token matches
      tokenMatches.push_back(tokenWordMatch);
    }

    parseResult.lexStream.length = (uint)tokenMatches.size();
    parseResult.lexStream.data = new ParseMatch[parseResult.lexStream.length];
    memcpy(parseResult.lexStream.data, &tokenMatches[0], sizeof(ParseMatch)*parseResult.lexStream.length);
  }

  INLINE bool Lexer::ParseSymbolToken(TokenType tokenType, const_cstring inputPosition, uint inputLength, ParseMatch& tokenMatch) const
  {
    const TokenRootIndex* const& tokenRootIndices = Lexer::tokenRootIndices[tokenType];
    LexMatch* const&  tokens = Lexer::tokens[tokenType];
    const char rootCharacter = *inputPosition;

    // Lookup token root character
    const TokenRootIndex& tokenRootIndex = tokenRootIndices[int(rootCharacter)];

    // Parse possible characters
    for(uint cToken = 0; cToken < tokenRootIndex.length; ++cToken)
    {
      const LexMatch& token = tokens[tokenRootIndex.offset + cToken];

      if(MatchSymbolToken(token, inputPosition, inputLength, tokenMatch.length))
      {
        tokenMatch.token = token.token;
        return true; // token match found
      }
    }

    return false; // no token match found
  }

  INLINE bool Lexer::MatchSymbolToken(const LexMatch& token, const_cstring inputPosition, uint inputLength, uint16& matchLength) const
  {
    // Precondition:
    OSI_ASSERT(token.valueLength >= 1);

    if(tokenCharacters[token.valueOffset] == SPECIAL_SINGLELINE_BOUNDING_CHAR || tokenCharacters[token.valueOffset] == SPECIAL_MULTILINE_BOUNDING_CHAR)
      return MatchBoundingToken(token, inputPosition, inputLength, matchLength);

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

  INLINE bool Lexer::MatchBoundingToken(const LexMatch& token, const_cstring inputPosition, uint inputLength, uint16& matchLength) const
  {
    //todo: refactor a little? (store lengths of boundary strings during matches)
    const_cstring const tokenValue = &tokenCharacters[token.valueOffset];
    const_cstring tokenValuePosition = &tokenValue[1];

    // Match starting boundary
    uint cInput = 0; // input counter

    while(true)
    {
      // Test whether remaining characters can contain token boundary strings
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
    if((tokenValuePosition[0] == PARSER_TOKEN_VALUE_EOF[0] // todo: remove the EOF concept in favour of \0??
        || tokenValuePosition[0] == '\0'))
    {
      while(cInput < inputLength)
      {
        if(tokenCharacters[token.valueOffset] == SPECIAL_SINGLELINE_BOUNDING_CHAR)
        {
          if( inputPosition[cInput] == '\n')
            break;
        }
        else // todo: perhaps make multi-line empty boundary illegal???
        {
          OSI_ASSERT(tokenCharacters[token.valueOffset] == SPECIAL_MULTILINE_BOUNDING_CHAR);
        }

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

  INLINE void Lexer::ParseWordToken(const_cstring inputPosition, ParseMatch& tokenMatch) const
  {
    const TokenRootIndex* const& tokenRootIndices = lexWordTokenRootIndices;
    LexMatch* const& tokens = lexWordTokens;
    const char rootCharacter = *inputPosition;

    // Lookup token root character
    const TokenRootIndex& tokenRootIndex = tokenRootIndices[int(rootCharacter)];

    // Parse possible characters
    for(uint cToken = 0; cToken < tokenRootIndex.length; ++cToken)
    {
      const LexMatch& token = tokens[tokenRootIndex.offset + cToken];

      // Use input length to quickly discard token words
      if(token.valueLength - 1 != tokenMatch.length)
        continue;

      // Match token
      if(MatchWordToken(token, inputPosition))
      {
        tokenMatch.token = token.token;
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
      tokenMatch.token = TOKEN_TERMINAL_LITERAL;
    else
      tokenMatch.token = TOKEN_TERMINAL_IDENTIFIER;
  }

  INLINE bool Lexer::MatchWordToken(const LexMatch& token, const_cstring inputPosition) const
  {
    for(uint c = 0; c < (uint)(token.valueLength - 1); ++c)
      /*if(token.value[c] != inputPosition[c])*/
      if(tokenCharacters[token.valueOffset + c] != inputPosition[c])
        return false;

    return true;
  }
}

#endif
#endif
