#ifndef __QPARSER_GRAMMARLD_H__
#define __QPARSER_GRAMMARLD_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMARLD.H
//
//    Copyright © 2007-2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      An LD (left-to-right, delayed reduction) parser

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
// Turn on testing for the library (This should be turned off for any production
// build)
//#define QPARSER_TEST_GRAMMARLD

/*                                   INCLUDES                               */
#include "builderld.h"

namespace QParser
{
/*                                  CLASSES                                 */
  // An LD item (which is more or less identical to an LR(0) item)
  struct LDItem : public LRItem
  {    
    uint inputPositionRule;   // The rule index for the token currently being referenced to by the input position (cursor). This is uint(-1) if the token is a terminal token.
    
    INLINE LDItem(ParseToken nonterminal) : LRItem(nonterminal), inputPositionRule(-1) {}
    INLINE LDItem(ParseToken nonterminal, uint ruleIndex) : LRItem(nonterminal, ruleIndex), inputPositionRule(-1) {}
    INLINE LDItem(const LDItem&) = default;
    INLINE LDItem() = delete;
    
    FORCE_INLINE bool operator < (const LDItem& item) const
    { return nonterminal < item.nonterminal
        || (nonterminal == item.nonterminal
          && (ruleIndex < item.ruleIndex
            || (ruleIndex == item.ruleIndex
              && inputPosition < item.inputPosition)
                || (inputPosition == item.inputPosition
                    && inputPositionRule < item.inputPositionRule))); }
    FORCE_INLINE bool operator == (const LDItem& item) const { return nonterminal == item.nonterminal && ruleIndex == item.ruleIndex && inputPosition == item.inputPosition /*&& inputPositionRule == item.inputPositionRule*/; }
    FORCE_INLINE bool operator != (const LDItem& item) const { return !(*this == item); }
  };
  
  // LD state is a collection of LD items along with additional grammar building information such as a delayed reduction stack
  struct LDState : public LRState<LDItem>
  {
    // todo: this may not be the most efficient layout... Investigate alternatives
    typedef std::map<uint, uint> DelayedRuleMap;            // A mapping between rules where the key is the parent rule and the value is a rule which has been delayed for reduction
    typedef std::vector<DelayedRuleMap> DelayedRuleStack;   // A stack of delayed rule maps
    typedef std::map<LDState*, ParseToken> Edges;           // A set of state transitions and the terminal token with which the edge is associated
    typedef Edges::value_type Edge;                         // A single edge in the state graph
    BuilderLD::ActionRow& row;                              // The parse table row corresponding to this (deterministic) state
    DelayedRuleStack delayedReductions;                     // A stack of all the reductions that had to be delayed
    Edges leadingEdges;                                     // The set of edges leading TO this state
    Edges forwardEdges;                                     // The set of edges leading FROM this state
    //std::set<uint> completeRules;                       // The set of completed rules of the final (complete) state
    //bool ambiguous;                                     // A flag indicating that this state is an ambiguous leaf node
    
    // Constructor
    INLINE LDState(BuilderLD::ActionRow& row) : row(row) {}
  };
  
  // LD Grammar
  class GrammarLD : public GrammarLR<LDItem, LDState>
  {
  public:    
    // Constructor
    INLINE GrammarLD(TokenRegistry& tokenRegistry) : GrammarLR<LDItem, LDState>(tokenRegistry) {}
    INLINE GrammarLD(const GrammarLD&) = delete;
    INLINE GrammarLD() = delete;
    
    // Destructor
    INLINE ~GrammarLD();
    
    // Parse table construction
    INLINE void ConstructParseTable(ParseTokens& parseTable);
    
  protected:
    // Common types
    typedef LDItem Item;
    typedef LDState State;
    
    /*struct ResolutionEnvironment
    {
      const State* leafState;     // The leaf state that preceded this delayed resolution
      bool ambiguous;             // A flag indicating that the leaf state is a ambiguous (it completes multiple rules)
      
      INLINE ResolutionEnvironment() : leafState(null), ambiguous(false) {}
    } resolutionEnvironment;*/
    
    // Construct a state graph (recursively)
    void ConstructStateGraph(BuilderLD& builder, State& state);
    
    // Resolve item tokens at the cursor to rules (if it is a nonterminal)
    //INLINE void ResolveItemsActiveToken(Items& items, Items::const_iterator iBegin);
    
    // Expand the set of items recursively until we reach a point where they cannot be expanded further
    void ExpandItemSet(State& state);
    void ExpandItemSet(State& state, uint cBegin, uint cEnd);
    
    // Get the set of terminal tokens at the input positions of items and step over them
    void StepOverTerminals(ParseTokenSet& terminals, Items& items) const;
    
    // Complete all items that have no more tokens left after the input position 
    // returns true if all items in the state are complete
    bool CompleteItems(BuilderLD& builder, State& state);
    
    // Copy a state using the given pivot terminal to eliminate all items that are not applicable
    void CopyStateUsingPivot(const State& state, State& targetState, ParseToken pivotTerminal) const;
    
    // Resolve all delayed reductions related to the given leaf state
    void ResolveDelays(BuilderLD& builder, State& leafState, uint leafRule);
    void ResolveDelays(BuilderLD& builder, State& leafState, ParseToken leafRowIndex, State& currentState, std::stack<uint>& ruleResolutionStack);
    
    /*// Determine the closure of a set of items
    INLINE void Closure(Items& items);
    INLINE void Closure(Items& items, uint cBegin, uint cEnd);

    // Determine the goto states, items and edges for an existing set of states
    INLINE void GoTo(States& states);
    INLINE void GoTo(States& states, uint cBegin, uint cEnd);*/
  };
}

/*                                   INCLUDES                               */
#include "grammarld.inl"

#endif
