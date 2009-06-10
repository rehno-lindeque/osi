#ifndef __QPARSER_GRAMMARLR_H__
#define __QPARSER_GRAMMARLR_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMARLR.H
//
//    Copyright © 2007-2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      A base class for LR class grammars.

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
  // Base item structure to use LR-style grammars
  struct LRItem
  {
    ParseToken nonterminal; // Nonterminal produced by this item's production
    uint ruleIndex;         // Index of the production rule that this item represents
    uint inputPosition;     // Position in the input tokens of the production which this item represents
    
    INLINE LRItem(ParseToken nonterminal) : 
      nonterminal(nonterminal), 
      ruleIndex(0), 
      inputPosition(0) {}
    INLINE LRItem(const LRItem&) = default;
    INLINE LRItem() = delete;
  };
  
  // LR Grammar base class
  template<typename Item>
  class GrammarLR : public Grammar
  {
  public:
    // Constructor
    INLINE GrammarLR(TokenRegistry& tokenRegistry) : Grammar(tokenRegistry) {}
    INLINE GrammarLR(const GrammarLR&) = delete;
    INLINE GrammarLR() = delete;
    
    // Destructror
    INLINE ~GrammarLR();
    
  protected:
    // Container types
    struct State;
    typedef std::vector<Item> Items;              // The set of items typically contained by a state
    typedef std::map<ParseToken, int> Edges;      // An edge between states where the label = nonterminal/terminal token and the target = state index where -1 is an end state
    typedef std::vector<State*> States;           // The set of states
    typedef std::map<Item, uint> ItemStateMap;    // A mapping between items and the index of the state they belong to (in the states member variable)
    
    // An LR state is a set of LR items
    struct State
    {
      Items items;    // A set of indexes contained by the state
      Edges edges;    // A set of edges connecting this state to other states
    };
    
    // Members
    States states;              // The set of all of the states in the parsing table
    ItemStateMap itemStateMap;  // A map of what state each item maps to (for quick lookup)

    // Get all initial items for productions that produce a certain non-terminal symbol
    INLINE void GetStartItems(ParseToken nonterminal, Items& items);

    // Get the first set of terminals for some token id. Returns true if the set is nullable.
    // todo: we'll remove the bool and instead just store nullable directly in the productionSet
    INLINE bool GetFirstTerminals(ParseToken token, ParseTokenSet& firstTerminals);
    
    // Get an item's lookahead terminal symbols (all possible terminals that can follow after the current input position)
    // including -1 if no terminal is a possibility (i.e. end-of-stream)
    INLINE void GetLookaheadTerminals(const Item& item, ParseTokenSet& lookaheadTerminals);

    // Find the state (index) that an item belongs to. Returns -1 if the item does not exist yet.
    INLINE int FindItemState(const Item& items);

#ifdef _DEBUG
    /*virtual void DebugOutputItem(const Item& item) const = 0;
    INLINE void DebugOutputEdge(typename State::Edges::const_reference edge) const;
    virtual void DebugOutputStates() const;*/
#endif
  };
}

/*                                   INCLUDES                               */
#include "grammarlr.inl"

#endif
