#ifdef  __QPARSER_BUILDERLD_H__
#ifndef __QPARSER_BUILDERLD_INL__
#define __QPARSER_BUILDERLD_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    BUILDERLD.INL
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QParser
{    
  ////////////////////////////////////////////////////////////////////////////
  // BuilderLD
  INLINE BuilderLD::~BuilderLD()
  {
    std::for_each(actionTable.begin(), actionTable.end(), DeleteFunctor<ActionRow>());
  }
  
  INLINE BuilderLD::ActionRow& BuilderLD::AddActionRow()
  {
    actionTable.push_back(new ActionRow(*this));
    return *actionTable.back();
  }
  
  INLINE ParseToken BuilderLD::GetRowIndex(const ActionRow& row) const 
  { 
    // todo: Optimize. Possibly speed this query up by storing the row index in every row instead of searching the entire set of rows.
    ParseToken index = std::find(actionTable.begin(), actionTable.end(), &row) - actionTable.begin();
    return index < actionTable.size()? index : ParseToken(-1);
  }
          
  INLINE void BuilderLD::ConstructParseTable(ParseTokens& parseTable)
  {
    parseTable.clear();
    
    // The final parse table that is output by the construction algorithm
    ParseTokens rowOffsets; // The offset of ever action row in the final parse table
    rowOffsets.reserve(actionTable.size()); // The offset of ever action row in the final parse table
    
    for(ActionTable::const_iterator iActionRow = actionTable.begin(); iActionRow != actionTable.end(); ++iActionRow)
    {
      ActionRow& actionRow = **iActionRow;
      
      // Store the final offset of this row in the parse table
      rowOffsets.push_back(ParseToken(parseTable.size()));
      
      // Get an iterator into the pivot sets associated with this action row
      PivotSets::const_iterator iPivotSet = actionRow.pivotSets.begin();
      
      for(ParseTokens::const_iterator iToken = actionRow.actions.begin(); iToken != actionRow.actions.end(); ++iToken)
      {
        // Process a pivot action
        if(*iToken == TOKEN_ACTION_PIVOT)
        {
          // Push the pivot action
          parseTable.push_back(TOKEN_ACTION_PIVOT);
          const PivotSet& pivotSet = **iPivotSet;
          
          // Push the number of pivots onto the action table
          parseTable.push_back(pivotSet.pivotTokens.size());
          
          for(uint cPivot = 0; cPivot < pivotSet.pivotTokens.size(); ++cPivot)
          {
            // Push the token to match for this pivot
            parseTable.push_back(pivotSet.pivotTokens[cPivot]);
            
            // Push the index of the target row for this pivot
            // (to be replaced by a parse table offset later)
            parseTable.push_back(pivotSet.targetRows[cPivot]);
          }
          
          // Move the pivot set iterator to the next pivot set associated this action row (if any)
          ++iPivotSet;
          continue;
        }
        
        // Process a goto action
        if(*iToken == TOKEN_ACTION_GOTO)
        {
          // Push the goto action
          parseTable.push_back(TOKEN_ACTION_GOTO);
          
          // Push the index of the lookahead row for this goto action
          // (to be replaced by a parse table offset later)
          ++iToken;
          OSI_ASSERT(iToken != actionRow.actions.end());
          parseTable.push_back(*iToken);
          
          // Push the index of the target row for this goto action
          // (to be replaced by a parse table offset later)
          ++iToken;
          OSI_ASSERT(iToken != actionRow.actions.end());
          parseTable.push_back(*iToken);
          continue;
        }
        
        // Process all other actions (shift, reduce, ignore, reduce previous, accept)
        {
          parseTable.push_back(*iToken);
        }
      }
      
      // Push a return statement onto the parse table
      // todo?...
    }
    
    // Replace all row indexes with parse table offsets
    for(ParseTokens::iterator iToken = parseTable.begin(); iToken != parseTable.end(); ++iToken)
    {
      if(*iToken == TOKEN_ACTION_PIVOT)
      {
        // Get the length of the pivot
        ++iToken;
        OSI_ASSERT(iToken != parseTable.end());       
        ParseToken pivotLength = *iToken;
         
        // Look up the table offset for the target row index of each pivot
        for(uint cPivot = 0; cPivot < pivotLength; ++cPivot)
        {         
          iToken += 2;
          OSI_ASSERT(iToken != parseTable.end());
          *iToken = rowOffsets[*iToken];
        }
        continue;
     }
     
     if(*iToken == TOKEN_ACTION_GOTO)
     {
       // Look up the table offset for the lookahead row index of the goto action
       ++iToken;
       OSI_ASSERT(iToken != parseTable.end());
       *iToken = rowOffsets[*iToken];
       
       // Look up the table offset for the target row index of the goto action
       ++iToken;
       OSI_ASSERT(iToken != parseTable.end());
       *iToken = rowOffsets[*iToken];
       continue;
     }
   }
  }
  
  ////////////////////////////////////////////////////////////////////////////
  // ActionRow
  
  INLINE BuilderLD::ActionRow::ActionRow(BuilderLD& builder) : builder(builder) 
  {}
  
  //INLINE BuilderLD::ActionRow::ActionRow(const ActionRow& row) : builder(row.builder)
  //{}
  
  INLINE BuilderLD::ActionRow::~ActionRow()
  {
    std::for_each(pivotSets.begin(), pivotSets.end(), DeleteFunctor<PivotSet>());
  }
  
  INLINE void BuilderLD::ActionRow::AddActionShift(ParseToken terminal)
  {
    actions.push_back(terminal);
  }  

  INLINE BuilderLD::PivotSet& BuilderLD::ActionRow::AddActionPivot()
  {
    actions.push_back(TOKEN_ACTION_PIVOT);
    pivotSets.push_back(new PivotSet(GetBuilder(), *this));
    return *pivotSets.back();
  }

  INLINE void BuilderLD::ActionRow::AddActionReduce(ParseToken rule)
  {
    actions.push_back(rule);      
  }
  
  INLINE void BuilderLD::ActionRow::AddActionReducePrev(ParseToken rule)
  {
    // We always prepend reduce prev actions since they reduce actions in previous states
    actions.insert(actions.begin(), rule | TOKEN_FLAG_REDUCEPREV);      
  }

  /*INLINE BuilderLD::ActionRow& BuilderLD::ActionRow::AddActionGoto()
  {
    actions.push_back(TOKEN_GOTO);
    actions.push_back(GetBuilder().GetActionTable().size());
    return GetBuilder().AddActionRow();
  }*/
  
  INLINE void BuilderLD::ActionRow::AddActionGoto(ParseToken lookaheadRow, ParseToken targetRow)
  {
    actions.push_back(TOKEN_ACTION_GOTO);
    actions.push_back(lookaheadRow);
    actions.push_back(targetRow);
  }
  
  INLINE void BuilderLD::ActionRow::AddActionReturn()
  {
    actions.push_back(TOKEN_ACTION_RETURN);
  }
  
  INLINE void BuilderLD::ActionRow::AddActionAccept()
  {
    actions.push_back(TOKEN_ACTION_ACCEPT);
  }
  
}

#endif
#endif
