#ifndef __QPARSER_PARSER_H__
#define __QPARSER_PARSER_H__
//////////////////////////////////////////////////////////////////////////////
//
//    PARSER.H
//
//    Copyright © 2007-2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      QParser parser base.
*/

/*                              COMPILER MACROS                             */
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4311)
# pragma warning(disable:4312)
#endif

/*                                  CLASSES                                 */
namespace QParser
{
  class ParserImplementation : public Base::Object
  {
  public:
    // Constructor
    INLINE ParserImplementation();

    //Destructor
    virtual ~ParserImplementation();

    // Construct the parser implementation using the given grammar definition
    virtual void ConstructParser(Grammar* grammar) = 0;
    
    // Tokens
    INLINE TokenRegistry& GetTokenRegistry() { return tokenRegistry; }
    INLINE const TokenRegistry& GetTokenRegistry() const { return tokenRegistry; }
    
    // Lexing
    //INLINE Lexer& GetLexer() { return lexer; }
    //INLINE const Lexer& GetLexer() const { return lexer; }

    // Parsing
    virtual void Parse(ParseResult& parseResult) = 0;

    INLINE void ParseFile(const_cstring fileName, ParseResult& parseResult);
    INLINE void ParseString(const_cstring stringBuffer, ParseResult& parseResult);

/*#ifdef _DEBUG
    void DebugOutputTokens() const;
    void DebugOutputProductions() const;
    //virtual void DebugOutputStates() const = 0;
    virtual void DebugOutputStates() const {};
    virtual void DebugOutputTable() const {};
    void DebugOutputParseResult(OSobject& parseResult) const;
#endif*/

    // Message streams
    INLINE void SetErrorStream(FILE* stream);
    INLINE void SetWarningStream(FILE* stream);
    INLINE void SetInfoStream(FILE* stream);

  protected:
    TokenRegistry tokenRegistry;  // A registry of the tokens used by both the parser and the lexer
    //Lexer lexer;                  // The lexer used to tokenize the incoming stream of characters
    
    // Streams for parser messages to be output
    std::ostream errorStream;
    std::ostream warnStream;
    std::ostream infoStream;

    ////// Parsing
    std::vector< std::vector<ParseMatch> > constructStatementMatches;    
    //INLINE void ReshuffleResult(ParseResult& parseResult);
    
    //// Miscelaneous    
/*#ifdef _DEBUG
    //void OutputStatementMatch(ParseResult& result, uint& index) const;
    void OutputStatementMatch(ParseResult& result, uint index) const;
    
    INLINE void DebugOutputProduction(ParseToken token, const Production& production) const;
    INLINE void DebugOutputProduction(const Production& production) const;
    INLINE void DebugOutputSymbol(ParseToken token) const;
#endif*/
  };
}

/*                                   INCLUDES                               */
#include "parser.inl"

#endif
