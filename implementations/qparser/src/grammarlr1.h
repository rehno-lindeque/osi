#ifndef __QPARSER_GRAMMARLR1_H__
#define __QPARSER_GRAMMARLR1_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMARLR1.H
//
//    Copyright © 2007-2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      LR(0) class grammar construction

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
  // An LR(1) item
  struct LR1Item : public LRItem
  {
    //OSid productionId;
    //uint productionIndex;
    //uint inputPosition;
    OSid lookaheadSymbol;
    
    INLINE LR1Item(OSid nonterminal, OSid lookaheadSymbol = -1) : LRItem(nonterminal), lookaheadSymbol(lookaheadSymbol) {}

    // todo: There's probably some nice SIMD instructions that can do this in less steps if we pack this into a bunch of uint32s...?
    //       The (Production*) could be a problem though. (No longer: We now use a 32 bit) index rather than a pointer)
    FORCE_INLINE bool operator < (const LR1Item& item) const
    { return productionId < item.productionId
             || (productionId == item.productionId
               && (productionIndex < item.productionIndex
                 || (productionIndex == item.productionIndex
                   && (inputPosition < item.inputPosition
                     || (inputPosition == item.inputPosition
                       && lookaheadSymbol < item.lookaheadSymbol))))); }
    FORCE_INLINE bool operator == (const LR1Item& item) const { return productionId == item.productionId && productionIndex == item.productionIndex && inputPosition == item.inputPosition && lookaheadSymbol == item.lookaheadSymbol; }
    FORCE_INLINE bool operator != (const LR1Item& item) const { return !(*this == item); }
  };
    
  class GrammarLR1 : public GrammarLR<LR1Item>
  {
  public:
    typedef LR1Item Item;
  
    // Constructor
    INLINE GrammarLR1() {}
    INLINE ~GrammarLR1();

    // Construct productions
    virtual void constructProductions();

    // Parse
   virtual void parse(ParseResult& parseResult);

  protected:
    vector<BinaryIndexElement> binaryParseTable;

    // Construct an LR(1) linear binary indexed parse table
    INLINE void constructParseTable();

    // This is similar to the getStartItems function in the base class, but the follow symbol in the parent 
    // item is added to each start item's symbols after the lookahead position.
    INLINE void getLookaheadStartItems(OSid nonterminal, vector<Item>& items, const Item& parentItem);

    // Determine the closure of a set of items
    INLINE void closure(vector<Item>& items);
    INLINE void closure(vector<Item>& items, uint cBegin, uint cEnd);

    // Determine the goto states, items and edges for an existing set of states
    INLINE void goTo(vector<State*>& states);
    INLINE void goTo(vector<State*>& states, uint cBegin, uint cEnd);

    // Build the binary table using the states DFA graph
    INLINE void constructBinaryLR1Table();
    //todo: INLINE void constructBinaryLALR1Table();

#ifdef _DEBUG
    virtual void debugOutputItem(const Item& item) const;
    virtual void debugOutputTable() const;
#endif
  };
}

/*                                   INCLUDES                               */
#include "grammarlr1.inl"

#endif
