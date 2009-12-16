#ifndef __OSIX_SEMANTICDBDBG_HPP__
#define __OSIX_SEMANTICDBDBG_HPP__
//////////////////////////////////////////////////////////////////////////////
//
//    SEMANTICDBDBG.HPP
//
//    Ver 0.1 (2009-08-21)
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*
    DESCRIPTION:
      Debug OpenSemanticDB C++ interface.

    USAGE:
      semanticdb.hpp must be included before this header
*/
/*                                 INCLUDES                                 */

/*                                 CONSTANTS                                */
#define SEMANTICDBDBG_VERSION "0.1"

namespace OSIX
{
  /*                                   TYPES                                  */

  /*                                     API                                  */
  class SemanticDBDbg
  {
  public:
    OSI_DYNAMIC_METHOD void OSI_API_CALL DebugOutputEnvironment();
  };
}

#endif
