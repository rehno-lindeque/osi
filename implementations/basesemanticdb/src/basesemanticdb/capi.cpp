//////////////////////////////////////////////////////////////////////////////
//
//    CAPI.CPP
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              IMPLEMENTATION                              */
#if defined(OSI_C_STATIC_BUILD) || defined(OSI_C_DYNAMIC_BUILD)

#include <osix/semanticdb/semanticdb.hpp>
#include <osix/semanticdb/semanticdb.h>
#ifdef _DEBUG
# include <osix/semanticdb/semanticdbdbg.hpp>
# include <osix/semanticdb/semanticdbdbg.h>
#endif
#include <base/common/types.h>

OSIX::SemanticDB *semanticDB = null;
#ifdef _DEBUG
OSIX::SemanticDBDbg *semanticDBDbg = null;
#endif

extern "C" {

SemanticId OSI_API_C_CALL DeclareSymbol(const OSchar* name)
{
  return semanticDB->DeclareSymbol(name);
}

SemanticId OSI_API_C_CALL GlobalSymbol(const OSchar* name)
{
  return semanticDB->GlobalSymbol(name);
}

//SemanticId OSI_API_C_CALL DeclareRelation(const Relation& relation);

SemanticId DeclareRelation(SemanticId domain, SemanticId codomain)
{
  return semanticDB->DeclareRelation(domain, codomain);
}

SemanticId SelectRelation(SemanticId domain, SemanticId codomain)
{
  return semanticDB->SelectRelation(domain, codomain);
}

SemanticId OSI_API_C_CALL DeclareOpenDomain(const OSchar* name)
{
  return semanticDB->DeclareOpenDomain(name);
}

void OSI_API_C_CALL CloseDomain(const OSchar* name)
{
  if (name == 0)
    semanticDB->CloseDomain();
  else
    semanticDB->CloseDomain(name);
}

void OSI_API_C_CALL BeginQuery()
{
  return semanticDB->BeginQuery();
}

SemanticId OSI_API_C_CALL EndQuery()
{
  return semanticDB->EndQuery();
}

void OSI_API_C_CALL SelectionDisjunct()
{
  semanticDB->SelectionDisjunct();
}

void OSI_API_C_CALL SelectionExclusiveDisjunct()
{
  semanticDB->SelectionExclusiveDisjunct();
}

void OSI_API_C_CALL SelectionConjunct()
{
  semanticDB->SelectionConjunct();
}

void OSI_API_C_CALL SelectionStrictConjunct()
{
  semanticDB->SelectionStrictConjunct();
}

void OSI_API_C_CALL MutationDisjunct()
{
  semanticDB->MutationDisjunct();
}

void OSI_API_C_CALL MutationExclusiveDisjunct()
{
  semanticDB->MutationExclusiveDisjunct();
}

void OSI_API_C_CALL MutationConjunct()
{
  semanticDB->MutationConjunct();
}

void OSI_API_C_CALL MutationStrictConjunct()
{
   semanticDB->MutationStrictConjunct();
}

OSobject OSI_API_C_CALL BeginEvaluation(SemanticId root)
{
  return semanticDB->BeginEvaluation(root);
}

SemanticId OSI_API_C_CALL Eval()
{
  return semanticDB->Eval();
}

SemanticId OSI_API_C_CALL GetEvalDomain()
{
  return semanticDB->GetEvalDomain();
}

const OSchar* OSI_API_C_CALL GetEvalString()
{
  return semanticDB->GetEvalString();
}

OSint16 OSI_API_C_CALL GetEvalInt16()
{
  return semanticDB->GetEvalInt16();
}

OSint32 OSI_API_C_CALL GetEvalInt32()
{
  return semanticDB->GetEvalInt32();
}

OSint64 OSI_API_C_CALL GetEvalInt64()
{
  return semanticDB->GetEvalInt64();
}

OSfloat OSI_API_C_CALL GetEvalFloat()
{
  return semanticDB->GetEvalFloat();
}

OSdouble OSI_API_C_CALL GetEvalDouble()
{
  return semanticDB->GetEvalDouble();
}

void OSI_API_C_CALL EndEvaluation(OSobject evaluation)
{
  return semanticDB->EndEvaluation(evaluation);
}

SemanticId OSI_API_C_CALL GetDomain(SemanticId symbol)
{
  return semanticDB->GetDomain(symbol);
}

const OSchar* OSI_API_C_CALL GetString(SemanticId symbol)
{
  return semanticDB->GetString(symbol);
}

OSint16 OSI_API_C_CALL GetInt16(SemanticId symbol)
{
  return semanticDB->GetInt16(symbol);
}

OSint32 OSI_API_C_CALL GetInt32(SemanticId symbol)
{
  return semanticDB->GetInt32(symbol);
}

OSint64 OSI_API_C_CALL GetInt64(SemanticId symbol)
{
  return semanticDB->GetInt64(symbol);
}

OSfloat OSI_API_C_CALL GetFloat(SemanticId symbol)
{
  return semanticDB->GetFloat(symbol);
}

OSdouble OSI_API_C_CALL GetDouble(SemanticId symbol)
{
  return semanticDB->GetDouble(symbol);
}

void* OSI_API_C_CALL SemanticDBDebugInit()
{
  void* dbg = semanticDB->DebugInit();
#ifdef _DEBUG
  semanticDBDbg = static_cast<OSIX::SemanticDBDbg*>(dbg);
#endif
  return dbg;
}

void OSI_API_C_CALL SemanticDBInit() { semanticDB = OSIX::SemanticDBInit(); }

#ifdef _DEBUG
void OSI_API_CALL DebugOutputEnvironment()
{
  if (semanticDBDbg)
    semanticDBDbg->DebugOutputEnvironment();
}
#endif

}

#endif
