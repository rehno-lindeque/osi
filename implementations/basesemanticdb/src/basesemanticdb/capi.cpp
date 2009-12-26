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

extern "C" SemanticId OSI_API_C_CALL DeclareSymbol(const OSchar* name)
{
  return semanticDB->DeclareSymbol(name);
}

extern "C" SemanticId OSI_API_C_CALL GlobalSymbol(const OSchar* name)
{
  return semanticDB->GlobalSymbol(name);
}

//extern "C" SemanticId OSI_API_C_CALL DeclareRelation(const Relation& relation);    

extern "C" SemanticId DeclareRelation(SemanticId domain, SemanticId codomain)
{
  return semanticDB->DeclareRelation(domain, codomain);
}

extern "C" SemanticId SelectRelation(SemanticId domain, SemanticId codomain)
{
  return semanticDB->SelectRelation(domain, codomain);
}

extern "C" SemanticId OSI_API_C_CALL DeclareOpenDomain(const OSchar* name)
{
  return semanticDB->DeclareOpenDomain(name);
}

extern "C" void OSI_API_C_CALL CloseDomain(const OSchar* name)
{
  if (name == 0)
    semanticDB->CloseDomain();
  else
    semanticDB->CloseDomain(name);
}

extern "C" void* OSI_API_C_CALL SemanticDBDebugInit() 
{ 
  void* dbg = semanticDB->DebugInit(); 
#ifdef _DEBUG
  semanticDBDbg = static_cast<OSIX::SemanticDBDbg*>(dbg);
#endif
  return dbg; 
}

extern "C" void OSI_API_C_CALL SemanticDBInit() { semanticDB = OSIX::SemanticDBInit(); }

#ifdef _DEBUG
void OSI_API_CALL DebugOutputEnvironment()
{
  if (semanticDBDbg)
    semanticDBDbg->DebugOutputEnvironment();
}
#endif

#endif
