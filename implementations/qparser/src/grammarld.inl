#ifdef  __QPARSER_GRAMMARLD_H__
#ifndef __QPARSER_GRAMMARLD_INL__
#define __QPARSER_GRAMMARLD_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMARLD.INL
//
//    Copyright © 2007-2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QParser
{
  GrammarLD::~GrammarLD()
  {
    /*// Clean up memory
    for(typename States::iterator i = states.begin(); i != states.end(); ++i)
      delete *i;*/
  }
  
  void GrammarLD::ConstructParseTable(ParseTokens& parseTable)
  {
    // Ensure that the root nonterminal a valid nonterminal token
    // We use the last production defined if no root was given by the user    
    if(rootNonterminal == ParseToken(-1))
    {
      if(tokenRegistry.GetNextAvailableNonterminal() == TOKEN_NONTERMINAL_FIRST) 
        return; // error: no nonterminals defined
      else 
        rootNonterminal = tokenRegistry.GetNextAvailableNonterminal() - 1; // Use the last defined nonterminal our root nonterminal
    }
    
    // Construct the states graph for the parser
    // Algorithm: This algorithm was derived primarily from the LR parsing algorithm from [modern compiler implementation in Java (ISBN 0-521-82060-X)]
    //            states represents the set of states listed as T which is the set of states seen so far
    //            State::edges represents the set of edges listed as E which is the set of shift or goto edges found so far

    // Initialize T to { Closure( { Start Production } ) }
    states.push_back(new State);
    getStartItems(rootNonterminal, states[0]->items);
    Closure(states[0]->items);

    // Evaluate each state until no new states are found
    GoTo(states);
    
    // Use the builder to construct the final parse table
    BuilderLD builder;
    builder.ConstructParseTable(parseTable);
  }
  
  void GrammarLD::Closure(Items& items)
  {
    Closure(items, 0, items.size());
  }

  void GrammarLD::Closure(Items& items, uint cBegin, uint cEnd)
  {
    for(uint c = cBegin; c < cEnd; ++c)
    {
      auto& item = items[c];
      const auto& production = productions[item.productionIndex].first;

      // Check whether the last symbol has been passed
      if(item.inputPosition >= production.tokensLength)
        continue;

      ParseToken token = production.tokens[item.inputPosition];

      // Check whether the symbol at input position is a terminal symbol
      if(TokenRegistry::IsTerminal(token))
        continue;

      // Add the closure of the nonterminal at input position to the set
      // (but first construct a starting item set for the production)
      getStartItems(token, items);
    }

    if(cEnd < items.size())
      Closure(items, cEnd, items.size());
  }

  void GrammarLD::GoTo(States& states)
  {
    GoTo(states, 0, states.size());
  }

  void GrammarLD::GoTo(States& states, uint cBegin, uint cEnd)
  {
    using std::pair;
    
    for(uint cState = cBegin; cState < cEnd; ++cState)
    {
      auto& state = *states[cState];

      for(uint cItem = 0; cItem < state.items.size(); ++cItem)
      {
        const auto& item = state.items[cItem];
        const auto& production = productions[item.productionIndex].first;

        // Check whether the last symbol has already been passed
        if(item.inputPosition >= production.tokensLength)
          continue;

        ParseToken token = production.tokens[item.inputPosition];

        // Construct goto item
        Item goToItem = item;
        goToItem.inputPosition = item.inputPosition + 1;

        // Check whether there's already an edge for this symbol in the current state
        // Note: This is not legal for LR(0) grammars! However, SLR grammars can sometimes handle this)
        auto iEdge = state.edges.find(token);
        int targetStateIndex;

        if(iEdge == state.edges.end() || iEdge->first != token)
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
            state.edges.insert(pair<ParseToken, int>(token, targetStateIndex));
            itemStateMap.insert(pair<Item, uint>(item, targetStateIndex));
          }
          else
          {
            state.edges.insert(pair<ParseToken, int>(token, targetStateIndex));
            continue; // we can continue since the item is already in the target state (i.e. there's no need to add it a second time)
          }
        }
        else
        {
          // (Since there's already an edge for this symbol it follows that the target state also already exists...)
          targetStateIndex = iEdge->second;
        }

        auto& targetState = *states[targetStateIndex];

        // Add goto item to set (J) (the target state)

        // Note: we still have to check whether the state already contains the goto item since it could have
        // been added by the closure of another item in the state
        Items::iterator i;
        for(i = targetState.items.begin(); i != targetState.items.end(); ++i) if(*i == goToItem) break;
        if(i != targetState.items.end()) continue;

        targetState.items.push_back(goToItem);

        // Add closure of goto item to the set (Note that the last item added to the targetState is the goto item we're refering to)
        Closure(targetState.items, targetState.items.size()-1, targetState.items.size());
      }
    }

    if(cEnd < states.size())
      GoTo(states, cEnd, states.size());
  }
}

#endif
#endif
