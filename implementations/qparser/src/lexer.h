#ifndef __QPARSER_LEXER_H__
#define __QPARSER_LEXER_H__
//////////////////////////////////////////////////////////////////////////////
//
//    LEXER.H
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      QParser's simple lexer

    TODO:
      + The lexer should eventually be implemented in a separate library,
        but we will just separate it in this class for the time being.
*/

/*                                  CLASSES                                 */
namespace QParser
{
  class Lexer : public Base::Object
  {
  public:
    // Types of lexical tokens recognized by the lexer
    enum TokenType 
    {
      TOKENTYPE_RAW        = 0,
      TOKENTYPE_NIL        = 1,
      TOKENTYPE_LEX        = 2,
      TOKENTYPE_LEX_SYMBOL = 2,
      TOKENTYPE_LEX_WORD   = 3
    };
    
    // Construction
    INLINE Lexer(TokenRegistry& tokenRegistry);
    
    // Add a string token to the lexer definition
    INLINE ParseToken StringToken(const_cstring tokenName, const_cstring value);
    
    // Add a single character token to the lexer definition
    INLINE ParseToken CharToken(const_cstring tokenName, char value);
    
    // Add a bounded token (With start and end characters) to the lexer definition
    INLINE ParseToken BoundedToken(const_cstring tokenName, const_cstring leftBoundingValue, const_cstring rightBoundingValue, OSIX::PARSER_BOUNDED_LINETYPE lineType);
    
    // Build the lexer definition so that it can be used for lexical analysis
    INLINE void Build(TokenType tokenType);
    
    // Perform the lexical analysis on the parser input (inputs a character stream 
    // and produces a lex stream)
    INLINE void LexicalAnalysis(ParseResult& parseResult);
    
  protected:
    TokenRegistry& tokenRegistry; // A reference to the token registry used by both the lexer and the parser
    
    // Special characters used in lexer token definitions
    static const char SPECIAL_SINGLELINE_BOUNDING_CHAR;
    static const char SPECIAL_MULTILINE_BOUNDING_CHAR;
    
    std::set<ParseToken> lexTokens; // The set of tokens already contained in the lexer
    
    // Lex match structure contains a token and an index to the corresponding
    // characters in some character stream
    struct LexMatch
    {
      ParseToken token;   // The lexical token
      uint16 valueOffset; // Offset into the character stream
      uint8  valueLength; // Length of the matched characters

      INLINE LexMatch(ParseToken token, uint16 valueOffset, uint8 valueLength) : token(token), valueOffset(valueOffset), valueLength(valueLength) {}
      INLINE LexMatch(const LexMatch& match) : token(match.token), valueOffset(match.valueOffset), valueLength(match.valueLength) {}
      INLINE LexMatch() {}
    };

    // A list of all string used for matching the tokens in the alphabet
    // The LexMatch instances contain indices into the tokenCharacters array.
    union
    {
      LexMatch* tokens[4];          // Tokens of all types
      struct
      {
        LexMatch* rawTokens;        // "Raw" tokens
        LexMatch* nilTokens;        // "Nil" tokens
        LexMatch* lexSymbolTokens;  // "Lex symbols"
        LexMatch* lexWordTokens;    // "Lex words"
      };
    };
    uint nTokens[4]; // Number of tokens of each type
    std::vector<char> tokenCharacters; // Concatenation of all lexical token characters

    // Token root indexing tables
    // (Provides fast recognition of tokens by matching the first (root) character
    // of the token)
    struct TokenRootIndex
    {
      uint8 offset; // Offset of the first token in the token liste
      uint8 length; // Number of tokens corresponding to the token root character
    };

    union
    {
      TokenRootIndex tokenRootIndices[4][256];      // All token root indices
      struct
      {
        TokenRootIndex rawTokenRootIndices[256];        // Token root indices corresponding to "raw" token types
        TokenRootIndex nilTokenRootIndices[256];        // Token root indices corresponding to "nil" token types
        TokenRootIndex lexSymbolTokenRootIndices[256];  // Token root indices corresponding to "lex symbols"
        TokenRootIndex lexWordTokenRootIndices[256];    // Token root indices corresponding to "lex words"
      };
    };

    // Data structures used during construction of the lexer
    typedef std::vector<LexMatch> TokenConstructionSet; // An array of lex matches used during construction
    TokenConstructionSet constructionTokens;            // Tokens array used during construction (Indexed by token value)
    
    std::vector<ParseMatch> constructMatches; // construction set for parse matches
    
    // Add a lex token to the lexer if it does not already exists
    INLINE void AddLexToken(ParseToken token, uint bufferLength, uint valueLength);
    
    // 
    INLINE bool ParseSymbolToken(TokenType tokenType, const_cstring inputPosition, uint inputLength, ParseMatch& tokenMatch) const;
    
    //
    INLINE bool MatchSymbolToken(const LexMatch& token, const_cstring inputPosition, uint length, uint16& matchLength) const;
    
    //
    INLINE bool MatchBoundingToken(const LexMatch& token, const_cstring inputPosition, uint length, uint16& matchLength) const;

    //
    INLINE void ParseWordToken(const_cstring inputPosition, ParseMatch& tokenMatch) const;
    
    //
    INLINE bool MatchWordToken(const LexMatch& token, const_cstring inputPosition) const;
  };
}

/*                                   INCLUDES                               */
#include "lexer.inl"

#endif
