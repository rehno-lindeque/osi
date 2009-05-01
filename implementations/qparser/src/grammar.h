  #ifndef __QPARSER_GRAMMAR_H__
#define __QPARSER_GRAMMAR_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMAR.H
//
//    Copyright © 2007-2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      QParser grammar base.

    USAGE:
      + Derive specific grammar implementations from this class. (e.g. for
        LL / LR / LARL class grammars etc.)

    IMPLEMENTATION:
      + Root tables (e.g. lexRootIndices, nilTokensRootIndices, etc) list
        indexes into their respective token parse table. An index of 255
        indicates no token.
      + Token ids are sequenced rather than hashed. Tokens and statements
        share the same id space, hence a token can't have the same id as a
        statement.
 
    OLD: This is no longer true! Have a look at the tokens.h header
        we'll have to re-factor this system...
        + Even tokens are nonterminals (production ids)
        + Odd tokens are terminal tokens (lexical ids)
          + -1 is a special token, sometimes used to represent no token or
            end-of-stream token in subclasses (specific to the grammar
            implementation)
          + 1 is an identifier token used for both declerations and
            references (during lexical analysis and parsing stage)
          + 3 is an identifier declaration (psuedo-)token used in production symbols
            during the grammar construction phase
          + 5 is an identifier reference (psuedo-)token used in production symbols
            during the grammar construction phase
          + 7 is a numeric constant token. representing integer or
            real values

    TODO:
      + Implement precedence mechanism for shift-reduce collision resolution
      + Add "multi-identifier" merging extension
      + The lexer implementation should be separated from the parser!
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
  class Grammar : public Base::Object
  {
  public:
    // Constructor
    INLINE Grammar();

    //Destructor
    virtual ~Grammar();

    // Productions
    INLINE ParseToken BeginProduction(const_cstring productionName);
    INLINE void EndProduction();

    INLINE void ProductionToken(ParseToken token);
    INLINE ParseToken ProductionToken(const_cstring tokenName);
    INLINE ParseToken ProductionIdentifierDecl(const_cstring typeName);
    //INLINE void ProductionIdentifierRef(OSid type);
    INLINE ParseToken ProductionIdentifierRef(const_cstring typeName);

    INLINE ParseToken DeclareProduction(const_cstring productionName);

    INLINE void Precedence(const_cstring token1Name, const_cstring token2Name);
    INLINE void Precedence(ParseToken token1, ParseToken token2);

    INLINE void GrammarStartSymbol(ParseToken nonterminal);

    INLINE bool CheckForwardDeclarations() const;

    virtual void ConstructProductions() = 0;
    
    // Tokens
    INLINE TokenRegistry& GetTokenRegistry() { return tokenRegistry; }
    INLINE const TokenRegistry& GetTokenRegistry() const { return tokenRegistry; }
    
    // Lexing
    INLINE Lexer& GetLexer() { return lexer; }
    INLINE const Lexer& GetLexer() const { return lexer; }

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

    INLINE void SetErrorStream(FILE* stream);
    INLINE void SetWarningStream(FILE* stream);
    INLINE void SetInfoStream(FILE* stream);

  protected:

    TokenRegistry tokenRegistry;  // A registry of the tokens used by both the parser and the lexer
    Lexer lexer;                  // The lexer used to tokenize the incoming stream of characters
    
    // Streams for parser messages to be output
    std::ostream errorStream;
    std::ostream warnStream;
    std::ostream infoStream;

    //// Productions (non-terminals)
    struct Production
    {
      // Symbols
      struct Symbol
      {
        /*union
        {
          bool isTerminal : 1;
          OSid id;
        };*/
        ParseToken token;
        //uint32 param;

        FORCE_INLINE Symbol() {}
        //FORCE_INLINE Symbol(ParseToken token) : token(token) , param(0) {}
        FORCE_INLINE Symbol(ParseToken token) : token(token) {}
        //FORCE_INLINE Symbol(ParseToken token, uint32 param) : token(token), param(param) {}
      };

      Symbol* symbols;
      uint8 symbolsLength;

      FORCE_INLINE Production() : symbols(null), symbolsLength(0) {}
    };

    // A production set is the set of productions producing the same symbol (id)
    struct ProductionSet
    {
      uint productionsOffset;
      uint8 productionsLength;

      // LL(1) attributes
      bool nullable;
      uint8 visitedCount;
      //OSid *firstSet;
      //uint8 firstSetLength;
      //OSid *followSet;
      //uint8 followSetLength;
      //vector<Production::Symbol> followSet;

      FORCE_INLINE ProductionSet() : productionsOffset(0), productionsLength(0), nullable(false), visitedCount(0) /*, firstSet(null), firstSetLength(0), followSet(null), followSetLength(0)*/ {}
    };

    Production*                     activeProduction;
    std::vector<Production::Symbol> activeProductionSymbols;

    std::multimap<ParseToken, ParseToken> precedenceMap;
    std::set<ParseToken> silentTerminals;

    ParseToken startSymbol;

    ////// Parsing
    std::vector< std::vector<ParseMatch> > constructStatementMatches;    
    INLINE void ReshuffleResult(ParseResult& parseResult);

    //// Grammar construction operations
    std::map<ParseToken, ProductionSet*>             productionSets; // todo: rename to something like ProductionUnions / ProductionGroups
    std::vector< std::pair<Production, ParseToken> > productions;

    // Construct a terminal token (and add it to the list of tokens being processed)
    //INLINE ParseToken ConstructTerminal(const_cstring tokenName, uint bufferLength, uint valueLength);
    
    // Construct a non-terminal token
    INLINE ParseToken ConstructNonterminal(const_cstring tokenName);
    
    // Replace all tokens 
    INLINE void ReplaceAllTokens(ParseToken oldToken, ParseToken newToken);

    // Test whether a production is silent
    //INLINE bool isSilent(const ProductionSet& productionSet) const;
    INLINE bool IsSilent(const Production& production) const;

    // Test whether a lexical token is silent
    INLINE bool IsSilent(ParseToken token) const;
    
    // Find a precedence directive for token1 < token2
    std::multimap<ParseToken, ParseToken>::const_iterator FindPrecedenceDirective(ParseToken token1, ParseToken token2) const;

    //// Accessors
    INLINE const ProductionSet* GetProductionSet(ParseToken nonterminal) const;
    INLINE       ProductionSet* GetProductionSet(ParseToken nonterminal);
    
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
#include "grammar.inl"

#endif
