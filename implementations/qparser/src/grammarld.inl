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
    for(typename vector<State*>::iterator i = states.begin(); i != states.end(); ++i)
      delete *i;*/
  }
  
  void GrammarLD::ConstructParser(Grammar* grammar)
  {
    builder.ConstructParseTable(parseTable);
  }

  void GrammarLD::Parse(ParseResult& parseResult)
  {
    // Check that the parse table has been created. We know it should contain at 
    // least an accept action, so if it is empty we exit immmediately.
    if(parseTable.size() == 0)
      return; // todo: error, parse table is empty (no grammar defined)

    // Perform the recognition pass
    ParseTokens rules;
    RecognitionPass(parseResult, rules);
    
    // Perform the final translation pass   
    
    /*// Main parse loop
    while(true)
    {
      token = (cToken < parseResult.lexStream.length)? parseResult.lexStream.data[cToken].id : ID_SPECIAL;

    skipTokenInitialization:

      const BinaryIndexElement& parseElement = binaryParseTable[cTable];
      if(parseElement.action == BinaryIndexElement::LRACTION_ERROR)
      {*/
  }
  
  void GrammarLD::RecognitionPass(ParseResult& parseResult, ParseTokens& rules)
  {
    rules.clear();
    
    // Parse table (actions) state
    uint parseState = 0;                  // The current position in the parse table
    ParseToken parseAction;               // The current parse action (from parseTable[parseState])
    std::stack<ParseToken> returnStates;  // Positions in the parse table to return on a return action
    uint lookaheadState = 0;              // The last leaf-node position in the parse table to use for resolving goto actions
    std::stack<ParseToken> delayedStates; // The position of each ignore token reduced (which still needs to be resolved)
            
    // Lexical stream state
    uint16 lexState = 0;              // The current position in the lex stream
    ParseToken lexToken = 0;          // The last token read from the lex stream
    
    bool skipReadingToken = false;     // A flag that allows the algorithm to skip reading a token from the lex stream
        
    // Perform the recognition
    while(true)
    {
      if (!skipReadingToken)
      {
        // Read a lexical token from the stream
        lexToken = (lexState < parseResult.lexStream.length? parseResult.lexStream.data[lexState].token : TOKEN_SPECIAL_EOF);
#ifdef QPARSER_TEST_GRAMMARLD
        infoStream << "Read lexical token (" << (lexToken & (~TOKEN_FLAG_SHIFT)) << ')' << std::endl;
#endif
      }
      skipReadingToken = true;
    
      // If the lexical token doesn't need to be re-read, skip to this label instead of continueing
      // Todo: we should test this and make sure that this optimization doesn't confuse the compiler
      // skipReading:
        
      // Read a parse action from the parse table
      parseAction = parseTable[parseState];
      
      // Recognize a terminal token
      // (Perform a shift action)
      if(parseAction == lexToken)
      {
#ifdef QPARSER_TEST_GRAMMARLD
        infoStream << "Shift(" << (lexToken & (~TOKEN_FLAG_SHIFT)) << ')' << std::endl;
#endif
        ++parseState;
        ++lexState;
        skipReadingToken = false;
        continue;
      }
      
      // Check for a reduce action
      // (we do this first because reduce and reduce previous actions are
      // likely to occur first, so we want to optimize for them)
      if(parseAction < TOKEN_RESERVED_TOKENS)
      {
        // Check whether this is a normal reduce action
        if(!(parseAction&(TOKEN_FLAG_SHIFT|TOKEN_FLAG_REDUCEPREV)))
        {
#ifdef QPARSER_TEST_GRAMMARLD
          if(parseAction == TOKEN_SPECIAL_IGNORE)
            infoStream << "Reduce (delay)" << std::endl;
          else
            infoStream << "Reduce (" << parseAction << ')' << std::endl;
#endif
      
          // If the token to reduce is an ignore token, add its position to the delayed states
          if(parseAction == TOKEN_SPECIAL_IGNORE)
            delayedStates.push(ParseToken(rules.size()));
          
          // Reduce the rule
          rules.push_back(parseAction);
          ++parseState;
          continue;
        }
        
        // Reduce a previously delayed token
        if(parseAction & TOKEN_FLAG_REDUCEPREV)
        {
#ifdef QPARSER_TEST_GRAMMARLD
          if((parseAction & (~TOKEN_FLAG_REDUCEPREV)) == TOKEN_SPECIAL_IGNORE)
            infoStream << "ReducePrev (delay)" << std::endl;
          else
            infoStream << "ReducePrev (" << (parseAction & (~TOKEN_FLAG_REDUCEPREV)) << ')' << std::endl;
#endif
          
          // Look up the position of the delayed rule
          OSI_ASSERT(!delayedStates.empty());
          ParseToken delayedState = delayedStates.top();
          delayedStates.pop();
           
          // Substitute the delayed rule with the proper token
          rules[delayedState] = parseAction & (~TOKEN_FLAG_REDUCEPREV);
          
          ++parseState;
          continue;
        }

        if(parseAction&TOKEN_FLAG_SHIFT)
        {
          // ERROR: Expected lexToken
          errorStream << "Unexpected token, at line [???] in program [???]" << std::endl;            
          errorStream << "-> Expected: " << (parseAction & (~TOKEN_FLAG_SHIFT)) << std::endl;          
          return;
        }
        
        continue;
      }
      
      // Check for all branching parse actions
      // (goto, ignore, return, accept)
      switch(parseAction)
      {
        case TOKEN_ACTION_PIVOT: 
        {          
          // Look up the number of branches in the pivot
          OSI_ASSERT(parseState != parseTable.size());
          ParseToken nPivots = parseTable[parseState+1];
          parseState += 2;
          
          // Push the next parse state onto the return stack (we will return to this point once done with the target state)
          // (We assume that the pivot action will succeed)
          returnStates.push(parseState + 2*nPivots);
          
          // Attempt to shift each of the pivots until a hit is 
          uint c;
          for(c = 0; c < nPivots; ++c)
          {
            ParseToken shiftToken = parseTable[parseState];
            parseState += 2;
            if(shiftToken == lexToken)
            {              
              // Get the target state to jump to and set this to the current parse state
              parseState = parseTable[parseState-1];
              
#ifdef QPARSER_TEST_GRAMMARLD
              infoStream << "Pivot(" << (lexToken & (~TOKEN_FLAG_SHIFT)) << ") -> " << parseState << std::endl;
#endif
              
              // Set the lookahead state to this state (for use with "goto" actions after we return)
              lookaheadState = parseState;
              
              // Advance the current position in the lexical stream
              ++lexState;
              skipReadingToken = false;
              break;
            }
          }
          
          // Check whether a pivot was reached
          if(c == nPivots)
          {
            // Pivot action did not succeed, so we must remove the returnState that we pushed onto the stack.
            returnStates.pop();
            
            // ERROR: Expected lexToken
            errorStream << "Unexpected token, at line [???] in program [???]" << std::endl;            
            errorStream << "-> Expected one of: ";
            for(uint c = 0; c < nPivots; ++c)
            {
              ParseToken shiftToken = parseTable[parseState - 2*(c+1)];
              errorStream << (shiftToken & (~TOKEN_FLAG_SHIFT)) << ' ';
            }
            errorStream << std::endl;
            
            return;
          }
          
          continue;
        }
        case TOKEN_ACTION_RETURN:
        {
          // Get the parse state to return to from the return stack
          parseState = returnStates.top();
          returnStates.pop();
          
#ifdef QPARSER_TEST_GRAMMARLD
          infoStream << "Return -> " << parseState << std::endl;
#endif              
          
          continue;
        }
        case TOKEN_ACTION_GOTO: 
        {
          OSI_ASSERT(parseState+2 < parseTable.size());
          
#ifdef QPARSER_TEST_GRAMMARLD
          infoStream << "Goto" << std::endl;
#endif
          
          // Get the lookahead state associated with this goto action and test it against 
          // the current lookahead state to determine if we should perform the jump
          ParseToken gotoLookaheadState = parseTable[parseState+1];
          if(lookaheadState == gotoLookaheadState)
          {
            // Lookup the target state to jump to 
            // (Do not assign this new state to the lookahead state. Goto actions do not set the 
            // lookahead state.)
            parseState = parseTable[parseState+2];
            continue;
          }
          parseState += 3;
          
          continue;
        }
        case TOKEN_ACTION_ACCEPT: 
        {
          // Post-condions:
          OSI_ASSERT(delayedStates.empty());
          OSI_ASSERT(returnStates.empty());
          
#ifdef QPARSER_TEST_GRAMMARLD
          infoStream << "Accept" << std::endl;
#endif
          
          // Check whether there are any lexical tokens left
          // If so, then log an error since the end of the file was expected at this point
          if(lexToken != TOKEN_SPECIAL_EOF)
          {
            // ERROR: End-of-file expected
            errorStream << "Unexpected token, at line [???] in program [???]" << std::endl;            
            errorStream << "-> Expected end of file" << std::endl;
            return;
          }          
          return; // we're done with the recognition phase
        }
      }      
    }
  }

  /*template<typename Item>
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
  
  template<typename Item>
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
  
  template<typename Item>
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

  template<typename Item>
  int GrammarLR<Item>::findItemState(const Item& item)
  {
    typename map<Item, uint>::iterator i = itemStateIndex.find(item);
    if(i == itemStateIndex.end() || i->first != item)
      return -1;
    return i->second;
  }

#ifdef _DEBUG
  template<typename Item>
  void GrammarLR<Item>::debugOutputEdge(typename State::Edges::const_reference edge) const
  {
    cout << "--(";
    debugOutputSymbol(edge.first);
    cout << ")--> " << edge.second;
  }

  template<typename Item>
  void GrammarLR<Item>::debugOutputStates() const
  {
    cout << std::endl;
    for(uint c = 0; c < states.size(); ++c)
    {
      const State& state = *states[c];
      cout << "State " << c << std::endl;
      cout << "---------" << std::endl;
      cout << "Items:" << std::endl;
      for(typename vector<Item>::const_iterator iItem = state.items.begin(); iItem != state.items.end(); ++iItem)
      {
        cout << ' ';
        debugOutputItem(*iItem);
        cout << std::endl;
      }

      cout << "Edges:" << std::endl;
      for(typename State::Edges::const_iterator iEdge = state.edges.begin(); iEdge != state.edges.end(); ++iEdge)
      {
        cout << ' ';
        debugOutputEdge(*iEdge);
        cout << std::endl;
      }

      cout << std::endl;
    }
  }
#endif*/
}

#endif
#endif
