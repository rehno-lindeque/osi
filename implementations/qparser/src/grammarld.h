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
/*                                   INCLUDES                               */
#include "builderld.h"

namespace QParser
{
/*                                  CLASSES                                 */
  /*struct LRItem
  {
    OSid productionId;
    uint productionIndex;
    uint inputPosition;
    
    INLINE LRItem(OSid nonterminal) : productionId(nonterminal), productionIndex(0), inputPosition(0) {}
  
  private:
    INLINE LRItem() {}
  };*/
  
  struct LDOutput
  {
    uint16 ruleNumber;
    uint16 descendants;
  };
  
  class GrammarLD : public Grammar
  {
  public:
    typedef std::vector<ParseToken> ParseTokens;
    
    // Constructor
    INLINE GrammarLD() {}
    INLINE ~GrammarLD();
    
    // Construct productions
    virtual void constructProductions();

    // Parse
    virtual void parse(ParseResult& parseResult);
      
  protected:
    BuilderLD builder;
    ParseTokens parseTable;
  };
}

/*                                   INCLUDES                               */
#include "grammarld.inl"

#endif
