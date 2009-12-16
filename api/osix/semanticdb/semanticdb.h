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
typedef OSuint32 SemanticId;

#ifdef __cplusplus
extern "C" {
#endif
/*                                     API                                  */
/* symbols */
SemanticId OSI_API_C_CALL DeclareSymbol(const OSchar* name);    

/* relations */
//SemanticId OSI_API_C_CALL DeclareRelation(const Relation& relation);    
SemanticId DeclareRelation(SemanticId domain, SemanticId codomain);

/* domains */
SemanticId OSI_API_C_CALL DeclareOpenDomain(const OSchar* name);
void OSI_API_C_CALL CloseDomain(const OSchar* name = 0);

/* initialization */
void OSI_API_C_CALL SemanticDBInit();

/* debug */
void* OSI_API_C_CALL SemanticDBDebugInit();

#ifdef __cplusplus
}
#endif

#endif
