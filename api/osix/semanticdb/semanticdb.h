#ifndef __OSIX_SEMANTICDB_H__
#define __OSIX_SEMANTICDB_H__
//////////////////////////////////////////////////////////////////////////////
//
//    SEMANTICDB.H
//
//    Ver 0.1 (2009-08-21)
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Main OpenSemanticDB C interface.
*/
/*                                 INCLUDES                                 */
#include "../../osi/os.h"

/*                                 CONSTANTS                                */
#define PARSER_VERSION "0.1"
#define PARSER_TOKEN_VALUE_EOF "\x01"

/*                                   TYPES                                  */

#ifdef __cplusplus
extern "C" {
#endif

/*                                     API                                  */
/* debug */
//void* OSI_API_C_CALL debugInit();
  
void OSI_API_C_CALL parserInit();

#ifdef __cplusplus
}
#endif

#endif
