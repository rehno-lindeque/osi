#ifdef  __QSEMANTICDB_SEMANTICDB_H__
#ifndef __QSEMANTICDB_SEMANTICDB_INL__
#define __QSEMANTICDB_SEMANTICDB_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    SEMANTICDB.INL
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QSemanticDB
{
  SemanticDBImplementation::SemanticDBImplementation() : nextTokenId(0)
  {
  }

  SemanticDBImplementation::~SemanticDBImplementation()
  {
  }
  
  SemanticId SemanticDBImplementation::DeclareSymbol(const char* name)
  {
    return DeclareRelation(activeDomainId, DeclareGlobal(name));
  }
  
  SemanticId SemanticDBImplementation::DeclareRelation(const Relation& relation)
  {
    return -1;
  }
  
  SemanticId SemanticDBImplementation::DeclareOpenDomain(const char* name)
  {
    SemanticId domainId = DeclareSymbol(name);
    environment.push(domainId);
    activeDomainId = domainId;
    return -1;
  }
  
  void SemanticDBImplementation::CloseDomain(const char* name)
  {
    // todo: test if name matches the opened domain
    CloseDomain();
  }
  
  void SemanticDBImplementation::CloseDomain()
  {
    environment.pop();
    activeDomainId = environment.top();
  }
  
  SemanticId SemanticDBImplementation::DeclareGlobal(const char* name)
  {
    OSI_ASSERT(name != null);
    const std::string nameString(name);
    SemanticId &id = tokens[nameString];
    if (id == SemanticId(-1))
    {
      id = nextTokenId;
      epsilonDomain.push_back(id);
      ++nextTokenId;
    }
    return id;
  }
  
  SemanticId SemanticDBImplementation::DeclareRelation(SemanticId domain, SemanticId codomain)
  {
    return DeclareRelation(Relation(domain, codomain));
  }
}

#endif
#endif
