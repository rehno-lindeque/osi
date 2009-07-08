#ifndef __QPARSER_BUILDERLD_H__
#define __QPARSER_BUILDERLD_H__
//////////////////////////////////////////////////////////////////////////////
//
//    BUILDERLD.H
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      An LD (left-to-right, delayed reduction) grammar builder

    REFERENCES:
      + [modcomp]
        title = "...."
        author = "Rehno Lindeque"
        publisher = ""
        isbn = ""
        pages = ""
        year = ""
*/
/*                                 INCLUDES                                 */
#include "token.h"
#include "functors.h"

/*                                  CLASSES                                 */
namespace QParser
{
  typedef ParseToken ParseToken;
    
  class BuilderLD
  {
  public:
    //// Table construction
    class PivotSet;
    class GotoSet;
    class ActionRow; 
    typedef std::vector<ParseToken> ParseTokens;
    typedef std::vector<PivotSet*> PivotSets;
    typedef std::vector<ActionRow*> ActionTable;
    
    // Construction / Destruction
    ~BuilderLD();
        
    // Returns the index to an action row
    ActionRow& AddActionRow();
    
    //// Accessors
    // Get an action row at a specific index
    INLINE ActionRow& GetActionRow(ParseToken rowIndex) { return *actionTable[rowIndex]; }
    INLINE const ActionRow& GetActionRow(ParseToken rowIndex) const { return *actionTable[rowIndex]; }
    
    // Get the index of an action row
    ParseToken GetRowIndex(const ActionRow& row) const;
    
    // Get the entire action table
    INLINE ActionTable& GetActionTable() { return actionTable; }
    INLINE const ActionTable& GetActionTable() const { return actionTable; }
    
    //// Construct the parse table
    void ConstructParseTable(ParseTokens& parseTable);
    
  protected:
    ActionTable actionTable;
  };
  
  // Set of pivots
  class BuilderLD::PivotSet
  {
  public:
    ParseTokens pivotTokens;
    //ParseTokens targetRows;
    std::vector<const ActionRow*> targetRows;

    // Construction / Destruction
    //INLINE PivotSet(const PivotSet& pivotSet) : row(const_cast<ActionRow*>(pivotSet.row)) {}
    INLINE PivotSet(BuilderLD& builder, ActionRow& row) : builder(builder), row(row) {}

    // Add a pivot to the set
    INLINE ActionRow& AddPivot(ParseToken token) 
    {
      ActionRow& row = GetBuilder().AddActionRow();
      AddPivot(token, row);
      return row;
    }

    INLINE void AddPivot(ParseToken token, const ActionRow& row) 
    {
      pivotTokens.push_back(token);
      targetRows.push_back(&row);
    }
    
    //// Accessors
    INLINE ActionRow& GetRow() { return row; }
    INLINE const ActionRow& GetRow() const { return row; }
    INLINE BuilderLD& GetBuilder() { return builder; }
    INLINE const BuilderLD& GetBuilder() const { return builder; }
    
  private:
    BuilderLD& builder;               // The builder object
    ActionRow& row;                   // The parent row
    
    //INLINE PivotSet() : builder(null), row(null) {}
  };
  
  // Set of gotos
  class BuilderLD::GotoSet
  {
  public:
    typedef std::pair<const ActionRow*, const ActionRow*> GotoEdge;
    typedef std::vector<GotoEdge> GotoEdges;
    GotoEdges gotoEdges;    // List of edges with from lookaheadRow -> targetRow
    
    // Construction / Destruction
    INLINE GotoSet(BuilderLD& builder, ActionRow& row) : builder(builder), row(row) {}
    
    // Add a goto to the set
    void AddGoto(const ActionRow& lookaheadRow, const ActionRow& targetRow)
    {
      gotoEdges.push_back(std::make_pair(&lookaheadRow, &targetRow));
    }
    
    ActionRow& AddGoto(const ActionRow& lookaheadRow)
    {
      ActionRow& targetRow = GetBuilder().AddActionRow();
      gotoEdges.push_back(std::make_pair(&lookaheadRow, &targetRow));
      return targetRow;
    }
    
    //// Accessors
    INLINE ActionRow& GetRow() { return row; }
    INLINE const ActionRow& GetRow() const { return row; }
    INLINE BuilderLD& GetBuilder() { return builder; }
    INLINE const BuilderLD& GetBuilder() const { return builder; }
    
  private:
    BuilderLD& builder;               // The builder object
    ActionRow& row;                   // The parent row
  };
  
  // An action row used during construction
  class BuilderLD::ActionRow
  {
  public:
    ParseTokens actions;            // Sequence of actions contained in this row
    ParseTokens resolvedReductions; // Delayed reductions that should be resolved using ReducePrev actions
    PivotSets pivotSets;            // Sets of pivots contained in the row
    GotoSet gotoSet;                // Set of gotos contained in the row

    // Construction / Destruction
    //INLINE ActionRow() : builder(null));
    ActionRow(BuilderLD& builder);
    //ActionRow(const ActionRow& row);
    
    ~ActionRow();
    
    //// Actions
    // Add a shift action to the row
    void AddActionShift(ParseToken terminal);

    // Add a pivot action to the row
    PivotSet& AddActionPivot();

    // Add a reduce action to the table
    void AddActionReduce(ParseToken rule);
    
    // Add a reduce previous action to the table
    void AddActionReducePrev(ParseToken rule);

    // Add a goto action to the row
    GotoSet& AddActionGoto();
    
    // Add a return action to the table
    void AddActionReturn();
    
    // Add an accept action to the table
    void AddActionAccept();
    
    //// Accessors
    INLINE BuilderLD& GetBuilder() { return builder; }
    INLINE const BuilderLD& GetBuilder() const { return builder; }
    
    INLINE const PivotSet& GetLastPivotSet() const { return *pivotSets.back(); }
    INLINE PivotSet& GetLastPivotSet() { return *pivotSets.back(); }
    
  private:
    bool gotoActionAdded;             // Flag indicating that a goto action has been added
    BuilderLD& builder;               // The main builder object
    
    //INLINE ActionRow() : builder(null) {}    
  };
}

/*                                   INCLUDES                               */
#include "builderld.inl"

#endif
