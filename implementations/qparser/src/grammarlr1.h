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
    
    /*// An LR(1) state is a set of LR(1) items
    struct State
    {
      typedef map<OSid, int> Edges; // (edge label = symbol, target = state index where -1 is an end state)
      vector<Item> items;
      Edges edges;
    };

    vector<State*>             states;
    map<Item, uint>            itemStateIndex; // And index of what state each item maps to (for quick lookup)*/
    vector<BinaryIndexElement> binaryParseTable;

    // Construct an LR(1) linear binary indexed parse table
    INLINE void constructParseTable();

    // Get all initial items for productions that produce a certain non-terminal symbol. For the second version,
    // the follow symbol in the parent item is added to each start item's symbols after the lookahead position.
    INLINE void getStartItems(OSid nonterminal, vector<Item>& items);
    INLINE void getLookaheadStartItems(OSid nonterminal, vector<Item>& items, const Item& parentItem);

    // Get an item's lookahead terminal symbols (all possible terminals that can follow after the current input position)
    // including -1 if no terminal is a possibility (i.e. end-of-stream)
    INLINE void getLookaheadTerminals(const Item& item, set<OSid>& lookaheadTerminals);

    // Get the first set of terminals for some token id. Returns true if the set is nullable.
    // todo: we'll remove the bool and instead just store nullable directly in the productionSet
    INLINE bool getFirstTerminals(OSid id, set<OSid>& firstTerminals);

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
