#ifdef  __QSEMANTICDB_API_H__
#ifndef __QSEMANTICDB_API_INL__
#define __QSEMANTICDB_API_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    API.INL
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              COMPILER MACROS                             */
#define _this (*(QSemanticDB::SemanticDB*)this)

#define cast_id     QSemanticDB::SemanticDB::cast_id
#define cast_object QSemanticDB::SemanticDB::cast_object

/*                              IMPLEMENTATION                              */
OSIX::SemanticDB* OSI_API_CALL OSIX::semanticDBInit()
{
  return static_cast<OSIX::SemanticDB*>(new QSemanticDB::SemanticDB());
}

#endif
#endif
