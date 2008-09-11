#ifdef  __QPARSER_GRAMMARLR1_H__
#ifndef __QPARSER_GRAMMARLR1_INL__
#define __QPARSER_GRAMMARLR1_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMARLR1.INL
//
//    Copyright © 2007-2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QParser
{
  GrammarLR1::~GrammarLR1()
  {
    // Clean up memory
    for (vector<State*>::iterator i = states.begin(); i != states.end(); ++i)
      delete *i;
  }

  void GrammarLR1::constructProductions()
  {
    // Construct the parse table using closure / goto algorithms
    constructParseTable();
  }

  void GrammarLR1::parse(ParseResult& parseResult)
  {
    vector<ParseMatch> matches;

    if(binaryParseTable.size() == 0)
      return;

    // Traverse binary sorted parse table (tight parser inner-loop)
    uint cToken = 0;      // The current token index
    uint cTable = 0;      // The current table index
    uint cTableStart = 0; //
    uint cMatch = 0;      // The current match index (This is sometimes retrieved from the stateIndexStack)
    OSid token;           // The last read token
    vector< pair<uint, uint> > stateIndexStack; // State indexes stack <table index, match index> used with reduce actions
    //int matchLengthModifier = 0; // Number of skipped (silent) tokens / added tokens (due to skipped nonterminals) during
                                 // a reduce action.
                                 // This is used to determine correct match lengths...
    //int childMatchLength = 0; // match length of the previous reduced production. This is added to the match length modifier iff the
                              // parent (the current one directly following the child) reduction is silent

    while(true)
    {
      token = (cToken < parseResult.lexStream.length)? parseResult.lexStream.data[cToken].id : ID_SPECIAL;

    skipTokenInitialization:

      const BinaryIndexElement& parseElement = binaryParseTable[cTable];
      if(parseElement.action == BinaryIndexElement::LRACTION_ERROR)
      {
        errorStream << "Unexpected token, at line [???] in program [???]" << endl;

#ifdef _DEBUG // TEMPORARY
        errorStream << cTable << '\t' << getTokenName(token) << "\terror" << endl;
#endif
        //sprintf(
        //outputError( "Unexpected token, %token, expected one of %[token]", token);
        // todo: NB! perform error handling here
        break;
      }
      else if(token == parseElement.id)
      {
        // Old: Reduce and goto action must not eat the current input token. (This is because during these actions we are actually
        //      doing lookahed... Only shift actions must eat input tokens.
        // ++cToken;

        switch(parseElement.action & 7) // test least significant 4 bits for the action
        {
          case BinaryIndexElement::LRACTION_SHIFT:
            {
              // Handle identifier declarations / references
              if(token == ID_IDENTIFIER)
              {
                //if(parseElement == BinaryIndexElement::LRACTION_SHIFTDECL) declareIdentifier(token, type);
                //else if(!referenceIdentifier(token, type)) error;
              }

              // Eat the token from the lex input stream
              ++cToken;

              // Get the current match index (position in the output stream)
              cMatch = matches.size();

#ifdef _DEBUG // TEMPORARY
              errorStream << cTable << '\t' << getTokenName(token) << "\tshift(" << parseElement.param << ")" << endl;
#endif

              // Push current table counter and match index onto the stack (until the production is reduced later)
              stateIndexStack.push_back(std::make_pair(cTableStart, cMatch));
              cTableStart = cTable = parseElement.param;

              // Push the lex token onto the output stream
              // todo: do we need to check whether token is -1 and skip it here? (can this happen?)
              
              //matches.push_back(ParseMatch(cToken-1, 0, token));
              matches.push_back(ParseMatch(cToken-1, (cToken < parseResult.lexStream.length)? parseResult.lexStream.data[cToken-1].length : 0, token));
            }
            break;

          case BinaryIndexElement::LRACTION_GOTO:
            {
#ifdef _DEBUG //TEMPORARY
              errorStream << cTable << '\t' << getTokenName(token) << "\tgoto(" << parseElement.param << ")" << endl;
#endif

              // Push current table counter onto the stack (until the production is reduced later)
              // Note: for the match index we use the same one carried forward from the previous reduce action
              stateIndexStack.push_back(std::make_pair(cTableStart, cMatch));
              cTableStart = cTable = parseElement.param;
            }
            break;

          case BinaryIndexElement::LRACTION_ACCEPT:
          case BinaryIndexElement::LRACTION_REDUCE:
            {
              if(token == ID_IDENTIFIER)
              {
                //if(parseElement == BinaryIndexElement::LRACTION_REDUCEDECL) declareIdentifier(token, type);
                //else if(!referenceIdentifier(token, type)) error;
              }

              Production& production = productions[parseElement.param].first;

#ifdef _DEBUG //TEMPORARY
              errorStream << cTable << '\t' << getTokenName(token);
#endif
              // Replace token with production id being reduced to
              token = productions[parseElement.param].second;
              
              // We do an accept action only if the stack is empty. If not the start symbol is recursive which
              // implies that we should reduce until the state index stack is empty.
              if((parseElement.action & 7) == BinaryIndexElement::LRACTION_ACCEPT && stateIndexStack.size() == production.symbolsLength)
              {
                // Insert the root (starting symbol) production match (non-terminal) into the output stream
#ifdef _DEBUG   //TEMPORARY
                errorStream << "\taccept(" << getTokenName(productions[parseElement.param].second) << ")" << endl;
#endif
                //matches.insert(matches.begin(), ParseMatch(0, production.symbolsLength + matchLengthModifier, productions[parseElement.param].second));
                matches.insert(matches.begin(), ParseMatch(0, production.symbolsLength, productions[parseElement.param].second));
                //matchLengthModifier = 0;
                break;
              }

              // Pop n table counters off the stack (pushed by the last shift/goto actions) where
              // n is the number of symbols in the production
              const pair<uint, uint> stateIndex = stateIndexStack[stateIndexStack.size() - production.symbolsLength];
              stateIndexStack.resize(stateIndexStack.size() - production.symbolsLength);

              cTableStart = cTable = stateIndex.first;
              cMatch = stateIndex.second;

#ifdef _DEBUG //TEMPORARY
              errorStream << "\treduce(" << stateIndex.first << ", " << getTokenName(productions[parseElement.param].second) << ")";
              if (parseElement.action & BinaryIndexElement::LRACTION_FLAG_SILENT)
                errorStream << " *silent*" << endl;
              //errorStream << endl;
#endif

              // Insert the production match (non-terminal) into the output stream
              // todo: get the correct length and offset (i.e. should the length actually include
              // children of nonterminals?) Also what is the offset exactly?
              //OLDER: matches.push_back(ParseMatch(0, production.symbolsLength, token));
              //OLD: matches.insert(matches.begin() + cMatch, ParseMatch(0, production.symbolsLength, token));

              if(!(parseElement.action & BinaryIndexElement::LRACTION_FLAG_SILENT))
              {
                matches.insert(matches.begin() + cMatch, ParseMatch(0, production.symbolsLength, token));

#ifdef _DEBUG //TEMPORARY
                //errorStream << " <match length += " << matchLengthModifier << ">";
                errorStream << endl;
#endif

                /*// Reset the number of skipped tokens for the next iteration
                childMatchLength = production.symbolsLength + match;
                matchLengthModifier = 0;*/
              }
              else
              {
                /*// Add the production's tokens to the match length modifier and subtract the nonterminal token from the modifier
                OSI_ASSERT(production.symbolsLength == 1);
                matchLengthModifier += childMatchLength - 1;
                childMatchLength = 1;

                matchLengthModifier = 0;*/
              }
            }
            goto skipTokenInitialization;

          /*case BinaryIndexElement::LRACTION_ACCEPT:
            {
              //Note: We could already insert this match (the root) right in the beginning before-hand as a little optimization,
              //      but we'd need to set its length here in any case based on the production.
              //      We're simply doing it here for clarity & consistency.

              // Insert the production match (non-terminal) into the output stream
              Production& production = productions[parseElement.param].first;

#ifdef _DEBUG //TEMPORARY
              errorStream << cTable << '\t' << getTokenName(token) << "\taccept(" << getTokenName(productions[parseElement.param].second) << ")" << endl;
              //errorStream << cTable << '\t' << getTokenName(token) << "\taccept" << endl;
#endif
              matches.insert(matches.begin(), ParseMatch(0, production.symbolsLength, productions[parseElement.param].second));
            }
            break;*/

          default:
            OSI_ASSERT(false); // unknown action in parse table
            break;
        }

        if((parseElement.action & 7) == BinaryIndexElement::LRACTION_ACCEPT)
          break;
      }
      else if(token > parseElement.id)
      {
        cTable = parseElement.largerIndex;
        goto skipTokenInitialization;
      }
      else
      {
        ++cTable;
        goto skipTokenInitialization;
      }
    }

    // Copy array to parse result
    parseResult.globalMatchesLength = 1;
    parseResult.parseStream.length = matches.size();
    parseResult.parseStream.data = new ParseMatch[parseResult.parseStream.length];

    cMatch = 0;
    for(vector<ParseMatch>::iterator i = matches.begin(); i != matches.end(); ++i, ++cMatch)
    {
      if(!isTerminal(i->id))
        i->offset = cMatch + 1;
      parseResult.parseStream.data[cMatch] = *i;
    }

    /*OLD:// Recursively reverse the matches and write to the array (to get matches into the correct order)

    uint cMatch = 0;
    for(vector<ParseMatch>::reverse_iterator i = matches.rbegin(); i != matches.rend();)
    {
      parseResult.matches[cMatch] = *i;
      ++cMatch; ++i;

      for(uint c = 0; c < i->length; )
      {
        if(!isTerminal(i->id) && i->length > 0)
          i->offset = cMatch+1; // assign the next token's index as the offset to the match
        cMatch = ;
      }
    }*/
  }

  void GrammarLR1::constructParseTable()
  {
    if(startSymbol == OSid(-1))
    {
      if(nextNonterminal == 0) return; // error: no productions defined
      else startSymbol = nextNonterminal - 2;
    }

    //OSI_ASSERT(startProductionSet);

    // Algorithm: The LR parsing algorithm from [modern compiler implementation in Java (ISBN 0-521-82060-X)] has been used
    //            states represents the set of states listed as T which is the set of states seen so far
    //            State::edges represents the set of edges listed as E which is the set of shift or goto edges found so far

    // Reset all production sets' nullity flags (see getFirstTerminals(...) for an explanation)
    for(map<OSid, ProductionSet*>::iterator i = productionSets.begin(); i != productionSets.end(); ++i)
    {
      i->second->nullable = false;
      i->second->visitedCount = 0;
    }

    // Initialize T to { Closure( { Start Production } ) }
    states.push_back(new State);
    getStartItems(startSymbol, states[0]->items);
    closure(states[0]->items);

    // Evaluate each state until no new states are found
    goTo(states);
    constructBinaryLR1Table();
  }

  void GrammarLR1::getStartItems(OSid nonterminal, vector<Item> &items)
  {
    OSI_ASSERT(!isTerminal(nonterminal));
    ProductionSet* productionSet = getProductionSet(nonterminal);

    Item item;
    item.inputPosition = 0;
    item.productionId = nonterminal;
    item.lookaheadSymbol = -1;

    for(uint cProduction = 0; cProduction < productionSet->productionsLength; ++cProduction)
    {
      item.productionIndex = productionSet->productionsOffset + cProduction;

      // Add the item to the set if not already present
      // todo: (optimization) use some form of index for this... (unfortunately stl doesn't allow multiple indexes...)
      vector<Item>::iterator i;
      for(i = items.begin(); i != items.end(); ++i) if (*i == item) break;
      if(i == items.end())
        items.push_back(item);
    }
  }

  void GrammarLR1::getLookaheadStartItems(OSid nonterminal, vector<Item> &items, const Item& parentItem)
  {
    OSI_ASSERT(!isTerminal(nonterminal));
    ProductionSet& productionSet = *getProductionSet(nonterminal);

    // Get the lookahead symbols by determining all possible terminals following the current nonterminal
    // in the parent item
    set<OSid> lookaheadTerminals;
    getLookaheadTerminals(parentItem, lookaheadTerminals);

    // Get all the items starting the nonterminal with lookahead terminals apended
    Item item;
    item.inputPosition = 0;
    item.productionId = nonterminal;

    for(uint cProduction = 0; cProduction < productionSet.productionsLength; ++cProduction)
    {
      item.productionIndex = productionSet.productionsOffset + cProduction;

      for(set<OSid>::iterator i = lookaheadTerminals.begin(); i != lookaheadTerminals.end(); ++i)
      {
        item.lookaheadSymbol = *i;

        // Add the item to the set if not already present
        // todo: (optimization) use some form of index for this... (unfortunately stl doesn't allow multiple indexes...)
        vector<Item>::iterator i;
        for(i = items.begin(); i != items.end(); ++i)
          if(*i == item)
            break;
        if(i == items.end())
          items.push_back(item);
      }
    }
  }

  void GrammarLR1::getLookaheadTerminals(const Item& item, set<OSid>& lookaheadTerminals)
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

  bool GrammarLR1::getFirstTerminals(OSid id, set<OSid>& firstTerminals)
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

  void GrammarLR1::closure(vector<Item> &items)
  {
    closure(items, 0, items.size());
  }

  void GrammarLR1::closure(vector<Item> &items, uint cBegin, uint cEnd)
  {
    for(uint c = cBegin; c < cEnd; ++c)
    {
      Item& item = items[c];
      const Production& production = productions[item.productionIndex].first;

      // Check whether the last symbol has been passed
      if(item.inputPosition >= production.symbolsLength)
        continue;

      OSid symbol = production.symbols[item.inputPosition].id;

      // Check whether the symbol at input position is a terminal symbol
      if(isTerminal(symbol))
        continue;

      // Add the closure of the nonterminal at input position to the set
      // (but first construct a starting item set for the production)
      getLookaheadStartItems(symbol, items, item);
    }

    if(cEnd < items.size())
      closure(items, cEnd, items.size());
  }

  void GrammarLR1::goTo(vector<State*>& states)
  {
    goTo(states, 0, states.size());
  }

  void GrammarLR1::goTo(vector<State*>& states, uint cBegin, uint cEnd)
  {
    for(uint cState = cBegin; cState < cEnd; ++cState)
    {
      State& state = *states[cState];

      for(uint cItem = 0; cItem < state.items.size(); ++cItem)
      {
        const Item& item = state.items[cItem];
        const Production& production = productions[item.productionIndex].first;

        // Check whether the last symbol has already been passed
        if(item.inputPosition >= production.symbolsLength)
          continue;

        OSid symbol = production.symbols[item.inputPosition].id;

        //Note: we remap ID_IDENTIFIER_DECL and ID_IDENTIFIER_REF to be inserted into the map at ID_IDENTIFIER position
        if(symbol == ID_IDENTIFIER_DECL || symbol == ID_IDENTIFIER_REF) symbol = ID_IDENTIFIER;

        // Construct goto item
        Item goToItem = item;
        goToItem.inputPosition = item.inputPosition + 1;

        // Check whether there's already a state defined for this goto item
        int targetStateIndex = findItemState(item);

        // Check whether there's already an edge for this symbol in the current state
        State::Edges::iterator iEdge = state.edges.find(symbol);

        if(iEdge == state.edges.end() || iEdge->first != symbol)
        {
          if(targetStateIndex == -1)
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
        else if(targetStateIndex == -1)
        {
          // (There's already an edge for the symbol (due to another item), but the corresponding goto item does not exist in the target state yet)
          targetStateIndex = iEdge->second;
        }
        else
          continue; // (There already exists a goto item corresponding to the item in the target state)

        State& targetState = *states[targetStateIndex];

        // Note: we still have to check whether the state already contains the goto item since it could have
        // been added by the closure of another item in the state
        vector<Item>::iterator i;
        //bug: for(i = targetState.items.begin(); i != targetState.items.end(); ++i) if(*i == item) break;
        for(i = targetState.items.begin(); i != targetState.items.end(); ++i) if(*i == goToItem) break;
        if(i != targetState.items.end()) continue;

        // Add goto item to set (J) (the target state)
        targetState.items.push_back(goToItem);

        // Add closure of goto item to the set (Note that the last item added to the targetState is the goto item we're refering to)
        closure(targetState.items, targetState.items.size()-1, targetState.items.size());
      }
    }

    if(cEnd < states.size())
      goTo(states, cEnd, states.size());
  }

  int GrammarLR1::findItemState(const Item& item)
  {
    map<Item, uint>::iterator i = itemStateIndex.find(item);
    //if (OLD: i == itemStateIndex.end())
    if(i == itemStateIndex.end() || i->first != item)
      return -1;
    return i->second;
  }

  multimap<OSid, OSid>::const_iterator GrammarLR1::findPrecedenceDirective(OSid token1, OSid token2) const
  {
    multimap<OSid, OSid>::const_iterator i = precedenceMap.lower_bound(token1);
    while(i != precedenceMap.end() && i->first == token1)
    {
      if(i->second == token2)
        return i;
      ++i;
    }
    return precedenceMap.end();
  }

  void GrammarLR1::constructBinaryLR1Table()
  {
    vector<uint> stateTableOffsets; // state-to-table offset mapping

    //// Construct an LR parsing table as serialized (flatened) binary tree
    for(vector<State*>::const_iterator iState = states.begin(); iState != states.end(); ++iState)
    {
      // todo: The contents of this loop can be optimized by directly using a binary tree if necessary

      const State& state = **iState;

      stateTableOffsets.push_back(binaryParseTable.size());

      // Determine all the binary index elements corresponding to the state
      map<OSid, BinaryIndexElement> elementsSortedLinear;

      // Add reduce/accept actions to the elements
      for(vector<Item>::const_iterator iItem = state.items.begin(); iItem != state.items.end(); ++iItem)
      {
        const Item& item = *iItem;
        const Production& production = productions[item.productionIndex].first;
        //const ProductionSet& productionSet = *getProductionSet(productions[item.productionIndex].second);

        if(item.inputPosition == production.symbolsLength)
        {
          //Note: we remap ID_IDENTIFIER_DECL and ID_IDENTIFIER_REF to be inserted into the map at ID_IDENTIFIER position
          BinaryIndexElement element;
          element.id = item.lookaheadSymbol;
          if(element.id == ID_IDENTIFIER_DECL || element.id == ID_IDENTIFIER_REF) element.id = ID_IDENTIFIER;

          // OLD: (NOT): (item.productionId == startSymbol && item.lookaheadSymbol == ID_SPECIAL) is not sufficient arguments
          //       to say that the action is an accept action
          //       (This is not true, we simply need to check the stack when an accept action is reached. If the stack is not empty, we still reduce anyway).

          //OLD:
          element.action = (item.productionId == startSymbol && item.lookaheadSymbol == static_cast<OSid>(ID_SPECIAL))? BinaryIndexElement::LRACTION_ACCEPT : BinaryIndexElement::LRACTION_REDUCE;
          /*if(item.productionId == startSymbol && item.lookaheadSymbol == ID_SPECIAL)
          {
            element.action = BinaryIndexElement::LRACTION_ACCEPT;
          }
          else
          {
            // We don't emit tokens for productions that have only a single non-terminal token since this is redundant for the front-end coder
            //if(production.symbolsLength == 1 && !isTerminal(production.symbols[0].id))
             element.action = isSilent(production)? BinaryIndexElement::LRACTION_REDUCE_SILENT : BinaryIndexElement::LRACTION_REDUCE;
          }*/
          if(isSilent(production))
            element.action = static_cast<BinaryIndexElement::Action>(element.action | BinaryIndexElement::LRACTION_FLAG_SILENT);

          element.param = item.productionIndex;
          element.largerIndex = 0;

          // Check for shift-reduce / reduce-reduce conflicts...
          map<OSid, BinaryIndexElement>::iterator iFoundElement = elementsSortedLinear.find(element.id);
          if(iFoundElement != elementsSortedLinear.end())
          {
            BinaryIndexElement& foundElement = iFoundElement->second;
#ifdef _DEBUG
            OSid foundId = iFoundElement->first;
#endif
            switch(foundElement.action & 7)
            {
              case BinaryIndexElement::LRACTION_SHIFT:
              case BinaryIndexElement::LRACTION_GOTO:
              {
                OSid shiftId = foundElement.id;
                OSid reduceId = item.productionId;

                multimap<OSid, OSid>::const_iterator i1 = findPrecedenceDirective(shiftId, reduceId);
                multimap<OSid, OSid>::const_iterator i2 = findPrecedenceDirective(reduceId, shiftId);
                if((i1 == precedenceMap.end() && i2 == precedenceMap.end())
                  || (i1 != precedenceMap.end() && i2 != precedenceMap.end()))
                  cout << "FATAL ERROR: ";
                else
                  cout << "WARNING: ";

                cout << "Shift-reduce conflict at line(?\?)" << endl;

                cout << "  Shift: " << getTokenName(shiftId) << endl;

                cout << "  Reduce: ";
#ifdef _DEBUG
                debugOutputItem(item);
#endif
                cout << " (current item)" << endl;

                if(i1 == precedenceMap.end() && i2 == precedenceMap.end())
                  cout << "\t(No precedence rule defined with " << getTokenName(shiftId) << ", " << getTokenName(reduceId) << ')' << endl;
                else if(i1 != precedenceMap.end() && i2 != precedenceMap.end())
                  cout << "\t(Conflicting precedence rules: "
                       << getTokenName(i1->first) << " < " << getTokenName(i1->second) << " ; "
                       << getTokenName(i2->first) << " < " << getTokenName(i2->second) << ')' << endl;
                else
                {
                  cout << "\t(Resolved by precedence rule: ";
                  if(i1 != precedenceMap.end())
                    cout << getTokenName(i1->first) << " < " << getTokenName(i1->second);
                  else
                    cout << getTokenName(i2->first) << " < " << getTokenName(i2->second);
                  cout << ')' << endl;

                  if(i2 != precedenceMap.end())
                    foundElement = element;
                }

                cout << endl;

                break;
              }

              case BinaryIndexElement::LRACTION_REDUCE:
              case BinaryIndexElement::LRACTION_ACCEPT:
              {
                if(foundElement.param != element.param)
                {
                  cout << "FATAL ERROR: Reduce-reduce conflict at line(?\?)" << endl;

                  cout << "  Reduce 1: ";
#ifdef _DEBUG
                  debugOutputProduction(productions[foundElement.param].second, productions[foundElement.param].first);
#endif
                  cout << endl;

                  cout << "  Reduce 2: ";
#ifdef _DEBUG
                  debugOutputItem(item);
#endif
                  cout << " (current item)" << endl;
                  cout << endl;
                }
                break;
              }

              default:
                OSI_ASSERT(false);
            }
          }
          else
            elementsSortedLinear.insert(pair<OSid, BinaryIndexElement>(element.id, element));
        }
      }

      // Add shift/goto actions to the elements
      for(State::Edges::const_iterator iEdge = state.edges.begin(); iEdge != state.edges.end(); ++iEdge)
      {
        //OLD:  we remap ID_IDENTIFIER_DECL and ID_IDENTIFIER_REF to be inserted into the map at ID_IDENTIFIER position
        //      (NO LONGER NECESARY FOR SHIFT ACTIONS)
        BinaryIndexElement element;
        element.id = iEdge->first;
        //if(element.id == ID_IDENTIFIER_DECL || element.id == ID_IDENTIFIER_REF) element.id = ID_IDENTIFIER;
        element.action = isTerminal(element.id)? BinaryIndexElement::LRACTION_SHIFT : BinaryIndexElement::LRACTION_GOTO;
        //element.action |= iEdge->
        element.param = iEdge->second; // param = state index
        element.largerIndex = 0;

        // Check for shift-reduce conflicts...
        map<OSid, BinaryIndexElement>::iterator iFoundElement = elementsSortedLinear.find(element.id);
        if(iFoundElement != elementsSortedLinear.end())
        {
          BinaryIndexElement& foundElement = iFoundElement->second;

          switch(foundElement.action & 7)
          {
            case BinaryIndexElement::LRACTION_SHIFT:
            case BinaryIndexElement::LRACTION_GOTO:
            {
              cout << "FATAL ERROR: Shift-shift conflict at line(?\?) <-- WARNING: THIS SHOULD BE IMPOSSIBLE!" << endl;
              cout << "  Shift: " << getTokenName(element.id) << " (current item)" << endl;
              cout << "  Shift: " << getTokenName(iFoundElement->first) << endl;
              cout << endl;
              break;
            }

            case BinaryIndexElement::LRACTION_REDUCE:
            case BinaryIndexElement::LRACTION_ACCEPT:
            {
              OSid reduceId = productions[foundElement.param].second;
              OSid shiftId = element.id;

              multimap<OSid, OSid>::const_iterator i1 = findPrecedenceDirective(reduceId, shiftId);
              multimap<OSid, OSid>::const_iterator i2 = findPrecedenceDirective(shiftId, reduceId);
              if((i1 == precedenceMap.end() && i2 == precedenceMap.end())
                || (i1 != precedenceMap.end() && i2 != precedenceMap.end()))
                cout << "FATAL ERROR: ";
              else
                cout << "WARNING: ";

              cout << "Shift-reduce conflict at line(?\?)" << endl;
              cout << "  Reduce: ";
#ifdef _DEBUG
              debugOutputProduction(reduceId, productions[foundElement.param].first);
              cout << ", " << getTokenName(element.id);
#endif
              cout << endl;

              cout << "  Shift: " << getTokenName(element.id) << " (current item)" << endl;

              // (Output all the corresponding shift items in the state)
              for(vector<Item>::const_iterator iItem = state.items.begin(); iItem != state.items.end(); ++iItem)
              {
                const Item& item = *iItem;
                const Production& production = productions[item.productionIndex].first;

                if(item.inputPosition >= production.symbolsLength || production.symbols[item.inputPosition].id != element.id)
                  continue;

                cout << "\t\t";
#ifdef _DEBUG
                //debugOutputProduction(production);
                debugOutputItem(item);
#endif
                cout << endl;
              }

              if(i1 == precedenceMap.end() && i2 == precedenceMap.end())
                cout << "\t(No precedence rule defined with " << getTokenName(reduceId) << ", " << getTokenName(shiftId) << ')' << endl;
              else if(i1 != precedenceMap.end() && i2 != precedenceMap.end())
                cout << "\t(Conflicting precedence rules: "
                     << getTokenName(i1->first) << " < " << getTokenName(i1->second) << " ; "
                     << getTokenName(i2->first) << " < " << getTokenName(i2->second) << ')' << endl;
              else
              {
                cout << "\t(Resolved by precedence rule: ";
                if(i1 != precedenceMap.end())
                  cout << getTokenName(i1->first) << " < " << getTokenName(i1->second);
                else
                  cout << getTokenName(i2->first) << " < " << getTokenName(i2->second);
                cout << ')' << endl;

                if(i2 != precedenceMap.end())
                    foundElement = element;
              }

              cout << endl;

              break;
            }
            
            default:
              // Post-condition: Check for invalid parse action
              OSI_ASSERT(false);
          }
        }
        else
            elementsSortedLinear.insert(pair<OSid, BinaryIndexElement>(element.id, element));
      }

      // Resort elements using binary ordering
      vector<BinaryIndexElement> elementsSortedBinary;
      //OLD: uint index;

      struct SortBin
      { SortBin(vector<BinaryIndexElement>& elements, map<OSid, BinaryIndexElement>::iterator iBegin, map<OSid, BinaryIndexElement>::iterator iEnd, int size)
        {
          int midIndex = size>>1;

          map<OSid, BinaryIndexElement>::iterator iMid = iBegin;
          for(uint c = 0; int(c) < midIndex; ++c) ++iMid;

          //BinaryIndexElement& midElement = elements.push_back((begin+midIndex)->second);
          elements.push_back(iMid->second);
          uint elementIndex = elements.size()-1;                // Store the current element index

          if(midIndex > 0)
            SortBin(elements, iBegin, iMid, midIndex);

          elements[elementIndex].largerIndex = elements.size(); // For binary search: set "larger than" index for element
          if(midIndex < size-1)
            SortBin(elements, ++iMid, iEnd, size-midIndex-1);
      } };

      SortBin(elementsSortedBinary, elementsSortedLinear.begin(), elementsSortedLinear.end(), elementsSortedLinear.size());

      // Add binary sorted elements to the table
      uint baseIndex = binaryParseTable.size();
      for(vector<BinaryIndexElement>::iterator i = elementsSortedBinary.begin(); i != elementsSortedBinary.end(); ++i)
      {
        i->largerIndex += baseIndex;
        binaryParseTable.push_back(*i);
      }

      //(TEMPORARY: Append a final "error" element to the table - error handling should be improved later...)
      BinaryIndexElement element;
      element.id = ID_SPECIAL;
      element.action = BinaryIndexElement::LRACTION_ERROR;
      element.param = 0;
      element.largerIndex = 0;
      binaryParseTable.push_back(element);
    }

    //// Additional post-processing of the parse table
    for(vector<BinaryIndexElement>::iterator i = binaryParseTable.begin(); i != binaryParseTable.end(); ++i)
    {
      // Convert shift/goto action's state index into a parse table offset
      if(i->action == BinaryIndexElement::LRACTION_SHIFT || i->action == BinaryIndexElement::LRACTION_GOTO)
        i->param = stateTableOffsets[i->param];
    }
  }

#ifdef _DEBUG
  void GrammarLR1::debugOutputItem(const Item& item) const
  {
    const Production& production = productions[item.productionIndex].first;

    if(production.symbolsLength <= 0)
      return;

    debugOutputSymbol(item.productionId);
    cout << " -> ";
    for(uint c = 0; c < production.symbolsLength; ++c)
    {
      if(item.inputPosition == c) cout << '.';
      debugOutputSymbol(production.symbols[c].id);
      if(c < static_cast<uint>(production.symbolsLength-1) || item.inputPosition == c+1) cout << ' ';
    }
    if(item.inputPosition == production.symbolsLength) cout << '.';
    if(item.lookaheadSymbol != static_cast<OSid>(ID_SPECIAL))
    {
      cout << ", ";
      debugOutputSymbol(item.lookaheadSymbol);
    }
  }

  void GrammarLR1::debugOutputEdge(State::Edges::const_reference edge) const
  {
    cout << "--(";
    debugOutputSymbol(edge.first);
    cout << ")--> " << edge.second;
  }

  void GrammarLR1::debugOutputStates() const
  {
    cout << endl;
    for(uint c = 0; c < states.size(); ++c)
    {
      const State& state = *states[c];
      cout << "State " << c << endl;
      cout << "---------" << endl;
      cout << "Items:" << endl;
      for(vector<Item>::const_iterator iItem = state.items.begin(); iItem != state.items.end(); ++iItem)
      {
        cout << ' ';
        debugOutputItem(*iItem);
        cout << endl;
      }

      cout << "Edges:" << endl;
      for(State::Edges::const_iterator iEdge = state.edges.begin(); iEdge != state.edges.end(); ++iEdge)
      {
        cout << ' ';
        debugOutputEdge(*iEdge);
        cout << endl;
      }

      cout << endl;
    }
  }

  void GrammarLR1::debugOutputTable() const
  {
    uint index = 0;
    cout << "Index\tID\tName\t\tAction\tParam\tLargerThanIndex" << endl;
    for(vector<BinaryIndexElement>::const_iterator i = binaryParseTable.begin(); i != binaryParseTable.end(); ++i)
    {
      const BinaryIndexElement& element = *i;
      cout << index << '\t' << (int&) element.id << '\t';
      if(element.id != static_cast<OSid>(ID_SPECIAL))
        debugOutputSymbol(element.id);
      else
        cout << "";

      switch(element.action & 7)
      {
        case BinaryIndexElement::LRACTION_ACCEPT: cout << "\t\taccept"; break;
        case BinaryIndexElement::LRACTION_SHIFT:  cout << "\tshift"; break;
        case BinaryIndexElement::LRACTION_REDUCE: cout << "\treduce"; break;
        case BinaryIndexElement::LRACTION_GOTO:   cout << "\tgoto"; break;
        case BinaryIndexElement::LRACTION_ERROR:  cout << "\t\terror"; break;
        default: cout << "\tunknown"; break;
      }

      cout << '\t' << element.param << '\t' << element.largerIndex << endl;
      ++index;
    }
    cout << endl << "Table size: " << binaryParseTable.size() * sizeof(BinaryIndexElement) << " bytes" << endl << endl;

    cout << endl;
  }
#endif
}

#endif
#endif
