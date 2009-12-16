#ifndef __OSIX_SEMANTICDBDBG_H__
#define __OSIX_SEMANTICDBDBG_H__
//////////////////////////////////////////////////////////////////////////////
//
//    SEMANTICDBDBG.H
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
      semanticdb.h must be included before this header
*/
/*                                 INCLUDES                                 */

/*                                 CONSTANTS                                */
#define SEMANTICDBDBG_VERSION "0.1"

/*                                   TYPES                                  */

#ifdef __cplusplus
extern "C" {
#endif

/*                                     API                                  */
void OSI_API_C_CALL DebugOutputEnvironment();

#ifdef __cplusplus
}
#endif

#endif
