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
  return &semanticDB; 
}

// OSIX
OSIX::SemanticDB* OSI_API_CALL OSIX::SemanticDBInit()
{
  QSemanticDB::SemanticDB* semanticDB = new QSemanticDB::SemanticDB();
  (*semanticDB)->Init();
  return static_cast<OSIX::SemanticDB*>(semanticDB);
}

/*void SemanticDB::Init()
{
  // Create the semantic db implementation
  if (semanticDB)
    delete semanticDB;
  semanticDB = new QSemanticDB::SemanticDBImplementation();
  semanticDB->Init();
}*/

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::DeclareSymbol(const OSchar* name)
{
  return _this->DeclareSymbol(name);
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::GlobalSymbol(const OSchar* name)
{
  return _this->GlobalSymbol(name);
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::DeclareRelation(const Relation& relation)
{
  return _this->DeclareRelation(relation);
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::SelectRelation(const Relation& relation)
{
  std::vector<OSIX::SemanticId> qualifiedCodomains;
  _this->SelectRelation(relation, qualifiedCodomains);
  if (qualifiedCodomains.empty())
    return -1; // todo: return invalidId
  return qualifiedCodomains[0];
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

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::SelectionDisjunct(OSIX::SemanticId domain, OSIX::SemanticId selection)
{
   return _this->SelectionDisjunct(domain, selection);
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::SelectionExclusiveDisjunct(OSIX::SemanticId domain, OSIX::SemanticId selection)
{
  return _this->SelectionDisjunct(domain, selection);
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::SelectionConjunct(OSIX::SemanticId domain, OSIX::SemanticId selection)
{
  return _this->SelectionConjunct(domain, selection);
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::SelectionStrictConjunct(OSIX::SemanticId domain, OSIX::SemanticId selection)
{
  return _this->SelectionStrictConjunct(domain, selection);
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::MutationDisjunct(OSIX::SemanticId domain, OSIX::SemanticId mutation)
{
  return _this->MutationDisjunct(domain, mutation);
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::MutationExclusiveDisjunct(OSIX::SemanticId domain, OSIX::SemanticId mutation)
{
  return _this->MutationExclusiveDisjunct(domain, mutation);
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::MutationConjunct(OSIX::SemanticId domain, OSIX::SemanticId mutation)
{
  return _this->MutationConjunct(domain, mutation);
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::MutationStrictConjunct(OSIX::SemanticId domain, OSIX::SemanticId mutation)
{
  return _this->MutationStrictConjunct(domain, mutation);
}

OSobject OSI_API_CALL OSIX::SemanticDB::BeginEvaluation(OSIX::SemanticId query)
{
  _this->BeginEvaluation(query);
  return 0; // todo...
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::Eval()
{
  return _this->Eval();
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::GetEvalDomain()
{
  //return _this->GetDomain(_this->GetEvalQuery());
  return _this->GetDomain(_this->GetActiveQuery());
}

const OSchar* OSI_API_CALL OSIX::SemanticDB::GetEvalString()
{
  return _this->GetString(_this->GetActiveQuery());
}

OSint16 OSI_API_CALL OSIX::SemanticDB::GetEvalInt16()
{
  return _this->GetInt16(_this->GetActiveQuery());
}

OSint32 OSI_API_CALL OSIX::SemanticDB::GetEvalInt32()
{
  return _this->GetInt32(_this->GetActiveQuery());
}

OSint64 OSI_API_CALL OSIX::SemanticDB::GetEvalInt64()
{
  return _this->GetInt64(_this->GetActiveQuery());
}

OSfloat OSI_API_CALL OSIX::SemanticDB::GetEvalFloat()
{
  return _this->GetFloat(_this->GetActiveQuery());
}

OSdouble OSI_API_CALL OSIX::SemanticDB::GetEvalDouble()
{
  return _this->GetDouble(_this->GetActiveQuery());
}

void OSI_API_CALL OSIX::SemanticDB::EndEvaluation(OSobject evaluation)
{
  _this->EndEvaluation();
}

OSIX::SemanticId OSI_API_CALL OSIX::SemanticDB::GetDomain(SemanticId symbol)
{
  return _this->GetDomain(symbol);
}

const OSchar* OSI_API_CALL OSIX::SemanticDB::GetString(SemanticId symbol)
{
  return _this->GetString(symbol);
}

OSint16 OSI_API_CALL OSIX::SemanticDB::GetInt16(SemanticId symbol)
{
  return _this->GetInt16(symbol);
}

OSint32 OSI_API_CALL OSIX::SemanticDB::GetInt32(SemanticId symbol)
{
  return _this->GetInt32(symbol);
}

OSint64 OSI_API_CALL OSIX::SemanticDB::GetInt64(SemanticId symbol)
{
  return _this->GetInt64(symbol);
}

OSfloat OSI_API_CALL OSIX::SemanticDB::GetFloat(SemanticId symbol)
{
  return _this->GetFloat(symbol);
}

OSdouble OSI_API_CALL OSIX::SemanticDB::GetDouble(SemanticId symbol)
{
  return _this->GetDouble(symbol);
}

void* OSI_API_CALL OSIX::SemanticDB::DebugInit()
{
# ifdef _DEBUG
    return &_this.semanticDBDbg;
# else
    return null;
# endif
}

#endif
#endif
