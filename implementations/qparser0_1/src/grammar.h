#ifndef __QPARSER_GRAMMAR_H__
#define __QPARSER_GRAMMAR_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMAR.H
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Grammar for the MSL parser implementation.

    USAGE:
      + A switch statement cannot form a cyclic statement (currently)

    IMPLEMENTATION:
      + Root tables (e.g. lexRootIndices, nilTokensRootIndices, etc) list
        indexes into their respective token parse table. An index of 255
        indicates no token.
      + Token ids are sequenced rather than hashed. Tokens and statements
        share the same id space, hence a token can't have the same id as a
        statement.

    TODO:
      + Possibly replace Boost multi-index implementation:
        + Added complexity of the multi-index interface makes the code less
          readable
        + The "always consistent" requirement of the library may lead to
          performance deficiencies compared to a run-time equivalent that
          allows temporarily invalid states or "build-on-demand" indexes.
          In fact, this might even suit a state-based
          (begin-end / open-close) API better.
        + Many operations perform unnecessary or redundant indexing operations
          + e.g. the modify method will probably re-insert an element
            even when no keys were modified? (speculative)
      + Suppress boost warning (4503: decorated name length exceeded, name
        was truncated)
*/

/*                                   INCLUDES                               */
// STL
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <fstream>

#ifdef _DEBUG
  #include <iostream>
#endif

using std::string;
using std::vector;
using std::stack;
using std::map;
using std::ifstream;
using std::ios_base;
using std::pair;

#ifdef _DEBUG
  using std::cout;
  using std::endl;
#endif

// Boost
#define BOOST_MULTI_INDEX_DISABLE_SERIALIZATION // Disable serialization: It's not necessary and it's not always available

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

using boost::multi_index::multi_index_container;
using boost::multi_index::indexed_by;
using boost::multi_index::hashed_unique;
using boost::multi_index::ordered_unique;
using boost::multi_index::sequenced;
using boost::multi_index::identity;
using boost::multi_index::member;
using boost::multi_index::const_mem_fun;
using boost::multi_index::tag;
using boost::hash;

namespace QParser
{
  struct ParseMatch : public ::ParseMatch
  {
    FORCE_INLINE ParseMatch(uint16 offset, uint16 length, OSid id) { ParseMatch::offset = offset; ParseMatch::length = length; ParseMatch::id = id; }
    INLINE ParseMatch() {}
  };

/*                                  CLASSES                                 */
  class Grammar : public Base::Object
  {
  public:
    // Constructor
    INLINE Grammar() : nextTokenId(1),
                       nextStatementId(0),
                       global(false),
                       rawTokenNames(tokenNames[RAW_TOKEN]),
                       nilTokenNames(tokenNames[NIL_TOKEN]),
                       lexTokenNames(tokenNames[LEX_TOKEN]),
                       //OLD: sequenceStatements(statements[SEQUENCE_STATEMENT]),
                       //OLD: switchStatements(statements[SWITCH_STATEMENT]),
                       activeSubTokenType((SubTokenType)0)
    { memset(tokenRootIndices, 0, sizeof(tokenRootIndices)); }

    // Token construction
    enum TokenType
    {
      RAW_TOKEN = 0,
      NIL_TOKEN = 1,
      LEX_TOKEN = 2,
    };

    enum SubTokenType
    {
      LEX_SYMBOL = 0,
      LEX_WORD = 1
    };

    static const OSid ID_UNKNOWN_WORD_TOKEN = 0xffffffff; // todo: correctness (can this value be hashed?)

    TokenType activeTokenType;
    SubTokenType activeSubTokenType;

    INLINE OSid stringToken(const_cstring tokenName, const_cstring value);
    INLINE OSid charToken(const_cstring tokenName, char value);
    INLINE OSid boundedToken(const_cstring tokenName, const_cstring leftBoundingValue, const_cstring rightBoundingValue);
    INLINE void constructTokens();

    // Statement construction
    enum StatementType
    {
      SEQUENCE_STATEMENT = 0,
      SWITCH_STATEMENT = 1
    };

    INLINE OSid beginStatement(StatementType statementType, const_cstring statementName);
    INLINE OSid beginSequence(const_cstring statementName) { return beginStatement(SEQUENCE_STATEMENT, statementName); }
    INLINE OSid beginSwitch(const_cstring statementName)   { return beginStatement(SWITCH_STATEMENT, statementName); }

    INLINE void endSequence();
    INLINE void endSwitch();

    INLINE void sequenceElement(OSid id, PARSER_SEQUENCE_MULTIPLIER multiplier);
    INLINE void switchElement(OSid statementId);

    INLINE void constructStatements();

    INLINE void beginGlobal();
    INLINE void globalStatement(OSid statementId);
    INLINE void constructGlobalStatements();

    // Parsing
    INLINE OSobject parseFile(const_cstring fileName);

#ifdef _DEBUG
    void debugOutputTokens() const;
    void debugOutputStatements() const;
    void debugOutputParseResult(OSobject& parseResult) const;
#endif

  protected:

    ////// Grammar construction
    //// General
    OSid nextTokenId;
    OSid nextStatementId;

    bool global;

    //// Tokens
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
      TokenRootIndex tokenRootIndices[4][255];
      struct
      {
        TokenRootIndex rawTokenRootIndices[255];
        TokenRootIndex nilTokenRootIndices[255];
        TokenRootIndex lexSymbolTokenRootIndices[255];
        TokenRootIndex lexWordTokenRootIndices[255];
      };
    };

    // Token Name (todo: perhaps change this to ElementName and use set for all tokens + statements)
    struct TokenName
    {
      std::string name;
      OSid id;

      INLINE TokenName(std::string name, OSid id) : name(name), id(id) {}
    };

    typedef multi_index_container
    <
      TokenName,
      indexed_by
      <
        hashed_unique< member<TokenName, std::string, &TokenName::name> >,
        ordered_unique< tag<OSid>, member<TokenName, OSid, &TokenName::id> >
      >
    > TokenNameSet;

    TokenNameSet  tokenNames[3];
    TokenNameSet& rawTokenNames;
    TokenNameSet& nilTokenNames;
    TokenNameSet& lexTokenNames;

    TokenNameSet allTokenNames;

    // Token construction set (used to retain order during token insertion)
    typedef multi_index_container
    <
      Token,
      //indexed_by< hashed_unique< member<Token, char*, &Token::value> > >
      indexed_by< sequenced<> >
    > TokenConstructionSet;

    TokenConstructionSet constructionTokens; // tokens array used during construction (Indexed by token value)

    //// Statements
    // Statement elements
    struct StatementElement
    {
      OSid id;

      FORCE_INLINE StatementElement() {}
      FORCE_INLINE StatementElement(OSid id) : id(id) {}
    };

    struct SequenceElement : public StatementElement
    {
      PARSER_SEQUENCE_MULTIPLIER multiplier;

      FORCE_INLINE SequenceElement() {}
      //FORCE_INLINE SequenceElement(SequenceElement& sequenceElement) :  StatementElement(sequenceElement.id), multiplier(sequenceElement.multiplier) {}
      FORCE_INLINE SequenceElement(OSid id, PARSER_SEQUENCE_MULTIPLIER multiplier) : StatementElement(id), multiplier(multiplier) {}
    };

    struct Statement
    {
      uint8 elementsLength;
      StatementElement* elements;

      FORCE_INLINE Statement() : elementsLength(0), elements(null) {}
    };

    struct StatementState
    {
      StatementType type;
      Statement*    statement;
    };

    stack<StatementState> parentStatementStates;
    StatementState        activeStatementState;

    vector<StatementElement> constructionSwitchElements;
    vector<SequenceElement>  constructionSequenceElements;

    /*OLD:
    vector<Statement>  statements[2];
    vector<Statement>& sequenceStatements;
    vector<Statement>& switchStatements;*/

    // Deterministic statement root (parse) index
    const Statement** statementRootIndex;

    ////// Parsing
    //// Parse result
    struct ParseResult
    {
      // Input data
      const char* inputData;
      uint        inputLength;

      ParseMatch* matches;
      uint        matchesLength;
      uint        globalMatchesLength;

      // Token matches
      ParseMatch* tokenMatches;
      uint        tokenMatchesLength;

/*    // Statement matches
      ParseMatch** statementMatches;
      uint         statementLevels;
      uint*        statementLevelLengths;*/
    };

    vector<ParseMatch> constructMatches; // construction set for parse matches
    vector< vector<ParseMatch> > constructStatementMatches;

    //// Parse operations
    INLINE void lexicalAnalysis(ParseResult& parseResult);
    INLINE bool parseSymbolToken(TokenType tokenType, const char* inputPosition, uint inputLength, ParseMatch& tokenMatch) const;
    INLINE bool matchSymbolToken(const Token& token, const char* inputPosition, uint length, uint16& matchLength) const;
    INLINE bool matchBoundingToken(const Token& token, const char* inputPosition, uint length, uint16& matchLength) const;

    INLINE void parseWordToken(const char* inputPosition, ParseMatch& tokenMatch) const;
    INLINE bool matchWordToken(const Token& token, const char* inputPosition) const;

    /*INLINE void statementReduction(ParseResult& parseResult);
    INLINE bool reduceStatement(ParseResult& parseResult, const Statement* statement, uint parsePosition);*/


    /* Temporary: (deep parsing) */
    map<OSid, Statement*>    statementIds;
    map<OSid, StatementType> statementTypes;
    map<OSid, string>        statementNames;
    vector<OSid>             globalStatements;


    INLINE void deepStatementReduction(ParseResult& parseResult);
    INLINE bool deepParseStatement(const ParseResult& parseResult, vector<ParseMatch>& statementMatches, uint& cParsePosition);
    //INLINE bool deepReduceStatement(const ParseResult& parseResult, OSid statementId, uint& cParsePosition, uint cLevel);
    INLINE bool deepReduceStatement(const ParseResult& parseResult, OSid statementId, vector<ParseMatch>& statementMatches, uint& cParsePosition);
    //INLINE void consolidateStatementMatches(ParseResult& parseResult, vector<ParseMatch>& parseMatches, vector<ParseMatch>::iterator& iParseMatch, uint statementLevel);
    INLINE void consolidateStatementMatches(ParseResult& parseResult, const vector<ParseMatch>::iterator& beginParseMatches, const vector<ParseMatch>::iterator& endParseMatches, uint statementLevel);

    //// Grammar construction operations
    INLINE void constructStatementRootIndex();
    //INLINE OSid getNextOSid();
    INLINE OSid getNextTokenId();
    INLINE OSid getNextStatementId();
    INLINE bool isTokenId(OSid id) const;

    //// Accessors
    INLINE const string& getTokenName(OSid tokenId) const;
    INLINE const string& getStatementName(OSid statementId) const;

    //// Miscelaneous
    void outputStatementMatch(ParseResult& result, uint c) const;

  };
}

/*                                   INCLUDES                               */
#include "grammar.hxx"

#endif
