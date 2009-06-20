#ifdef  __QPARSER_GRAMMARLR_H__
#ifndef __QPARSER_GRAMMARLR_INL__
#define __QPARSER_GRAMMARLR_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMARLR.INL
//
//    Copyright © 2007-2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QParser
{
  template<typename Item, typename State>
  INLINE GrammarLR<Item, State>::~GrammarLR()
  {
    // Clean up memory
    for(auto i = states.begin(); i != states.end(); ++i)
      delete *i;
    states.clear();
  }

  template<typename Item, typename State>
  INLINE void GrammarLR<Item, State>::GetStartItems(ParseToken nonterminal, Items &items)
  {
    OSI_ASSERT(!TokenRegistry::IsTerminal(nonterminal));
    ProductionSet* productionSet = GetProductionSet(nonterminal);

    Item item(nonterminal);
    for(uint cProduction = 0; cProduction < productionSet->rulesLength; ++cProduction)
    {
      item.ruleIndex = productionSet->rulesOffset + cProduction;

      // Add the item to the set if not already present
      // todo: (optimization) use some form of index for this... (unfortunately stl doesn't allow multiple indexes...)
      typename Items::iterator i;
      for(i = items.begin(); i != items.end(); ++i) if (*i == item) break;
      if(i == items.end())
        items.push_back(item);
    }
  }

  template<typename Item, typename State>
  INLINE bool GrammarLR<Item, State>::GetFirstTerminals(ParseToken token, ParseTokenSet& firstTerminals)
  {
    if(TokenRegistry::IsTerminal(token))
    {
      firstTerminals.insert(token);
      return false;  // FIRST(id) is not nullable
    }

    // Initialize the nullable attribute
    ProductionSet& productionSet = *GetProductionSet(token);
    ++productionSet.visitedCount;

    if(productionSet.visitedCount > 1)
      return productionSet.nullable;

    // Get first items
    for(uint c = 0; c < productionSet.rulesLength; ++c)
    {
      const ProductionRule& rule = rules[productionSet.rulesOffset + c].first;
      bool productionNullable = true;

      for(uint cToken = 0; cToken < rule.tokensLength; ++cToken)
      {
        productionNullable &= GetFirstTerminals(rule.tokens[cToken], firstTerminals);
        if(!productionNullable)
          break;
      }

      // If the production set's nullity changes and the production set is recursive, we must restart the process with the new nullity value
      if((productionNullable || rule.tokensLength == 0) && productionSet.nullable == false)
      {
        // Note: If getFirstTerminals is called on productionSet for the first time, its nullity MUST be false.
        //       It can change to true during evaluation of this function, but can never return to false.
        //       If the nullity of the nonterminal changes and the nonterminals is recursive (visited count becomes > 1
        //       during evaluation of its sub tokens), the function must be restarted using the new nullity value
        //       (which must then be true since we can't return to false).
        //       Also note that we don't need to restore the firstTerminals to its previous state since, we can only
        //       add more terminals by making this nonterminal nullable.

        productionSet.nullable = true;
        productionSet.visitedCount = 0;
        return GetFirstTerminals(token, firstTerminals);
      }
    }

    // Reset the visited count (since this is the first evaluation of the production set)
    productionSet.visitedCount = 0;

    return productionSet.nullable;
  }

  template<typename Item, typename State>
  INLINE void GrammarLR<Item, State>::GetLookaheadTerminals(const Item& item, ParseTokenSet& lookaheadTerminals)
  {
    const ProductionRule& rule = rules[item.ruleIndex].first;
    for(uint cToken = item.inputPosition + 1; cToken < rule.tokensLength; ++cToken)
    {
      if(!GetFirstTerminals(rule.tokens[cToken], lookaheadTerminals))
        return; // The symbol (terminal / nonterminal) is not nullable, so we're done
    }

    // Add the lookahead symbol of the parent item if all other items are nullable
    // (this might be -1 for no lookahead)
    lookaheadTerminals.insert(item.lookaheadSymbol);
  }

  template<typename Item, typename State>
  INLINE int GrammarLR<Item, State>::FindItemState(const Item& item)
  {
    typename ItemStateMap::iterator i = itemStateMap.find(item);
    if(i == itemStateMap.end() || i->first != item)
      return -1;
    return i->second;
  }

  template<typename Item, typename State>
  INLINE bool GrammarLR<Item, State>::IsItemComplete(const Item& item) const
  {
    return item.inputPosition == GetRule(item.ruleIndex).tokensLength;
  }

#ifdef _DEBUG
  /*template<typename Item, typename State>
  void GrammarLR<Item, State>::DebugOutputEdge(typename Edges::const_reference edge) const
  {
    using std::cout;
    
    std::cout << "--(";
    debugOutputSymbol(edge.first);
    std::cout << ")--> " << edge.second;
  }

  template<typename Item, typename State>
  void GrammarLR<Item>::DebugOutputStates() const
  {
    using std::cout;
    using std::endl;
    
    cout << endl;
    for(uint c = 0; c < states.size(); ++c)
    {
      const State& state = *states[c];
      cout << "State " << c << endl;
      cout << "---------" << endl;
      cout << "Items:" << endl;
      for(typename Items::const_iterator iItem = state.items.begin(); iItem != state.items.end(); ++iItem)
      {
        cout << ' ';
        debugOutputItem(*iItem);
        cout << endl;
      }

      cout << "Edges:" << endl;
      for(typename Edges::const_iterator iEdge = state.edges.begin(); iEdge != state.edges.end(); ++iEdge)
      {
        cout << ' ';
        DebugOutputEdge(*iEdge);
        cout << endl;
      }

      cout << endl;
    }
  }*/
#endif
}

#endif
#endif
