#ifndef __OSIX_SEMANTICDB_HPP__
#define __OSIX_SEMANTICDB_HPP__
//////////////////////////////////////////////////////////////////////////////
//
//    SEMANTICDB.HPP
//
//    Ver 0.1 (2009-08-21)
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Main OpenParser C++ interface.
*/
/*                                 INCLUDES                                 */
#include "../../osi/os.hpp"

/*                                 CONSTANTS                                */
#define SEMANTICDB_VERSION "0.1"

namespace OSIX
{
  /*                                   TYPES                                  */
  typedef OSuint32 SemanticId;

  const SemanticId SEMANTICID_INVALID    = 0xffffffff;
  const SemanticId SEMANTICID_EPSILON    = 0xfffffffe;
  const SemanticId SEMANTICID_QUERY      = 0xfffffffd;
  const SemanticId SEMANTICID_INT16      = 0xfffffffc;
  const SemanticId SEMANTICID_INT32      = 0xfffffffb;
  const SemanticId SEMANTICID_INT64      = 0xfffffffa;
  const SemanticId SEMANTICID_UINT16     = 0xfffffff9;
  const SemanticId SEMANTICID_UINT32     = 0xfffffff8;
  const SemanticId SEMANTICID_UINT64     = 0xfffffff7;
  const SemanticId SEMANTICID_FLOAT      = 0xfffffff6;
  const SemanticId SEMANTICID_DOUBLE     = 0xfffffff5;
  const SemanticId SEMANTICID_CHAR       = 0xfffffff4;
  const SemanticId SEMANTICID_STRING     = 0xfffffff3;

  /*                                     API                                  */
  class SemanticDB
  {
  protected:
    OSI_INLINE_METHOD SemanticDB() {}
    OSI_INLINE_METHOD SemanticDB(const SemanticDB&) {}

  public:
    /* symbols */
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL DeclareSymbol(const OSchar* name);
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL GlobalSymbol(const OSchar* name);

    /* relations */
    struct Relation
    {
      SemanticId domain;
      SemanticId codomain;
      OSI_INLINE_METHOD Relation(SemanticId domain, SemanticId codomain) : domain(domain), codomain(codomain) {}
    };

    /** declarations */
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL DeclareRelation(const Relation& relation);
    OSI_INLINE_METHOD SemanticId OSI_API_CALL DeclareRelation(SemanticId domain, SemanticId codomain) { return DeclareRelation(Relation(domain, codomain)); }

    /** queries **/
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL SelectRelation(const Relation& relation);
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL SelectRelation(SemanticId domain, SemanticId codomain) { return SelectRelation(Relation(domain, codomain)); }

    /* domains */
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL DeclareOpenDomain(const OSchar* name);
    OSI_DYNAMIC_METHOD void OSI_API_CALL CloseDomain(const OSchar* name);
    OSI_DYNAMIC_METHOD void OSI_API_CALL CloseDomain();

    /* queries */
    OSI_DYNAMIC_METHOD void OSI_API_CALL BeginQuery();
    OSI_DYNAMIC_METHOD void OSI_API_CALL EndQuery();

    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL SelectionDisjunct(SemanticId selection);
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL SelectionExclusiveDisjunct(SemanticId selection);
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL SelectionConjunct(SemanticId selection);
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL SelectionStrictConjunct(SemanticId selection);
    //OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL SelectionStrictExclusiveDisjunct(SemanticId selection);

    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL MutationDisjunct(SemanticId mutation);
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL MutationExclusiveDisjunct(SemanticId mutation);
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL MutationConjunct(SemanticId mutation);
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL MutationStrictConjunct(SemanticId mutation);
    //OSI_DYNAMIC_METHOD  SemanticId OSI_API_CALL MutationStrictExclusiveDisjunct(SemanticId mutation);

    // OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL SelectiveMutation(SemanticId domain, SemanticId mutation); // Conjunctive // Injective/selective morphism
    // OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL AssertiveMutation(SemanticId domain, SemanticId mutation);
    // OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL Disjunctive/AdditiveMutation/UnifyingQuery(SemanticId domain, SemanticId mutation);

    // OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL SelectiveQuery(SemanticId domain, SemanticId query);
    // OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL AssertiveQuery(SemanticId domain, SemanticId query);
    // OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL AdditiveQuery/UnifyingQuery(SemanticId domain, SemanticId query);

    /* evaluation */
    OSI_DYNAMIC_METHOD OSobject OSI_API_CALL BeginEvaluation(SemanticId query);
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL Eval();
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL GetEvalDomain();
    OSI_DYNAMIC_METHOD const OSchar* OSI_API_CALL GetEvalString();
    OSI_DYNAMIC_METHOD OSint16 OSI_API_CALL GetEvalInt16();
    OSI_DYNAMIC_METHOD OSint32 OSI_API_CALL GetEvalInt32();
    OSI_DYNAMIC_METHOD OSint64 OSI_API_CALL GetEvalInt64();
    OSI_DYNAMIC_METHOD OSfloat OSI_API_CALL GetEvalFloat();
    OSI_DYNAMIC_METHOD OSdouble OSI_API_CALL GetEvalDouble();
    OSI_DYNAMIC_METHOD void OSI_API_CALL EndEvaluation(OSobject evaluation);

    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL GetDomain(SemanticId symbol);
    OSI_DYNAMIC_METHOD const OSchar* OSI_API_CALL GetString(SemanticId symbol);
    OSI_DYNAMIC_METHOD OSint16 OSI_API_CALL GetInt16(SemanticId symbol);
    OSI_DYNAMIC_METHOD OSint32 OSI_API_CALL GetInt32(SemanticId symbol);
    OSI_DYNAMIC_METHOD OSint64 OSI_API_CALL GetInt64(SemanticId symbol);
    OSI_DYNAMIC_METHOD OSfloat OSI_API_CALL GetFloat(SemanticId symbol);
    OSI_DYNAMIC_METHOD OSdouble OSI_API_CALL GetDouble(SemanticId symbol);

    /* debug */
    OSI_DYNAMIC_METHOD void* OSI_API_CALL DebugInit();
  };

  /* initialization */
  SemanticDB* OSI_API_CALL SemanticDBInit();
}

#endif
