#ifndef __QPARSER_GRAMMAR_H__
#define __QPARSER_GRAMMAR_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMAR.H
//
//    Copyright © 2007-2008, Rehno Lindeque. All rights reserved.
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
*/

/*                              COMPILER MACROS                             */
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4311)
# pragma warning(disable:4312)
#endif

#ifdef _MSC_VER
# define STDEXT_NAMESPACE stdext
#else
# define STDEXT_NAMESPACE __gnu_cxx
#endif

/*                                   INCLUDES                               */
// STL
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#ifdef _MSC_VER
  #include <hash_map>
  #include <hash_set>
  //#include <ext/stdio_filebuf.h>?
#else
  #include <ext/hash_map>
  #include <ext/hash_set>
  #include <ext/stdio_filebuf.h>
#endif
//#include <unordered_map>
//#include <unordered_set>

using std::string;
using std::vector;
using std::stack;
using std::set;
using std::map;
using std::multimap;
using std::ifstream;
using std::ios_base;
using std::ostream;
using std::pair;
using std::cout;
using std::clog;
using std::cerr;
using std::endl;
using STDEXT_NAMESPACE::hash;
using STDEXT_NAMESPACE::hash_map;
using STDEXT_NAMESPACE::hash_set;
using STDEXT_NAMESPACE::stdio_filebuf;

// CLib
#include <string.h>

namespace QParser
{
  struct ParseMatch : public OSIX::ParseMatch
  {
    FORCE_INLINE ParseMatch(uint16 offset, uint16 length, OSid id) { ParseMatch::offset = offset; ParseMatch::length = length; ParseMatch::id = id; }
    INLINE ParseMatch() {}
  };
  
/*                                  CLASSES                                 */
  class Grammar : public Base::Object
  {
  public:

    enum TerminalId
    {
      ID_SPECIAL         = -1,
      ID_IDENTIFIER      = 1,
      ID_IDENTIFIER_DECL = 3,
      ID_IDENTIFIER_REF  = 5,
      ID_CONST_NUM       = 7,
      ID_FIRST_TERMINAL  = 9
    };

    enum TokenType
    {
      RAW_TOKEN = 0,
      NIL_TOKEN = 1,
      LEX_TOKEN = 2
    };

    enum SubTokenType
    {
      LEX_SYMBOL = 0,
      LEX_WORD = 1
    };

    // Constructor
    INLINE Grammar();

    //Destructor
    virtual ~Grammar();

    // Token construction
    TokenType activeTokenType;
    SubTokenType activeSubTokenType;

    INLINE OSid stringToken(const_cstring tokenName, const_cstring value);
    INLINE OSid charToken(const_cstring tokenName, char value);
    INLINE OSid boundedToken(const_cstring tokenName, const_cstring leftBoundingValue, const_cstring rightBoundingValue, OSIX::PARSER_BOUNDED_LINETYPE lineType);
    INLINE void constructTokens();

    // Productions
    INLINE OSid beginProduction(const_cstring productionName);
    INLINE void endProduction();

    INLINE void productionToken(OSid token);
    INLINE OSid productionToken(const_cstring tokenName);
    INLINE OSid productionIdentifierDecl(const_cstring typeName);
    INLINE void productionIdentifierRef(OSid type);
    INLINE OSid productionIdentifierRef(const_cstring typeName);

    INLINE OSid declareProduction(const_cstring productionName);

    INLINE void precedence(const_cstring token1Name, const_cstring token2Name);
    INLINE void precedence(OSid token1, OSid token2);

    INLINE void grammarStartSymbol(OSid nonterminal);

    INLINE bool checkForwardDeclarations() const;

    virtual void constructProductions() = 0;

    // Parsing
    class ParseResult : public Base::Object
    {
    public:
      // Input data
      struct
      {
        uint          length;
        uint          elementSize;
        const_cstring data;
      } inputStream;

      // Parse matches
      struct
      {
        uint        length;
        uint        elementSize;
        ParseMatch* data;
      } parseStream;
      uint globalMatchesLength;

      // Lexical token matches
      struct
      {
        uint        length;
        uint        elementSize;
        ParseMatch* data;
      } lexStream;

      virtual ~ParseResult() { delete[] parseStream.data; delete[] lexStream.data; }
    };

    virtual void parse(ParseResult& parseResult) = 0;

    INLINE void parseFile(const_cstring fileName, ParseResult& parseResult);
    INLINE void parseString(const_cstring stringBuffer, ParseResult& parseResult);
    
    INLINE const string& getTokenName(OSid tokenId) const;

#ifdef _DEBUG
    void debugOutputTokens() const;
    void debugOutputProductions() const;
    //virtual void debugOutputStates() const = 0;
    virtual void debugOutputStates() const {};
    virtual void debugOutputTable() const {};
    void debugOutputParseResult(OSobject& parseResult) const;
#endif

    INLINE void setErrorStream(FILE* stream);
    INLINE void setWarningStream(FILE* stream);
    INLINE void setInfoStream(FILE* stream);

  protected:

    static const char SPECIAL_SINGLELINE_BOUNDING_CHAR;
    static const char SPECIAL_MULTILINE_BOUNDING_CHAR;

    //// Miscelaneous
    ostream errorStream;
    ostream warnStream;
    ostream infoStream;

    ////// Grammar construction
    //// General
    struct StlStringHash
    { INLINE size_t operator()(const string& arg) const { return hash<const char*>()(arg.c_str()); }  };
    
    // todo: The different hash_map implementations are inconsistent. Try to find some standard way for defining one.
    typedef hash_map<const string, OSid, StlStringHash> TokenIds; // Token name -> id
    //typedef hash_map<const_cstring, OSid, hash<const_cstring> > TokenIds; // Token name -> id
    typedef map<OSid, string> TokenNames;                            // Token id -> name
    
    OSid nextTerminal;     // Terminal ids are always odd     (lexical tokens)
    OSid nextNonterminal;  // Nonterminal ids are always even (production tokens)
    OSid nextTemporaryId;  // Temporary ids are always > nextNonterminal (they are eventually replaced by normal nonterminals

    //// Identifiers
    hash_set<const char*> identifierTypes;

    //// Lexical tokens (terminals)
    // Token value sets (parse data)
    struct Token
    {
      OSid id;
      uint16 valueOffset;
      uint8  valueLength;

      INLINE Token(OSid id, uint16 valueOffset, uint8 valueLength) : id(id), valueOffset(valueOffset), valueLength(valueLength) {}
      INLINE Token(const Token& token) : id(token.id), valueOffset(token.valueOffset), valueLength(token.valueLength) {}
      INLINE Token() {}
    };

    vector<char> tokenCharacters; // concatenation of all token characters

    union
    {
      Token* tokens[4];
      struct
      {
        Token* rawTokens;
        Token* nilTokens;
        Token* lexSymbolTokens;
        Token* lexWordTokens;
      };
    };

    uint nTokens[4];

    // Token root indexing tables
    struct TokenRootIndex
    {
      uint8 offset; // offset of the first token in the token liste
      uint8 length; // number of tokens corresponding to the token root character
    };

    union
    {
      TokenRootIndex tokenRootIndices[4][256];
      struct
      {
        TokenRootIndex rawTokenRootIndices[256];
        TokenRootIndex nilTokenRootIndices[256];
        TokenRootIndex lexSymbolTokenRootIndices[256];
        TokenRootIndex lexWordTokenRootIndices[256];
      };
    };

    TokenNames terminalNames;
    TokenIds terminalIds;

    typedef vector<Token> TokenConstructionSet;
    TokenConstructionSet constructionTokens; // tokens array used during construction (Indexed by token value)

    //// Productions (non-terminals)
    struct Production
    {
      // Symbols
      struct Symbol
      {
        union
        {
          bool isTerminal : 1;
          OSid id;
        };
        uint32 param;

        FORCE_INLINE Symbol() {}
        FORCE_INLINE Symbol(OSid id) : id(id) , param(0) {}
        FORCE_INLINE Symbol(OSid id, uint32 param) : id(id), param(param) {}
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

    Production*                activeProduction;
    vector<Production::Symbol> activeProductionSymbols;
    TokenNames nonterminalNames;
    TokenIds nonterminalIds;

    multimap<OSid, OSid> precedenceMap;
    set<OSid> silentTerminals;

    OSid startSymbol;

    ////// Parsing
    vector<ParseMatch> constructMatches; // construction set for parse matches
    vector< vector<ParseMatch> > constructStatementMatches;

    //// Parse operations
    INLINE void lexicalAnalysis(ParseResult& parseResult);
    INLINE bool parseSymbolToken(TokenType tokenType, const_cstring inputPosition, uint inputLength, ParseMatch& tokenMatch) const;
    INLINE bool matchSymbolToken(const Token& token, const_cstring inputPosition, uint length, uint16& matchLength) const;
    INLINE bool matchBoundingToken(const Token& token, const_cstring inputPosition, uint length, uint16& matchLength) const;

    INLINE void parseWordToken(const_cstring inputPosition, ParseMatch& tokenMatch) const;
    INLINE bool matchWordToken(const Token& token, const_cstring inputPosition) const;
    
    INLINE void reshuffleResult(ParseResult& parseResult);

    //// Grammar construction operations
    map<OSid, ProductionSet*>               productionSets; // todo: rename to something like ProductionUnions / ProductionGroups
    vector< pair<Production, OSid> >        productions;

    // Construct an LL(1) linear binary indexed parse table
    INLINE void constructParseTableLL1();

    // Get the next available (odd numbered) terminal id
    INLINE OSid getNextTerminalId();

    // Get the next available (even numbered) nonterminal id
    INLINE OSid getNextNonterminalId();
    
    // Get the next available temporary nonterminal id
    INLINE OSid getTemporaryNonterminalId();
    
    // Construct a terminal token (and add it to the list of tokens being processed)
    INLINE OSid constructTerminal(const_cstring tokenName, uint bufferLength, uint valueLength);
    
    // Construct a non-terminal token
    INLINE OSid constructNonterminal(const_cstring tokenName);
    
    // Replace all tokens 
    INLINE void replaceAllTokens(OSid oldId, OSid newId);

    // Test whether id is a token id (i.e. not a production)
    INLINE bool isTerminal(OSid id) const;

    // Test whether a production is silent
    //INLINE bool isSilent(const ProductionSet& productionSet) const;
    INLINE bool isSilent(const Production& production) const;

    // Test whether a lexical token is silent
    INLINE bool isSilent(OSid id) const;

    //// Accessors
    INLINE const ProductionSet* getProductionSet(OSid nonterminal) const;
    INLINE       ProductionSet* getProductionSet(OSid nonterminal);
    INLINE       OSid getTokenId(const_cstring tokenName) const;

    //// Miscelaneous
    //void outputStatementMatch(ParseResult& result, uint& index) const;
    void outputStatementMatch(ParseResult& result, uint index) const;

#ifdef _DEBUG
    INLINE void debugOutputProduction(OSid id, const Production& production) const;
    INLINE void debugOutputProduction(const Production& production) const;
    INLINE void debugOutputSymbol(OSid symbol) const;
#endif
  };
}

/*                                   INCLUDES                               */
#include "grammar.inl"

#endif
