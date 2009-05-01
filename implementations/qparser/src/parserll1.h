#ifndef __QPARSER_GRAMMARLL1_H__
#define __QPARSER_GRAMMARLL1_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GRAMMARLL1.H
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      LL1 class grammar
*/

/*                              COMPILER MACROS                             */
/*                                   INCLUDES                               */
namespace QParser
{
/*                                  CLASSES                                 */
  class GrammarLL1 : public Grammar
  {
  public:
    // Constructor
    INLINE GrammarLL1() {}

    //// Construction
    // Construct productions
    virtual void constructProductions();

    // Construct an LL(1) linear binary indexed parse table
    INLINE void constructParseTable();
  };
}

/*                                   INCLUDES                               */
#include "parserll1.inl"

#endif
