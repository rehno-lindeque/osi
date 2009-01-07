#ifndef __QPARSER_GRAMMARLR0_H__
#define __QPARSER_GRAMMARLR0_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMARLR0.H
//
//    Copyright © 2007-2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      LR(0) class grammar construction. Ability to extend to SLR class
      grammars has been allowed (i.e. multiple items can share the same 
      symbol-state edge, which is normally illegal in LR(0))

    REFERENCES:
      + [modcomp]
        title = "Modern Compiler Implementation in Java, Second Edition"
        author = "Andrew W. Appel and Jens Palsberg"
        publisher = "Cambridge University Press"
        isbn = "0-521-82060-X"
        pages = "58-62"
        year = "2002"
*/

/*                              COMPILER MACROS                             */
/*                                   INCLUDES                               */
namespace QParser
{
/*                                  CLASSES                                 */
  // An LR(0) item
  struct LR0Item : public LRItem
  {    
    INLINE LR0Item(OSid nonterminal) : LRItem(nonterminal) {}
    
    FORCE_INLINE bool operator < (const LR0Item& item) const
    { return productionId < item.productionId
        || (productionId == item.productionId
          && (productionIndex < item.productionIndex
            || (productionIndex == item.productionIndex
              && inputPosition < item.inputPosition))); }
    FORCE_INLINE bool operator == (const LR0Item& item) const { return productionId == item.productionId && productionIndex == item.productionIndex && inputPosition == item.inputPosition; }
    FORCE_INLINE bool operator != (const LR0Item& item) const { return !(*this == item); }
    
    //FORCE_INLINE bool operator < (const Item& item) const { return productionId < item.productionId || (productionId == item.productionId && (production < item.production || (production == item.production && inputPosition < item.inputPosition))); }
    //FORCE_INLINE bool operator == (const Item& item) const { return productionId == item.productionId && production == item.production && inputPosition == item.inputPosition; }
    //FORCE_INLINE bool operator != (const Item& item) const { return !(*this == item); }
  };
    
  class GrammarLR0 : public GrammarLR<LR0Item>
  {
  public:
    // Constructor
    INLINE GrammarLR0() {}
    INLINE ~GrammarLR0();

    //// Construction
    // Construct productions
    virtual void constructProductions();

  protected:
    // Construct an LR(0) linear binary indexed parse table
    INLINE void constructParseTable();

    /* An LR(0) state is a set of LR(0) items
    struct State
    {
      typedef map<OSid, int> Edges; // (edge label = symbol, target = state index where -1 is an end state)
      vector<Item> items;
      Edges edges;
    };

    vector<State*> states;
    map<Item, uint> itemStateIndex; // And index of what state each item maps to (for quick lookup)
*/
    
    // Get all initial items for productions that produce a certain non-terminal symbol
    INLINE void getStartItems(OSid nonterminal, vector<Item>& items);

    // Determine the closure of a set of items
    INLINE void closure(vector<Item>& items);
    INLINE void closure(vector<Item>& items, uint cBegin, uint cEnd);

    // Determine the goto states, items and edges for an existing set of states
    INLINE void goTo(vector<State*>& states);
    INLINE void goTo(vector<State*>& states, uint cBegin, uint cEnd);

    // Find the state (index) that an item belongs to. Returns -1 if the item does not exist yet.
    INLINE int findItemState(const Item& items);

    // Build the binary table using the states DFA graph
    INLINE void constructBinaryTable();

#ifdef _DEBUG
    INLINE void debugOutputItem(const Item& item) const;
    INLINE void debugOutputEdge(State::Edges::const_reference edge) const;
    virtual void debugOutputStates() const;
#endif
  };
}

/*                                   INCLUDES                               */
#include "grammarlr0.inl"

#endif
