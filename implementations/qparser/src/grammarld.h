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
    typedef boost::bimap<LDState*, ParseToken> PivotEdges;  // A set of state transitions and the terminal token with which the edge is associated
    typedef PivotEdges::value_type PivotEdge;               // A state transition originating from a pivot action
    typedef std::map<LDState*, LDState*> GotoEdges;         // A set of state transitions as well as a reference to the leaf state which is used for the goto action
    typedef GotoEdges::value_type GotoEdge;                 // A state transition originating from a goto action
    BuilderLD::ActionRow& row;                              // The parse table row corresponding to this (deterministic) state
    DelayedRuleStack delayedReductions;                     // A stack of all the reductions that had to be delayed
    PivotEdges incomingPivots;                              // The set of edges leading TO this state through pivot actions
    GotoEdges incomingGotos;                                // The set of edges leading TO this state through goto actions
    PivotEdges outgoingPivots;                              // The set of edges leading FROM this state through pivot actions
    GotoEdges outgoingGotos;                               // The set of edges leading FROM this state through goto actions
    //bool cyclic;                                            // A flag indicating that this state is in a cycle in the grammar (and hence delays should be resolved using goto actions)
    uint cyclicNestingDepth;                                // An integer indicating how many nested cycles this state is involved in
    //std::set<uint> completeRules;                       // The set of completed rules of the final (complete) state
    //bool ambiguous;                                     // A flag indicating that this state is an ambiguous leaf node
    
    // Constructor
    INLINE LDState(BuilderLD::ActionRow& row) : row(row), cyclicNestingDepth(0) /*, cyclic(false)*/ {}
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
    //typedef std::map<LDState*, uint> ResolvedRules;   // State, number of rules resolved (starting from the right side)
    
    //todo: The current leaf state
    //LDState* leafState;
        
    // Construct a state graph (recursively)
    void ConstructStateGraph(BuilderLD& builder, State& state/*, ResolvedRules& resolvedRules*/);
    
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
    //void ResolveDelays(BuilderLD& builder, ResolvedRules& resolvedRules);
    //void ResolveDelays(BuilderLD& builder, ResolvedRules& resolvedRules, ParseToken leafRowIndex, State& currentState);
    
    // Resolve all delayed rules starting from the root state
    void ResolveDelays(BuilderLD& builder, State& rootState);
    
    // Try to generate a cyclic pivot for the given end-state. If such a cycle exists, then the function returns true,
    // otherwise it returns false and a normal pivot should be generated
    bool GenerateCyclicPivot(BuilderLD& builder, State& state, const ParseTokenSet& terminals);
    
    // Try to find a previous state corresponding with the last state so that a cycle can be formed
    State* DetectCycle(State& lastState);
    State* DetectCycle(State& currentState, const State& lastState);
    
    // Compare two end-state to see if they have the same pivots and remaining actions. Returns true if they are compatible (i.e. a cycle may be formed)
    bool CompareEndStates(const State& state1, const State& state2) const;
    
    // Generate a (non-cyclic) pivot for the given end-state
    void GeneratePivot(BuilderLD& builder, State& state, /*ResolvedRules& resolvedRules,*/ const ParseTokenSet& terminals);
  };
}

/*                                   INCLUDES                               */
#include "grammarld.inl"

#endif
