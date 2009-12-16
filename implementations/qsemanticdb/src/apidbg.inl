#if defined(__QSEMANTICDB_API_H__) && defined(_DEBUG)
#ifndef __QSEMANTICDB_APIDBG_INL__
#define __QSEMANTICDB_APIDBG_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    APIDBG.INL
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              COMPILER MACROS                             */
//#define _this (*(QSemanticDB::SemanticDB*)this)

/*                              IMPLEMENTATION                              */
// OSIX
void OSI_API_CALL OSIX::SemanticDBDbg::DebugOutputEnvironment()
{
  QSemanticDB::SemanticDB::SemanticDBDbg* semanticDBDbg = static_cast<QSemanticDB::SemanticDB::SemanticDBDbg*>(this);
  if(semanticDBDbg)
    semanticDBDbg->GetSemanticDB()->DebugOutputEnvironment();
}

#endif
#endif
