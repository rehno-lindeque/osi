//////////////////////////////////////////////////////////////////////////////
//
//    API.CPP
//
//    Copyright © 2007-2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    IMPLEMENTATION:
      + Parsing happens in packets of characters. Packet size must be a
        power of two.

    TODO:
      + Optimize using SSE and binary operations
      + The _this macro is not necesarily a very good idea since it could
        lead to confusion when a different object (such as QParser::ParserDbg)
        is used.
*/

/*                              COMPILER MACROS                             */
#ifdef _MSC_VER
# pragma warning(push)
#endif
/*                                 INCLUDES                                 */
#include "api.h"

#ifndef OSI_STATIC_C_BUILD
# include "api.inl"
#endif

/*                                  MACROS                                  */
#ifdef _MSC_VER
# pragma warning(pop)
#endif
