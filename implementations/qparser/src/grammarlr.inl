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
  template<class Item>
  GrammarLR<Item>::~GrammarLR()
  {
    // Clean up memory
    for(typename vector<State*>::iterator i = states.begin(); i != states.end(); ++i)
      delete *i;
  }

  template<class Item>
  void GrammarLR<Item>::getStartItems(OSid nonterminal, vector<Item> &items)
  {
    OSI_ASSERT(!isTerminal(nonterminal));
    ProductionSet* productionSet = getProductionSet(nonterminal);

    Item item(nonterminal);
    for(uint cProduction = 0; cProduction < productionSet->productionsLength; ++cProduction)
    {
      item.productionIndex = productionSet->productionsOffset + cProduction;

      // Add the item to the set if not already present
      // todo: (optimization) use some form of index for this... (unfortunately stl doesn't allow multiple indexes...)
      typename vector<Item>::iterator i;
      for(i = items.begin(); i != items.end(); ++i) if (*i == item) break;
      if(i == items.end())
        items.push_back(item);
    }
  }
  
  template<class Item>
  bool GrammarLR<Item>::getFirstTerminals(OSid id, set<OSid>& firstTerminals)
  {
    if(isTerminal(id))
    {
      firstTerminals.insert(id);
      return false;  // FIRST(id) is not nullable
    }

    // Initialize the nullable attribute
    ProductionSet& productionSet = *getProductionSet(id);
    ++productionSet.visitedCount;

    if(productionSet.visitedCount > 1)
      return productionSet.nullable;

    // Get first items
    for(uint c = 0; c < productionSet.productionsLength; ++c)
    {
      const Production& production = productions[productionSet.productionsOffset + c].first;
      bool productionNullable = true;

      for(uint cSymbol = 0; cSymbol < production.symbolsLength; ++cSymbol)
      {
        productionNullable &= getFirstTerminals(production.symbols[cSymbol].id, firstTerminals);
        if(!productionNullable)
          break;
      }

      // If the production set's nullity changes and the production set is recursive, we must restart the process with the new nullity value
      if((productionNullable || production.symbolsLength == 0) && productionSet.nullable == false)
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
        return getFirstTerminals(id, firstTerminals);
      }
    }

    // Reset the visited count (since this is the first evaluation of the production set)
    productionSet.visitedCount = 0;

    return productionSet.nullable;
  }
  
  template<class Item>
  void GrammarLR<Item>::getLookaheadTerminals(const Item& item, set<OSid>& lookaheadTerminals)
  {
    const Production& production = productions[item.productionIndex].first;
    for(uint cSymbol = item.inputPosition + 1; cSymbol < production.symbolsLength; ++cSymbol)
    {
      if(!getFirstTerminals(production.symbols[cSymbol].id, lookaheadTerminals))
        return; // The symbol (terminal / nonterminal) is not nullable, so we're done
    }

    // Add the lookahead symbol of the parent item if all other items are nullable
    // (this might be -1 for no lookahead)
    lookaheadTerminals.insert(item.lookaheadSymbol);
  }

  template<class Item>
  int GrammarLR<Item>::findItemState(const Item& item)
  {
    typename map<Item, uint>::iterator i = itemStateIndex.find(item);
    if(i == itemStateIndex.end() || i->first != item)
      return -1;
    return i->second;
  }

#ifdef _DEBUG
  template<class Item>
  void GrammarLR<Item>::debugOutputEdge(typename State::Edges::const_reference edge) const
  {
    cout << "--(";
    debugOutputSymbol(edge.first);
    cout << ")--> " << edge.second;
  }

  template<class Item>
  void GrammarLR<Item>::debugOutputStates() const
  {
    cout << endl;
    for(uint c = 0; c < states.size(); ++c)
    {
      const State& state = *states[c];
      cout << "State " << c << endl;
      cout << "---------" << endl;
      cout << "Items:" << endl;
      for(typename vector<Item>::const_iterator iItem = state.items.begin(); iItem != state.items.end(); ++iItem)
      {
        cout << ' ';
        debugOutputItem(*iItem);
        cout << endl;
      }

      cout << "Edges:" << endl;
      for(typename State::Edges::const_iterator iEdge = state.edges.begin(); iEdge != state.edges.end(); ++iEdge)
      {
        cout << ' ';
        debugOutputEdge(*iEdge);
        cout << endl;
      }

      cout << endl;
    }
  }
#endif
}

#endif
#endif
