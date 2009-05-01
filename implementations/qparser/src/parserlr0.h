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
  };
    
  class GrammarLR0 : public GrammarLR<LR0Item>
  {
  public:
    typedef LR0Item Item;
    
    // Constructor
    INLINE GrammarLR0() {}
    INLINE ~GrammarLR0();

    //// Construction
    // Construct productions
    virtual void constructProductions();

  protected:
    // Construct an LR(0) linear binary indexed parse table
    INLINE void constructParseTable();

    // Determine the closure of a set of items
    INLINE void closure(vector<Item>& items);
    INLINE void closure(vector<Item>& items, uint cBegin, uint cEnd);

    // Determine the goto states, items and edges for an existing set of states
    INLINE void goTo(vector<State*>& states);
    INLINE void goTo(vector<State*>& states, uint cBegin, uint cEnd);

    // Build the binary table using the states DFA graph
    INLINE void constructBinaryLR0Table();

#ifdef _DEBUG
    virtual void debugOutputItem(const Item& item) const;
#endif
  };
}

/*                                   INCLUDES                               */
#include "parserlr0.inl"

#endif
