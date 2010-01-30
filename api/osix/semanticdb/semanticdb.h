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

#ifdef __cplusplus
extern "C" {
#endif

/*                                   TYPES                                  */
typedef OSuint32 SemanticId;

/*                                 CONSTANTS                                */
#define SEMANTICID_INVALID      0xffffffff
#define SEMANTICID_EPSILON      0xfffffffe
#define SEMANTICID_QUERY        0xfffffffd
#define SEMANTICID_INT16        0xfffffffc
#define SEMANTICID_INT32        0xfffffffb
#define SEMANTICID_INT64        0xfffffffa
#define SEMANTICID_UINT16       0xfffffff9
#define SEMANTICID_UINT32       0xfffffff8
#define SEMANTICID_UINT64       0xfffffff7
#define SEMANTICID_FLOAT        0xfffffff6
#define SEMANTICID_DOUBLE       0xfffffff5
#define SEMANTICID_CHAR         0xfffffff4
#define SEMANTICID_STRING       0xfffffff3

/*                                     API                                  */
/* symbols */
SemanticId OSI_API_C_CALL DeclareSymbol(const OSchar* name);
SemanticId OSI_API_C_CALL GlobalSymbol(const OSchar* name);

/* relations */

/** declarations **/
//SemanticId OSI_API_C_CALL DeclareRelation(const Relation& relation);
SemanticId OSI_API_C_CALL DeclareRelation(SemanticId domain, SemanticId codomain);

/** queries **/
SemanticId OSI_API_C_CALL SelectRelation(SemanticId domain, SemanticId codomain);

/* domains */
SemanticId OSI_API_C_CALL DeclareOpenDomain(const OSchar* name);
void OSI_API_C_CALL CloseDomain(const OSchar* name = 0);

/* builtin morphisms */
//SemanticId OSI_API_C_CALL AdditiveMutation(SemanticId domain, SemanticId crossSection);

/* queries */
void OSI_API_C_CALL BeginQuery();
void OSI_API_C_CALL EndQuery();

SemanticId OSI_API_C_CALL SelectionDisjunct(SemanticId selection);
SemanticId OSI_API_C_CALL SelectionExclusiveDisjunct(SemanticId selection);
SemanticId OSI_API_C_CALL SelectionConjunct(SemanticId selection);
SemanticId OSI_API_C_CALL SelectionStrictConjunct(SemanticId selection);
//SemanticId OSI_API_C_CALL SelectionStrictExclusiveDisjunct(SemanticId domain, SemanticId selection);

SemanticId OSI_API_C_CALL MutationDisjunct(SemanticId mutation);
SemanticId OSI_API_C_CALL MutationExclusiveDisjunct(SemanticId mutation);
SemanticId OSI_API_C_CALL MutationConjunct(SemanticId mutation);
SemanticId OSI_API_C_CALL MutationStrictConjunct(SemanticId mutation);
//SemanticId OSI_API_C_CALL MutationStrictExclusiveDisjunct(SemanticId domain, SemanticId mutation);

/* evaluation */
OSobject OSI_API_C_CALL BeginEvaluation(SemanticId query);
SemanticId OSI_API_C_CALL Eval();
SemanticId OSI_API_C_CALL GetEvalDomain();
const OSchar* OSI_API_C_CALL GetEvalString();
OSint16 OSI_API_C_CALL GetEvalInt16();
OSint32 OSI_API_C_CALL GetEvalInt32();
OSint64 OSI_API_C_CALL GetEvalInt64();
OSfloat OSI_API_C_CALL GetEvalFloat();
OSdouble OSI_API_C_CALL GetEvalDouble();
void OSI_API_C_CALL EndEvaluation(OSobject evaluation);

SemanticId OSI_API_C_CALL GetDomain(SemanticId symbol);
const OSchar* OSI_API_C_CALL GetString(SemanticId symbol);
OSint16 OSI_API_C_CALL GetInt16(SemanticId symbol);
OSint32 OSI_API_C_CALL GetInt32(SemanticId symbol);
OSint64 OSI_API_C_CALL GetInt64(SemanticId symbol);
OSfloat OSI_API_C_CALL GetFloat(SemanticId symbol);
OSdouble OSI_API_C_CALL GetDouble(SemanticId symbol);

/* debug */
void* OSI_API_C_CALL SemanticDBDebugInit();

/* initialization */
void OSI_API_C_CALL SemanticDBInit();


#ifdef __cplusplus
}
#endif

#endif
