#ifndef __OSIX_PARSERDBG_H__
#define __OSIX_PARSERDBG_H__
//////////////////////////////////////////////////////////////////////////////
//
//    PARSERDBG.H
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
      parser.h must be included before this header
*/
/*                                 INCLUDES                                 */

/*                                 CONSTANTS                                */
#define PARSERDBG_VERSION 0.2

/*                                   TYPES                                  */

#ifdef __cplusplus
extern "C" {
#endif

/*                                     API                                  */
OSI_DYNAMIC_METHOD void OSI_API_CALL debugOutputTokens();
OSI_DYNAMIC_METHOD void OSI_API_CALL debugOutputProductions();
OSI_DYNAMIC_METHOD void OSI_API_CALL debugOutputGrammar();
OSI_DYNAMIC_METHOD void OSI_API_CALL debugOutputParseResult(OSobject parseResult);

#ifdef __cplusplus
}
#endif

#endif
