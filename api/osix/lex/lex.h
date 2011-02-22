#ifndef __OSIX_LEX_H__
#define __OSIX_LEX_H__
//////////////////////////////////////////////////////////////////////////////
//
//    LEX.H
//
//    Ver 0.1 (2009-09-19)
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Main OpenLex C interface.
*/
/*                                 INCLUDES                                 */
#include "../../osi/os.h"

/*                                 CONSTANTS                                */
#define LEX_VERSION "0.1"
#define LEX_TOKEN_VALUE_EOF "\x01"

/*                                   TYPES                                  */

#ifdef __cplusplus
extern "C" {
#endif

/*                                     API                                  */
/* debug */
//void* OSI_API_C_CALL DebugInit();
  
void OSI_API_C_CALL LexInit();

#ifdef __cplusplus
}
#endif

#endif
