#ifdef  __QPARSER_GRAMMARLL1_H__
#ifndef __QPARSER_GRAMMARLL1_INL__
#define __QPARSER_GRAMMARLL1_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMARLL1.INL
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QParser
{
  void GrammarLL1::ConstructParser(Grammar* grammar)
  {
    constructParseTable();
  }

  void GrammarLL1::constructParseTable()
  {
    /*
    // TODO: productions can only derive one string at the moment, so this won't work right now...

    // Determine LL(1) first and follow sets
    // Algorithm: Dependent productions must be declared in order, hence we can simplify first and follow set
    //            determination by processing productions in order. (Hence we easily achieve an O(n) algorithm)
    //            Note that we don't sort or remove duplicates from first and follow sets since this is done later
    //            while we're constructing our parse table.
    //            Follow sets are determined in two steps. This is done so that iterative processing won't be needed.
    //            The first step we loop from front-to-back (through the productions) adding all up the appropriate
    //            first sets that are direct follow symbols within each production.
    //            For the second step we loop back-to-front adding follow sets for each parent production to their children's
    //            follow sets.
    //            Order of declaration is very important for all of the above to work. (i.e. forward declerations in the grammar
    //            will not be possible using this technique). Note: This will probably have to be adapted because forward declerations
    //            are necessary to do non-trivial (a->b, b->a) recursion...
    for(map<OSid, ProductionSet*>::iterator i = productionSets.begin(); i != productionSets.end(); ++i)
    {
      ProductionSet& productionSet = *i->second;
      OSid productionId = i->first;

      // Determine if production is nullable (if all symbols are nullable then production is nullable)
      productionSet.nullable = false;
      for(uint c = 0; c < productionSet.productionsLength; ++c)
      {
        const Production& production = productionSet.productions[c];

        bool productionNullable = true;
        for(uint cSymbol = 0; cSymbol < production.symbolsLength; ++cSymbol)
        {
          const Production::Symbol& symbol = production.symbols[cSymbol];

          // Ignore symbol if it references this production (i.e. a recursive
          if(symbol.id == productionId)
            continue;

          // Production is not nullable if it contains terminal symbols (i.e lexical tokens)
          if(isTokenId(symbol.id))
          {
            productionNullable = false;
            break; // found non-null token
          }

          // Test whether non-terminal is nullable
          OSI_ASSERT(isProductionId(symbol.id));
          productionNullable |= (*productionSets[symbol.id]).nullable;
          if(!productionNullable)
            break; // found non-null token
        }

        if(productionSet.nullable |= productionNullable)
          break; // found null production
      }

      // Find first set
      {
        vector<Production::Symbol> firstSet;
        for(uint c = 0; c < productionSet.productionsLength; ++c)
        {
          const Production& production = productionSet.productions[c];
          for(uint cSymbol = 0; cSymbol < production.symbolsLength; ++cSymbol)
          {
            const Production::Symbol& symbol = production.symbols[cSymbol];

            // Test for a terminal symbol.
            if(isTokenId(symbol.id))
            {
              firstSet.push_back(symbol);
              break; // Found terminal: First set is complete
            }

            // Add non-terminal's first set
            OSI_ASSERT(isProduction(symbol.id));
            ProductionSet& nonterminalSet = *productionSets[symbol.id];
            for(uint cFirst = 0; cFirst < nonterminalSet.firstSetLength; ++cFirst)
              firstSet.push_back(nonterminalSet.firstSet[cFirst]);

            // Test whether nonterminal is nullable
            if(!nonterminalSet.nullable)
              break; // Nonterminal is not nullabe: First set is complete
          }
        }

        productionSet.firstSetLength = firstSet.size();
        productionSet.firstSet = new Production::Symbol[productionSet.firstSetLength];
        memcpy(productionSet.firstSet, &firstSet[0], productionSet.firstSetLength * sizeof(Production::Symbol));
      }

      // Find follow sets within each production
      {
        for(uint c = 0; c < productionSet.productionsLength; ++c)
        {
          vector<Production::Symbol> followSet;

          const Production& production = productionSet.productions[c];
          for(int cSymbol = production.symbolsLength-1; cSymbol >= 0; --cSymbol)
          {
            const Production::Symbol& symbol = production.symbols[cSymbol];

            // Test for a terminal symbol.
            if(isTokenId(symbol.id))
            {
              // followset = { terminal }
              followSet.clear();
              followSet.push_back(symbol);
              continue;
            }

            OSI_ASSERT(isProduction(symbol.id));
            ProductionSet& nonterminalSet = *productionSets[symbol.id];

            // Add current followset to the nonterminal's follow set
            nonterminalSet.followSet.push_back(followSet);

            // Add nonterminal to current follow set
            // Algorithm: if nonterminal is nullable, followset = followset + first(nonterminal)
            //            else, followset = first(nonterminal)
            if(!nonterminalSet.nullable)
              followSet.clear();

            for(uint cFirst = 0; cFirst < nonterminalSet.firstSetLength; ++cFirst)
              followSet.push_back(nonterminalSet.firstSet[cFirst]);
          }
        }
      }
    }

    // Complete follow sets: Loop back to front and add the appropri parent productions' symbols to the children's
    // follow sets
    for(map<OSid, ProductionSet*>::reverse_iterator i = productionSets.rbegin(); i != productionSets.rend(); ++i)
    {
      ProductionSet& productionSet = *i->second;
      OSid productionId = i->first;

      // Determine if production is nullable (if all symbols are nullable then production is nullable)
      vector<Production::Symbol> newFollowSet;
      for(vector<Production::Symbol> iFollow = productionSet.followSet; iFollow != productionSet.followSet.end(); ++iFollow)
      {
        OSid id = iFollow->id;

        // If we find a terminal, add it directly to the follow set
        if(isTokenId(id))
        {
          newFollowSet.push_back(id);
          continue;
        }

        OSI_ASSERT(isProductionId(id));
        ProductionSet& nonterminalSet = *productionSets[symbol.id];

        // Expand nonterminal
        // Add the first set of each production to the follow set


      }
      productionSet.followSet = newFollowSet;
    }*/
  }
}

#endif
#endif
