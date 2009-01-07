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
/*
  void GrammarLR0::constructProductions()
  {
    constructParseTable();
  }

  void GrammarLR0::constructParseTable()
  {
    OSI_ASSERT(startProductionSet);

    // Algorithm: The LR parsing algorithm from [modern compiler implementation in Java (ISBN 0-521-82060-X)] has been used
    //            states represents the set of states listed as T which is the set of states seen so far
    //            State::edges represents the set of edges listed as E which is the set of shift or goto edges found so far

    // Initialize T to { Closure( { Start Production } ) }
    states.push_back(new State);
    getStartItems(startSymbol, states[0]->items);
    closure(states[0]->items);

    // Evaluate each state until no new states are found
    goTo(states);
  }

  void GrammarLR0::getStartItems(OSid nonterminal, vector<Item> &items)
  {
    OSI_ASSERT(!isTokenId(nonterminal))
    ProductionSet* productionSet = getProductionSet(nonterminal);

    Item item;
    item.inputPosition = 0;
    item.productionId = nonterminal;

    for(uint cProduction = 0; cProduction < productionSet->productionsLength; ++cProduction)
    {
      item.production = &productionSet->productions[cProduction];

      // todo: (optimization) use some form of index for this... (unfortunately stl doesn't allow multiple indexes...)
      vector<Item>::iterator i;
      for(i = items.begin(); i != items.end(); ++i)
        if(*i == item)
          break;

      if(i == items.end())
        items.push_back(item);
    }
  }

  void GrammarLR0::closure(vector<Item> &items)
  {
    closure(items, 0, items.size());
  }

  void GrammarLR0::closure(vector<Item> &items, uint cBegin, uint cEnd)
  {
    for(uint c = cBegin; c < cEnd; ++c)
    {
      Item& item = items[c];

      // Check whether the last symbol has been passed
      if(item.inputPosition >= item.production->symbolsLength)
        continue;

      OSid symbol = item.production->symbols[item.inputPosition].id;

      // Check whether the symbol at input position is a terminal symbol
      if(isTokenId(symbol))
        continue;

      // Add the closure of the nonterminal at input position to the set
      // (but first construct a starting item set for the production)
      getStartItems(symbol, items);
    }

    if(cEnd < items.size())
      closure(items, cEnd, items.size());
  }

  void GrammarLR0::goTo(vector<State*>& states)
  {
    goTo(states, 0, states.size());
  }

  void GrammarLR0::goTo(vector<State*>& states, uint cBegin, uint cEnd)
  {
    for(uint cState = cBegin; cState < cEnd; ++cState)
    {
      State& state = *states[cState];

      for(uint cItem = 0; cItem < state.items.size(); ++cItem)
      {
        const Item& item = state.items[cItem];

        // Check whether the last symbol has already been passed
        if(item.inputPosition >= item.production->symbolsLength)
          continue;

        OSid symbol = item.production->symbols[item.inputPosition].id;

        // Construct goto item
        Item goToItem;
        goToItem.inputPosition = item.inputPosition + 1;
        goToItem.productionId = item.productionId;
        goToItem.production = item.production;

        // Check whether there's already an edge for this symbol in the current state
        // Note: This is not legal for LR(0) grammars! However, SLR grammars can sometimes handle this)
        State::Edges::iterator iEdge = state.edges.find(symbol);
        int targetStateIndex;

        //OLD: if(iEdge == state.edges.end())
        if(iEdge == state.edges.end() || iEdge->first != symbol)
        {
          // Check whether there's already a state defined for this goto item
          // (This is something that [modcomp] doesn't really mention in any detail - we should look for existing target states for
          // this item or we'll end up in an infinite loop for any recursive grammar. It's very possible that a different state
          // will have the same item leading to some existing state.)
          targetStateIndex = findItemState(item);

          if (targetStateIndex == -1)
          {
            // Create a target state and a shift/goto edge
            states.push_back(new State);
            targetStateIndex = states.size() - 1;
            state.edges.insert(pair<OSid, int>(symbol, targetStateIndex));
            itemStateIndex.insert(pair<Item, uint>(item, targetStateIndex));
          }
          else
          {
            state.edges.insert(pair<OSid, int>(symbol, targetStateIndex));
            continue; // we can continue since the item is already in the target state (i.e. there's no need to add it a second time)
          }
        }
        else
        {
          // (Since there's already an edge for this symbol it follows that the target state also already exists...)
          targetStateIndex = iEdge->second;
        }

        State& targetState = *states[targetStateIndex];

        // Add goto item to set (J) (the target state)

        // Note: we still have to check whether the state already contains the goto item since it could have
        // been added by the closure of another item in the state
        vector<Item>::iterator i;
        //bug: for(i = targetState.items.begin(); i != targetState.items.end(); ++i) if(*i == item) break;
        for(i = targetState.items.begin(); i != targetState.items.end(); ++i) if(*i == goToItem) break;
        if(i != targetState.items.end()) continue;

        targetState.items.push_back(goToItem);

        // Add closure of goto item to the set (Note that the last item added to the targetState is the goto item we're refering to)
        closure(targetState.items, targetState.items.size()-1, targetState.items.size());
      }
    }

    if(cEnd < states.size())
      goTo(states, cEnd, states.size());
  }*/

  template<class Item>
  int GrammarLR<Item>::findItemState(const Item& item)
  {
    typename map<Item, uint>::iterator i = itemStateIndex.find(item);
    if(i == itemStateIndex.end() || i->first != item)
      return -1;
    return i->second;
  }

#ifdef _DEBUG
  /*void GrammarLR0::debugOutputItem(const Item& item) const
  {
    if(!item.production || item.production->symbolsLength <= 0)
      return;

    debugOutputSymbol(item.productionId);
    cout << " -> ";
    for(uint c = 0; c < item.production->symbolsLength; ++c)
    {
      if(item.inputPosition == c) cout << '.';
      debugOutputSymbol(item.production->symbols[c].id);
      if(c < item.production->symbolsLength-1)
        cout << ' ';
    }
    if(item.inputPosition == item.production->symbolsLength) cout << '.';
  }*/

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
