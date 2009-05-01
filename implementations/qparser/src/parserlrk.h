#ifndef __QPARSER_GRAMMARLRK_H__
#define __QPARSER_GRAMMARLRK_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMARLRK.H
//
//    Copyright © 2007-2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      LR(K) class grammar construction, for some arbitrary value of K
 
    USAGE:
      Values of K should remain as small as possible for memory and efficiency 
      purposes. A value of K = 2 is typical.

    REFERENCES:
      + [modcomp]
        title = "Modern Compiler Implementation in Java, Second Edition"
        author = "Andrew W. Appel and Jens Palsberg"
        publisher = "Cambridge University Press"
        isbn = "0-521-82060-X"
        pages = "63-64"
        year = "2002"
*/

/*                              COMPILER MACROS                             */
/*                                   INCLUDES                               */
namespace QParser
{
/*                                  CLASSES                                 */
  // An LR(K) item
  template<int K>
  struct LRKItem : public LRItem
  {
    OSid lookaheadSymbols[K];
    
    INLINE LRKItem(OSid nonterminal) : LRItem(nonterminal) { for(uint c = 0; c < K; ++c) lookaheadSymbols[c] = -1; }

    // todo: There's probably some nice SIMD instructions that can do this in less steps if we pack this into a bunch of uint32s...?
    FORCE_INLINE bool operator < (const LRKItem<K>& item) const
    { 
      if(productionId < item.productionId
        || (productionId == item.productionId
          && (productionIndex < item.productionIndex
            || (productionIndex == item.productionIndex
             && inputPosition < item.inputPosition))))
        return true;
      else if(inputPosition != item.inputPosition)
        return false;
      for(uint c = 0; c < K; ++c)
        if(lookaheadSymbols[c] >= item.lookaheadSymbols[c])
          return false;
      return true;      
    }
    FORCE_INLINE bool operator == (const LRKItem<K>& item) const 
    { 
      if(!(productionId == item.productionId && productionIndex == item.productionIndex && inputPosition == item.inputPosition))
        return false;      
      for(uint c = 0; c < K; ++c)
        if(lookaheadSymbols[c] != item.lookaheadSymbols[c])
          return false;
      return true;
    }
    FORCE_INLINE bool operator != (const LRKItem& item) const { return !(*this == item); }
  };
    
  template<int K>
  class GrammarLRK : public GrammarLR< LRKItem<K> >
  {
  public:
    typedef LRKItem<K> Item;
    typedef Grammar::ParseResult ParseResult;
    typedef Grammar::Production Production;
    typedef Grammar::ProductionSet ProductionSet;
    typedef GrammarLR<Item> LR;
    typedef typename LR::BinaryIndexElement BinaryIndexElement;
    typedef typename LR::State State;
  
    // Constructor
    INLINE GrammarLRK() {}
    INLINE ~GrammarLRK();

    // Construct productions
    virtual void ConstructParser(Grammar* grammar);

    // Parse
    virtual void parse(ParseResult& parseResult);

  protected:
    vector<BinaryIndexElement> binaryParseTable;
    
    using LR::getTokenName;
    using LR::getProductionSet;
    using LR::isTerminal;
    using LR::isSilent;
    using LR::findPrecedenceDirective;
    using LR::debugOutputSymbol;
    
    using LR::states;
    using LR::precedenceMap;
    using LR::errorStream;
    using LR::startSymbol;
    using LR::nextNonterminal;
    using LR::productionSets;
    using LR::productions;
    using LR::itemStateIndex;
    
    using LR::ID_IDENTIFIER_DECL;
    using LR::ID_IDENTIFIER_REF;
    using LR::ID_IDENTIFIER;
    using LR::ID_SPECIAL;

    // Construct an LR(K) linear binary indexed parse table
    INLINE void constructParseTable();

    // This is similar to the getStartItems function in the base class, but the follow symbol in the parent 
    // item is added to each start item's symbols after the lookahead position.
    typedef LookaheadIds OSid[k];
    typedef LookaheadSet set<LookaheadIds>;
    INLINE void getLookaheadStartItems(OSid nonterminal, vector<Item>& items, const Item& parentItem);
    INLINE void getLookaheadTerminals(const Item& item, LookaheadSet& lookaheadTerminals);

    // Determine the closure of a set of items
    INLINE void closure(vector<Item>& items);
    INLINE void closure(vector<Item>& items, uint cBegin, uint cEnd);

    // Determine the goto states, items and edges for an existing set of states
    INLINE void goTo(vector<State*>& states);
    INLINE void goTo(vector<State*>& states, uint cBegin, uint cEnd);

    // Build the binary table using the states DFA graph
    INLINE void constructBinaryLRKTable();
    //todo: INLINE void constructBinaryLALRKTable();

#ifdef _DEBUG
    virtual void debugOutputItem(const Item& item) const;
    virtual void debugOutputTable() const;
#endif
  };
}

/*                                   INCLUDES                               */
#include "parserlrk.inl"

#endif
