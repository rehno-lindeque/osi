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
  struct LRItem
  {
    OSid productionId;
    uint productionIndex;
    uint inputPosition;
    
    INLINE LRItem(OSid nonterminal) : productionId(nonterminal), productionIndex(0), inputPosition(0) {}
  
  private:
    INLINE LRItem() {}
  };
  
  template<typename Item>
  class GrammarLR : public Grammar
  {
  public:
    // Constructor
    INLINE GrammarLR() {}
    INLINE ~GrammarLR();
    
  protected:

    // An LR state is a set of LR items
    struct State
    {
      typedef map<OSid, int> Edges; // (edge label = symbol, target = state index where -1 is an end state)
      vector<Item> items;
      Edges edges;
    };

    vector<State*> states;
    map<Item, uint> itemStateIndex; // An index of what state each item maps to (for quick lookup)

    // Binary Index Element for the parse table
    #pragma pack(push)
#pragma pack(0)
  public:
    struct BinaryIndexElement
    {
      //uint state;
      OSid id;

      // Note: 4 least significant bits indicates the action. 2 most significant bits
      //       (128 & 64) are identifier additions (reference & declaration)
      enum Action
      {
        LRACTION_SHIFT                = 0,
        LRACTION_GOTO                 = 1,
        LRACTION_REDUCE               = 2,
        LRACTION_ACCEPT               = 3,
        LRACTION_ERROR                = 4,

        LRACTION_FLAG_SILENT          = 32,
        LRACTION_FLAG_IDREF           = 64,
        LRACTION_FLAG_IDDECL          = 128,
      } action;
      uint param;
      uint largerIndex;
    };
  protected:
#pragma pack(pop)

    
    // Get all initial items for productions that produce a certain non-terminal symbol
    INLINE void getStartItems(OSid nonterminal, vector<Item>& items);

    // Get the first set of terminals for some token id. Returns true if the set is nullable.
    // todo: we'll remove the bool and instead just store nullable directly in the productionSet
    INLINE bool getFirstTerminals(OSid id, set<OSid>& firstTerminals);
    
    // Get an item's lookahead terminal symbols (all possible terminals that can follow after the current input position)
    // including -1 if no terminal is a possibility (i.e. end-of-stream)
    INLINE void getLookaheadTerminals(const Item& item, set<OSid>& lookaheadTerminals);

    // Find the state (index) that an item belongs to. Returns -1 if the item does not exist yet.
    INLINE int findItemState(const Item& items);

#ifdef _DEBUG
    virtual void debugOutputItem(const Item& item) const = 0;
    INLINE void debugOutputEdge(typename State::Edges::const_reference edge) const;
    virtual void debugOutputStates() const;
#endif
  };
}

/*                                   INCLUDES                               */
#include "grammarlr.inl"

#endif
