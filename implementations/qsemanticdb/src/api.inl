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
// QSemanticDB
FORCE_INLINE QSemanticDB::SemanticDBImplementation* QSemanticDB::SemanticDB::operator-> ()
{ 
  return semanticDB; 
}

// OSIX
OSIX::SemanticDB* OSI_API_CALL OSIX::semanticDBInit()
{
  return static_cast<OSIX::SemanticDB*>(new QSemanticDB::SemanticDB());
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::DeclareSymbol(const OSchar* name)
{
  return _this->DeclareSymbol(name);
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::DeclareRelation(const Relation& relation)
{
  return _this->DeclareRelation(relation);
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::DeclareOpenDomain(const OSchar* name)
{
  return _this->DeclareOpenDomain(name);
}

void OSI_API_CALL OSIX::SemanticDB::CloseDomain(const OSchar* name)
{
  _this->CloseDomain(name);
}

void OSI_API_CALL OSIX::SemanticDB::CloseDomain()
{
  _this->CloseDomain();
}

#endif
#endif
