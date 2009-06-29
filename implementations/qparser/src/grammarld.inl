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
  {}
  
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
    
    // Construct the the parser
    // Get the start items
    BuilderLD builder;
    states.push_back(new State(builder.AddActionRow()));
    GetStartItems(rootNonterminal, states[0]->items);
    
    // Construct the state graph recursively until we are done
    std::map<LDState*, uint> resolvedRules;
    ConstructStateGraph(builder, *states[0]/*, resolvedRules*/);
    
    // Resolve all delayed reductions
    ResolveDelays(builder, *states[0]);
    
    // Use the builder to construct the final parse table
    builder.ConstructParseTable(parseTable);
  }
  
  INLINE void GrammarLD::ConstructStateGraph(BuilderLD& builder, State& state)
  {
    while(true)
    {
      // Expand items in the state
      ExpandItemSet(state);

      // Get the set of terminals (at the input positions) that can be stepped over
      ParseTokenSet terminals;
      StepOverTerminals(terminals, state.items);
      if(terminals.size() == 1)
      {
        // Geneterminalsrate a shift action
        state.row.AddActionShift(*terminals.begin());
        
        // Complete all rules that are left over
        bool allItemsComplete = CompleteItems(builder, state);
        
        // Stop if there are no more items in this state to complete
        if(allItemsComplete)
        {
          //state.row.allItemsCompleteAddActionReturn();
          return; // No items left to complete
        }
      }
      else
      {
        // Pre-condition: There will always be terminals left after the state has been expanded
        //                because then all items must necessarily be complete and rules are not allowed to be empty.
        OSI_ASSERT(terminals.size() > 1);
        /*if(terminals.size() == 0)
          return;*/
        
        // Generate a pivot
        // Try to find an existing end-state that corresponds with this end-state in the states leading up to this one.
        // If such a state exists, we've found a cycle. Otherwise we must generate new states for the pivots.
        if(!GenerateCyclicPivot(builder, state, terminals))
        {
          GeneratePivot(builder, state, terminals);
        }
        
        // Since we've reached a pivot, this row is now complete (only one decision (pivot) is allowed per row).
        // todo: determine when we can place an "accept" action rather than a "return" statement
        //state.row.AddActionReturn();
        return;
      }
    }
  }
    
  INLINE void GrammarLD::ExpandItemSet(State& state)
  {
    // Expand all items in the set
    ExpandItemSet(state, 0, state.items.size());
  }
  
  INLINE void GrammarLD::ExpandItemSet(State& state, uint cBegin, uint cEnd)
  {
    // Check whether there are any items left to expand
    if (cEnd <= cBegin+1)
      return;
    
    // Expand each item in the given range
    for (uint c = cBegin; c != cEnd; ++c)
    {
      auto& item = state.items[c];
      const ProductionRule& rule = GetRule(item.ruleIndex);
      
      // Check whether this item has already been resolved
      if(item.inputPositionRule != uint(-1))
        continue; 
      
      // Check whether the rule is complete: We cannot expand the rule further if it is complete
      if(IsItemComplete(item))
        continue;
      
      // Check whether the token at the input position is a terminal: We cannot expand a terminal token
      auto inputToken = rule.tokens[item.inputPosition];
      if(TokenRegistry::IsTerminal(inputToken))
        continue;
        
      // Resolve the input position token to rule(s)
      auto& inputProduction = *GetProductionSet(inputToken);
      
      for(auto cRuleIndex = inputProduction.rulesOffset; cRuleIndex < inputProduction.rulesOffset + inputProduction.rulesLength; ++cRuleIndex)
      {
        // Get the item again (because calling push_back may invalidate our previous reference!)
        auto& item = state.items[c];
        
        // Resolve the originating item's input position rule
        // (Note: the first time we'll merely set the input position rule, but there-after
        // the item must be duplicated for every possible input rule corresponding to the nonterminal)
        if(item.inputPositionRule == uint(-1))
        {
          item.inputPositionRule = cRuleIndex;
        }
        else
        {
          state.items.push_back(item);
          state.items.back().inputPositionRule = cRuleIndex;
        }
        
        // Generate a new item for the rule
        // Check whether a similar item already exists...
        Item newItem(inputToken, cRuleIndex);
        auto i = std::find(state.items.begin(), state.items.end(), newItem);
        if(i != state.items.end())
          continue; // The item already exists in the list
        
        // Add the new item to the state
        state.items.push_back(newItem);
      }
    }
    
    // Expand the newly added items
    ExpandItemSet(state, cEnd, state.items.size());
  }
  
  INLINE void GrammarLD::StepOverTerminals(ParseTokenSet& terminals, Items& items) const
  {
    for(auto i = items.begin(); i != items.end(); ++i)
    {
      const auto& rule = GetRule(i->ruleIndex);
      
      // Ignore items that are complete      
      if(IsItemComplete(*i))
        continue;
      
      if(TokenRegistry::IsTerminal(rule.tokens[i->inputPosition]))
      {
        // Add terminal token to the set
        terminals.insert(rule.tokens[i->inputPosition]);
        
        // Step over terminal and reset the input position rule
        ++i->inputPosition;
        i->inputPositionRule = uint(-1);
      }
    }
  }
  
  INLINE bool GrammarLD::CompleteItems(BuilderLD& builder, State& state)
  {
    Items& items = state.items; // Alias for the items
    
    // Find all rules corresponding with complete items
    bool allItemsComplete = true;     // Flag indicating that all items in the state have been completed
    bool waitingShiftActions = false; // Flag indicating that some items in the state have their input position in front of terminal tokens
    
    std::set<uint> completeRules;           // The set of all rules that have been completed
    std::set<uint> incompleteRules;         // The set of all rules that are incomplete
    std::vector<uint> completeItemIndexes;  // A list of indexes to the items that were completed
    for(uint c = 0; c < items.size(); ++c)
    {
      // If items are complete add them to a list of complete items and complete rules, otherwise add them to a set of incomplete rules
      // Note that the same rule may exist as both an incomplete and a complete item in the state. Hence one rule may be added to both the
      // sets 'completeRules' and 'incompleteRules'
      if(IsItemComplete(items[c]))
      {
        completeRules.insert(items[c].ruleIndex);
        completeItemIndexes.push_back(c);
        /////////////////////////////////// TEMPORARY
        //std::cout  << items[c].ruleIndex << std::endl;
        /////////////////////////////////// TEMPORARY
      }
      else
      {
        allItemsComplete = false;
        incompleteRules.insert(items[c].ruleIndex);        
        if(TokenRegistry::IsTerminal(GetRuleToken(items[c].ruleIndex, items[c].inputPosition)))
          waitingShiftActions = true;
      }
    }
    
    // If no more complete items could be found: All items are complete or all remaining items are incomplete
    if(completeRules.empty())
      return false;
    
    /*/// TODO: This is probably not the best way of doing things. We can probably improve the algorithm by reducing previous delays
    //       once we have exactly 1 path left, regardless of whether the items are complete or not,
    //       However, this means that we must then continue the parsing of those items in the parent node (and take into account gotos as well)
    //       Actually 
    
    //todo? If all items are complete, store the set of complete rules in the state    
    if (allItemsComplete)
    {
      // If more than one item is complete then the grammar is ambiguous
      if(completeRules.size() > 1)
      {
        // todo: ambiguous grammar
        // print out an error message or produce an ambiguous parse table...
        // e.g. ResolveDelaysAmbiguous(state, completeRules);
        
        //state.ambiguous = true;
      }
      else
      {
        OSI_ASSERT(completeRules.size() == 1);
                
        // Resolve all delayed reductions associated with this leaf state
        ResolveDelays(builder, state, *completeRules.begin());
      }
      return true;
    }*/

    // Add a reduce / delayed reduce action
    if(completeRules.size() > 1 || waitingShiftActions)
    {
      // If there are multiple completed rules, add a delayed reduce action
      state.row.AddActionReduce(TOKEN_SPECIAL_IGNORE);
      
      // Also push the possible reduce tokens onto the state's delayed stack
      state.delayedReductions.push_back(LDState::DelayedRuleMap());
      auto& delayedRuleMap = state.delayedReductions.back();
      for(auto i = completeItemIndexes.begin(); i != completeItemIndexes.end(); ++i)
      {
        const auto& item = items[*i];
        
        // Find all items that have a reference to this rule at their input positions
        for(uint c = 0; c < items.size(); ++c)
        {
          // Check whether the item has a reference to this item's rule 
          if(!IsItemComplete(items[c]) && items[c].inputPositionRule == item.ruleIndex)
            delayedRuleMap.insert(std::make_pair(items[c].ruleIndex, item.ruleIndex));
        }        
      }
      
      // Todo: What happens here if all items in the state is complete?
      //       (i.e. allItemsComplete == true)
      //       Does this mean that the grammar is ambiguous?
    }
    else
    {
      // If there is only one rule, add a reduce action 
      state.row.AddActionReduce(*completeRules.begin());
    }
    
    // Check whether all items are complete, and finish off this branch
    if (allItemsComplete)
    {
      // todo: additional work to do here?
      state.row.AddActionAccept();
      return true;
    }
    
    // Remove complete items from the state
    uint c = 0;
    while(c < items.size())
    {
      //if(completeRules.find(items[c].ruleIndex) != completeRules.end())    // TODO: THIS IS WRONG!!!!!!!!!!
      if(IsItemComplete(items[c])) 
      {
        items.erase(items.begin()+c);
        continue;
      }
      ++c;
    }
    
    ///////////////////////// TEMPORARY
    //std::cout << "complete:";
    //for(auto i = completeRules.begin(); i != completeRules.end(); ++i)
    //  std::cout << ' ' << *i;
    //std::cout << std::endl;
    //std::cout << "incomplete:";
    //for(auto i = incompleteRules.begin(); i != incompleteRules.end(); ++i)
    //  std::cout << ' ' << *i;
    //std::cout << std::endl;
    ///////////////////////// TEMPORARY

    // Step over complete rules in the remaining incomplete items
    uint nItems = items.size();
    for(uint c = 0; c < nItems; ++c)
    {
      // Check whether the token at the input position corresponds to a complete rule
      // (All complete items have been removed so we do not need to check for a valid input position here)
      if(completeRules.find(items[c].inputPositionRule) != completeRules.end())
      {
        // If the completed item's rule also exists as an incomplete item in the state then make a clone of this item before stepping over the token
        if(incompleteRules.find(items[c].inputPositionRule) != incompleteRules.end())
        {
          items.push_back(items[c]);
          ///////////////////////// TEMPORARY
          std::cout << "copy " << items[c].ruleIndex << std::endl;
          ///////////////////////// TEMPORARY
        }
        
        // Step over the token
        ++items[c].inputPosition;
        items[c].inputPositionRule = uint(-1);
      }
    }
    
    // OLD: Resolve any delays on the stack that may be possible due to the number of rules being reduced
    //ResolveDelays(builder, state);
    
    // Repeat the process (until all items are complete or all rules remain incomplete)
    return CompleteItems(builder, state);
  }
  
  INLINE void GrammarLD::CopyStateUsingPivot(const State& state, State& targetState, ParseToken pivotTerminal) const
  {
    std::vector<bool> copyItemsSubset(state.items.size()); // A vector indicating which items should be copied (which items are relevant)
    std::set<uint> copyRuleIndexes;                        // The set of all rule indexes that are produced by the algorithm
    
    // Get the set of rules that have the pivot terminal as their previous token and fill the set of next rules in order 
    // to perform quick updates
    for(uint c = 0; c < state.items.size(); ++c)
    {
      const auto& item = state.items[c];
      if(item.inputPosition > 0 && GetRuleToken(item.ruleIndex, item.inputPosition-1) == pivotTerminal)
      {
        copyItemsSubset[c] = true;
        copyRuleIndexes.insert(item.ruleIndex);
      }
      else
        copyItemsSubset[c] = false;
    }
    OSI_ASSERT(!rules.empty()); // Post-condition: at least one rule must reference the pivot terminal
    
    // Include items in the copy subset if their input position rule refers to any of the rules already in the subset
    bool copySubsetChanged; // A flag indicating that the copy subset changed (grew during this iteration). The loop is terminated once it can no longer grow.
    do
    {
      // Reset the termination flag
      copySubsetChanged = false;
              
      for(uint c = 0; c < state.items.size(); ++c)
      {
        const auto& item = state.items[c];
        
        // Ignore items that are already in the copy subset
        if(copyItemsSubset[c])
          continue; // The rule is already in the set
        
        // Test whether this rule should be included
        if(copyRuleIndexes.find(item.inputPositionRule) == copyRuleIndexes.end())
          continue; // The rule should not be included (yet)
        
        copyRuleIndexes.insert(item.ruleIndex);
        copyItemsSubset[c] = true;
        copySubsetChanged = true;
      }
    } while(copySubsetChanged);
    
    // Copy the resulting subset of items to the new state
    for(uint c = 0; c < state.items.size(); ++c)
      if(copyItemsSubset[c])
        targetState.items.push_back(state.items[c]);
  }
  
  /*INLINE void GrammarLD::ResolveDelays(BuilderLD& builder, State& leafState, uint leafRule)
  {
    // Determine the index of the leaf state's row in the parse table
    auto leafRowIndex = builder.GetRowIndex(leafState.row);
    OSI_ASSERT(leafRowIndex != ParseToken(-1));
    
    // Start with the leaf rule that was reduced last
    std::stack<uint> ruleResolutionStack; // A stack of the last reduced rules (used to determine the next delayed rule to reduce)
    ruleResolutionStack.push(leafRule);
        
    // Resolve the delayed actions of this state's parents
    for(auto i = leafState.incomingPivots.left.begin(); i != leafState.incomingPivots.left.end(); ++i)
      ResolveDelays(builder, leafState, leafRowIndex, *i->first, ruleResolutionStack);
  }
  
  INLINE void GrammarLD::ResolveDelays(BuilderLD& builder, State& leafState, ParseToken leafRowIndex, State& currentState, std::stack<uint>& ruleResolutionStack)
  {
    // Determine the index of the current state's row in the parse table
    /*auto rowIndex = builder.GetRowIndex(currentState.row);
    OSI_ASSERT(rowIndex != ParseToken(-1));* /
    
    // Add a new row for the coming goto action in this state
    auto& gotoRow = builder.AddActionRow();
    auto gotoRowIndex = builder.GetRowIndex(gotoRow);
    OSI_ASSERT(gotoRowIndex != ParseToken(-1));
    currentState.row.AddActionGoto(leafRowIndex, gotoRowIndex);

    // Resolve each reduction in the state using the leaf state row
    for(auto i = currentState.delayedReductions.rbegin(); i != currentState.delayedReductions.rend(); ++i)
    {
      const auto& delayedRuleMap = *i; // The delayed rule map at the current 

      // Find the last rule in the rule resolution stack that corresponds to the parent of a delayed reduction
      while(true)
      {
        // If the rule resolution stack has run out of elements a new leaf state will need to be found for any remaining reductions
        if(ruleResolutionStack.empty())
          // return;
          break;

        // If the rule in the stack doesn't correspond with any delayed reductions, pop it off of the stack and then try it with the next one
        auto iDelayedRule = delayedRuleMap.find(ruleResolutionStack.top());
        if(iDelayedRule == delayedRuleMap.end())
        {
          ruleResolutionStack.pop();
          continue;
        }

        // The delayed rule can be resolved (add a reduce previous action and continue to the next delayed rule)
        gotoRow.AddActionReducePrev(iDelayedRule->second);
        break;
      }
    }

    // Add a return action to the goto row to end it (todo: when should we add an accept action?)
    gotoRow.AddActionReturn();

    // Resolve the delayed actions of this state's parents
    for(auto i = currentState.incomingPivots.left.begin(); i != currentState.incomingPivots.left.end(); ++i)
      ResolveDelays(builder, leafState, leafRowIndex, *i->first, ruleResolutionStack);
  }*/
  
  /*INLINE void GrammarLD::ResolveDelays(BuilderLD& builder, ResolvedRules& resolvedRules)
  {
    // Determine the index of the leaf state's row in the parse table
    auto leafRowIndex = builder.GetRowIndex(leafState->row);
    OSI_ASSERT(leafRowIndex != ParseToken(-1));
    
    // Add all remaining rules in the leaf state to the stack
    std::stack< std::set<uint> > ruleResolutionStack;
    ruleResolutionStack.push(std::set<uint>());
    for(auto i = leafState.items.begin(); i != leafState.items.end(); ++i)
    {
      ruleResolutionStack.top().insert(i->ruleIndex);
    }
    
    /* Start with the leaf rule that was reduced last
    std::stack<uint> ruleResolutionStack; // A stack of the last reduced rules (used to determine the next delayed rule to reduce)
    ruleResolutionStack.push(leafRule);* /
        
    // Resolve the delayed actions of this state's parents
    for(auto i = leafState.incomingPivots.left.begin(); i != leafState.incomingPivots.left.end(); ++i)
      ResolveDelays(builder, leafRowIndex, *i->first, ruleResolutionStack);
  }
  
  INLINE void GrammarLD::ResolveDelays(BuilderLD& builder, ResolvedRules& resolvedRules, ParseToken leafRowIndex, State& currentState)
  {
    // Determine the index of the current state's row in the parse table
    /*auto rowIndex = builder.GetRowIndex(currentState.row);
    OSI_ASSERT(rowIndex != ParseToken(-1));* /
    
    // Add a new row for the coming goto action in this state
    auto& gotoRow = builder.AddActionRow();
    auto gotoRowIndex = builder.GetRowIndex(gotoRow);
    OSI_ASSERT(gotoRowIndex != ParseToken(-1));
    currentState.row.AddActionGoto(leafRowIndex, gotoRowIndex);

    // Resolve each reduction in the state using the leaf state row
    bool matchedAllRules = true;    // Flag indicating that all the rules on the rule resolution stack could be matched
    for(auto i = currentState.delayedReductions.rbegin(); i != currentState.delayedReductions.rend(); ++i)
    {
      const auto& delayedRuleMap = *i; // The delayed rule map at the current 

      // Find the last rule in the rule resolution stack that corresponds to the parent of a delayed reduction
      while(true)
      {
        // If the rule resolution stack has run out of elements a new leaf state will need to be found for any remaining reductions
        if(ruleResolutionStack.empty())
          // return;
          break;

        // If the rule in the stack doesn't correspond with any delayed reductions, pop it off of the stack and then try it with the next one
        auto iDelayedRule = delayedRuleMap.find(ruleResolutionStack.top());
        if(iDelayedRule == delayedRuleMap.end())
        {
          ruleResolutionStack.pop();
          continue;
        }

        // The delayed rule can be resolved (add a reduce previous action and continue to the next delayed rule)
        gotoRow.AddActionReducePrev(iDelayedRule->second);
        break;
      }
    }

    // Add a return action to the goto row to end it (todo: when should we add an accept action?)
    gotoRow.AddActionReturn();

    // Resolve the delayed actions of this state's parents
    for(auto i = currentState.incomingPivots.left.begin(); i != currentState.incomingPivots.left.end(); ++i)
      ResolveDelays(builder, leafRowIndex, *i->first, ruleResolutionStack);
  }*/
  
  INLINE bool GrammarLD::GenerateCyclicPivot(BuilderLD& builder, State& state, const ParseTokenSet& terminals)
  {
    // Try to find an state with a path to this state that has identical end-state.
    // For this we only take the remaining tokens in items into account that have not been stepped over
    
    State* prevState = DetectCycle(state);
    if(prevState == null)
      return false;
    
    // Generate the cyclic pivot 
    auto& pivots = state.row.AddActionPivot();
    for(auto i = terminals.begin(); i != terminals.end(); ++i)
    {
      ////////////////////////////// TEMPORARY
      //std::cout << "in state: " << builder.GetRowIndex(prevState->row) << std::endl;
      //std::cout << "terminal: " << *i << std:: endl;
      //std::cout << "all terminals: ";
      //for(auto iTerminal = prevState->outgoingPivots.right.begin(); iTerminal !=prevState->outgoingPivots.right.end(); ++iTerminal)
      //  std::cout << iTerminal->first;
      //std::cout << std::endl;
      ////////////////////////////// TEMPORARY
      auto& targetState = *prevState->outgoingPivots.right.at(*i);
      
      // Add the edge to both states
      state.outgoingPivots.insert(LDState::PivotEdge(&targetState, *i));
      targetState.incomingPivots.insert(LDState::PivotEdge(&state, *i));

      // Add pivot the pivot to the parse table
      ParseToken targetRow = builder.GetRowIndex(targetState.row);
      pivots.AddPivot(*i, targetRow); 
    }
    
    return true;
  }
  
  INLINE LDState* GrammarLD::DetectCycle(State& lastState)
  {
    // Check whether any of the states leading to this state forms a cycle 
    for(auto i = lastState.incomingPivots.left.begin(); i != lastState.incomingPivots.left.end(); ++i)
    {
      State* prevState = DetectCycle(*i->first, lastState);
      if(prevState)
      {
        // Cycle found
        ++prevState->cyclicNestingDepth;
        ++lastState.cyclicNestingDepth;
        return prevState;  // cycle found
      }
    }
    
    // No candidates could be found
    return null;
  }
  
  INLINE LDState* GrammarLD::DetectCycle(State& currentState, const State& lastState)
  {
    // Check whether the current state is compatible with the last state. If so we may form a cycle
    if(CompareEndStates(currentState, lastState))
      return &currentState;
    
    // If there are no more states leading to this state, then no cycle could be found
    if(currentState.incomingPivots.empty())
      return null;
    
    // Check whether any of the states leading to this state forms a cycle 
    for(auto i = currentState.incomingPivots.left.begin(); i != currentState.incomingPivots.left.end(); ++i)
    {
      State* prevState = DetectCycle(*i->first, lastState);
      if(prevState)
      {
        // Cycle found
        ++prevState->cyclicNestingDepth;
        return prevState;
      }
    }
    
    // No candidates could be found
    return null;
  }
    
  INLINE bool GrammarLD::CompareEndStates(const State& state1, const State& state2) const
  {
    // Compare the remaining items in the two states to see whether they have exactly the same end-state.
    // TODO: This algorithm isn't very optimized for the case where many items are similar
    //       It might be possible to use some form of hashing in the future for speeding up this comparison.
    for(auto i1 = state1.items.begin(); i1 != state1.items.end(); ++i1)
    {
      bool itemFound = false;
      for(auto i2 = state2.items.begin(); i2 != state2.items.end(); ++i2)
      {
        if(i1->ruleIndex == i2->ruleIndex
          && i1->inputPosition == i2->inputPosition
          && i1->inputPositionRule == i2->inputPositionRule)
        {
          itemFound = true;
          break;
        }
      }
      
      if(!itemFound)
        return false;  // The two end-states are not identical
    }
    return true; // The two end-states are identical
  }
  
  INLINE void GrammarLD::GeneratePivot(BuilderLD& builder, State& state/*, ResolvedRules& resolvedRules*/, const ParseTokenSet& terminals)
  {
    // Generate a pivot for each state
    auto& pivots = state.row.AddActionPivot();
    for(auto i = terminals.begin(); i != terminals.end(); ++i)
    {          
      // Copy the state and generate a new line in the action table to couple with it
      states.push_back(new State(pivots.AddPivot(*i)));
      auto& targetState = *states.back();
      CopyStateUsingPivot(state, targetState, *i);

      // Add the edge to both states
      state.outgoingPivots.insert(LDState::PivotEdge(&targetState, *i));
      targetState.incomingPivots.insert(LDState::PivotEdge(&state, *i));
    }
      
    // Continue building each state graph starting from the pivot
    for(auto i = state.outgoingPivots.begin(); i != state.outgoingPivots.end(); ++i)
    {
      auto& targetState = *i->left;
              
      // Complete all rules that are now finished (in the target state)
      bool allItemsComplete = CompleteItems(builder, targetState);
      
      // Stop if there are no more items in this state to complete
      if(allItemsComplete)
        continue; // No items left to complete
      
      /* Since we've reached a decision point:
      // Resolve all delays that are possible and set the new state as the current leaf node
      leafTarget = &targetState;      
      if(ResolveDelays(builder, state, resolvedRules))
        return;*/
      
      // Recursively build state graph for this new state
      ConstructStateGraph(builder, targetState);
    }
  }
  
  INLINE void GrammarLD::ResolveDelays(BuilderLD& builder, State& rootState)
  {
    // todo: it might be possible to optimize this slightly by moving forward gotos up to just before the pivot where the cyclic nesting depth
    // changes (in at least one of the outgoing states) ... we'll leave this for the future..
    
    // Set the delays checked flag (to terminate loops that would be infinite)
    if(rootState.delaysChecked)
      return;
    rootState.delaysChecked = true;
    
    // Resolve the delays in this state
    for(auto i = rootState.delayedReductions.rbegin(); i != rootState.delayedReductions.rend(); ++i)
    {
      for(auto iPivotEdge = rootState.outgoingPivots.begin(); iPivotEdge != rootState.outgoingPivots.end(); ++iPivotEdge)
      {
        std::stack<uint> ruleResolutionStack; // A stack of rule indices
        ResolveDelayedReduction(builder, rootState, *iPivotEdge->left, i, ruleResolutionStack);
      }
    }
    
    // Resolve all the delays of the child states
    for(auto iPivotEdge = rootState.outgoingPivots.begin(); iPivotEdge != rootState.outgoingPivots.end(); ++iPivotEdge)
      ResolveDelays(builder, *iPivotEdge->left);
    
    // Reset the delays checked flag
    rootState.delaysChecked = false;
  }
  
  INLINE void GrammarLD::ResolveDelayedReduction(BuilderLD& builder, State& rootState, State& state, DelayedRuleStack::const_reverse_iterator iDelayedRules, std::stack<uint>& ruleResolutionStack)
  {
    // Set the delays checked flag (to terminate loops that would be infinite)
    if(state.delaysChecked)
      return;
    state.delaysChecked = true;
    
    // Test whether the end-state can resolve the current set of delayed rules
    const auto& delayedRules = *iDelayedRules;
    std::set<uint> validReductions;  // The reductions that are possible for this set of rules
    bool ruleNotPresent = false;     // Flag indicating that a rule in the end-state is not present in the rule map, hence 
    for(auto i = state.items.begin(); i != state.items.end(); ++ i)
    {
      auto iDelay = delayedRules.find(i->ruleIndex);
      if(iDelay == delayedRules.end())
      {
        ruleNotPresent = true;
        break;
      }
      
      // Add the reduction to the set of reductions that can be resolved from this state
      validReductions.insert(iDelay->second);
    }
     
    
    if(ruleNotPresent || validReductions.size() != 1)
    {
      ////////////////////////////// TEMPORARY
      std::cout << "Could not resolve reduction in state: " << builder.GetRowIndex(state.row) << std::endl;
      std::cout << "  Rules:";
      for(auto i = state.items.begin(); i != state.items.end(); ++ i)
        std::cout << ' ' << i->ruleIndex;
      std::cout << std::endl;
      std::cout << "  Delays:";
      for(auto i = delayedRules.begin(); i != delayedRules.end(); ++i)
        std::cout << ' ' << i->first << "->" << i->second;
      std::cout << std::endl;
      std::cout << "  Valid reductions:";
      for(auto i = validReductions.begin(); i != validReductions.end(); ++i)
        std::cout << ' ' << *i;
      std::cout << std::endl;
      ////////////////////////////// TEMPORARY
      
      // Could not resolve the reduction, continue using the next pivots
      for(auto iPivotEdge = state.outgoingPivots.begin(); iPivotEdge != state.outgoingPivots.end(); ++iPivotEdge)
        ResolveDelayedReduction(builder, rootState, *iPivotEdge->left, iDelayedRules, ruleResolutionStack);
    }
    else
    {
      ////////////////////////////// TEMPORARY
      std::cout << "Resolved reduction in state: " << builder.GetRowIndex(state.row) << std::endl;
      ////////////////////////////// TEMPORARY
      
      /*todo: try this:
      // The reduction can be resolved
      std::stack<uint> ruleResolutionStack;
      ruleResolutionStack.push(*validReductions.begin());
      
      // If the root state and the leaf state are in different cycles, then we must generate goto action to resolve states
      if()*/
      
      state.row.AddActionReducePrev(*validReductions.begin());
    }
    
    // Reset the delays checked flag
    state.delaysChecked = false;

  }
}

#endif
#endif
