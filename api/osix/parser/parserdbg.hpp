#ifndef __OSIX_PARSERDBG_HPP__
#define __OSIX_PARSERDBG_HPP__
//////////////////////////////////////////////////////////////////////////////
//
//    PARSERDBG.HPP
//
//    Ver 0.2 (2007-11-28)
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*
    DESCRIPTION:
      Debug OpenParser C++ interface.

    USAGE:
      parser.hpp must be included before this header
*/
/*                                 INCLUDES                                 */

/*                                 CONSTANTS                                */
#define PARSERDBG_VERSION 0.2

namespace OSIX
{
  /*                                   TYPES                                  */

  /*                                     API                                  */
  class ParserDbg
  {
  public:
    OSI_DYNAMIC_METHOD void OSI_API_CALL debugOutputTokens();
    OSI_DYNAMIC_METHOD void OSI_API_CALL debugOutputProductions();
    OSI_DYNAMIC_METHOD void OSI_API_CALL debugOutputGrammar();
    OSI_DYNAMIC_METHOD void OSI_API_CALL debugOutputParseResult(OSobject parseResult);
  };
}

#endif
