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
    class ActionRow; 
    class PivotSet;
    typedef std::vector<ActionRow*> ActionTable;
    typedef std::vector<ParseToken> ParseTokens;
    typedef std::vector<PivotSet*> PivotSets;
    
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
  
  // An action row used during construction
  class BuilderLD::ActionRow
  {
  public:
    ParseTokens actions; // Sequence of actions contained in this row
    PivotSets pivotSets; // Sets of pivots contained in the row

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
    //ActionRow& AddActionGoto();
    void AddActionGoto(ParseToken lookaheadRow, ParseToken targetRow);
    
    // Add a return action to the table
    void AddActionReturn();
    
    // Add an accept action to the table
    void AddActionAccept();
    
    //// Accessors
    INLINE BuilderLD& GetBuilder() { return builder; }
    INLINE const BuilderLD& GetBuilder() const { return builder; }
    
  private:
    BuilderLD& builder;               // The main builder object
    
    //INLINE ActionRow() : builder(null) {}    
  };   
  
  // Set of pivots
  class BuilderLD::PivotSet
  {
  public:
    ParseTokens pivotTokens;
    ParseTokens targetRows;

    // Construction / Destruction
    //INLINE PivotSet(const PivotSet& pivotSet) : row(const_cast<ActionRow*>(pivotSet.row)) {}
    INLINE PivotSet(BuilderLD& builder, ActionRow& row) : builder(builder), row(row) {}

    // Add another pivot with a destination row number
    INLINE ActionRow& AddPivot(ParseToken token) 
    {
      AddPivot(token, ParseToken(GetBuilder().actionTable.size()));
      return GetBuilder().AddActionRow();
    }

    INLINE void AddPivot(ParseToken token, ParseToken rowNumber) 
    {
      pivotTokens.push_back(token);
      targetRows.push_back(rowNumber);
    }
    
    //// Accessors
    INLINE ActionRow& GetRow() { return row; }
    INLINE const ActionRow& GetRow() const { return row; }
    INLINE BuilderLD& GetBuilder() { return builder; }
    INLINE const BuilderLD& GetBuilder() const { return builder; }
    
  private:
    BuilderLD& builder;
    ActionRow& row;                   // The parent row
    
    //INLINE PivotSet() : builder(null), row(null) {}
  };
}

/*                                   INCLUDES                               */
#include "builderld.inl"

#endif
