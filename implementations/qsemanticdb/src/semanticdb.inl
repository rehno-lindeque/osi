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
  SemanticDBImplementation::SemanticDBImplementation()
  {
  }

  SemanticDBImplementation::~SemanticDBImplementation()
  {
  }
  
  SemanticId SemanticDBImplementation::DeclareSymbol(const char* name)
  {
    return DeclareRelation(activeDomainId, DeclareGlobal(name));
  }
  
  SemanticId SemanticDBImplementation::DeclareRelation(Relation& relation)
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
    activeDomainId = environment.peek();
  }
  
  SemanticId SemanticDBImplementation::DeclareGlobal(const char* name)
  {
    epsilonDomain.push(name)
  }
  
  OSIX::SemanticId DeclareRelation(SemanticId domain, SemanticId codomain)
  {
    DeclareRelation(Relation(domain, codomain))
  }
}

#endif
#endif
