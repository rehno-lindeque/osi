#ifndef __QPARSER_API_H__
#define __QPARSER_API_H__
//////////////////////////////////////////////////////////////////////////////
//
//    API.H
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      QParser API
*/
/*                              COMPILER MACROS                             */

/*                                 INCLUDES                                 */
//// Standard Parser (pseudo-OSI)
//#include "../common/types.h"
//#include "../common/definitions.h"

// OpenParser
#include <osix/parser/parser.hpp>
#ifdef _DEBUG
  #include <osix/parser/parserdbg.hpp>
#endif

// BaseParser
#include <baseparser/baseparser.h>

// STL
#include <list>
#include <stack>

//// SIMD
//#include "../simd/simd.h"

// QParser
#include "grammar.h"

/*                                  CLASSES                                 */
namespace QParser
{
  class Parser : public BaseParser::Parser
  {
  public:
    Grammar* grammar;

    FORCE_INLINE Parser() : grammar(null) {}
    
#   ifdef _DEBUG
      class ParserDbg : public ::ParserDbg
      {} parserDbg;
#   endif
  };
}

/*                                   INCLUDES                               */
#include "api.hxx"

#endif
