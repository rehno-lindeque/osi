#ifdef  __QSEMANTICDB_SEMANTICDB_H__
#ifndef __QSEMANTICDB_SEMANTICDB_BUILD_INL__
#define __QSEMANTICDB_SEMANTICDB_BUILD_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    SEMANTICDB_BUILD.INL
//
//    Copyright © 2009-2010, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QSemanticDB
{
  SemanticId SemanticDBImplementation::CreateQualifiedId(const Relation& relation)
  {
    SemanticId id = nextSymbolId;
    OSI_ASSERT(id < firstReservedId);
    ++nextSymbolId;
    return id;
  }

  SemanticId SemanticDBImplementation::DeclareRelation(SemanticId domain, SemanticId codomain)
  {
    return DeclareRelation(Relation(domain, codomain));
  }

  SemanticId SemanticDBImplementation::DeclareSpeculativeRelation(SemanticId domain, SemanticId codomain)
  {
    return DeclareSpeculativeRelation(Relation(domain, codomain));
  }

  SemanticId SemanticDBImplementation::DeclareSpeculativeRelation(const Relation& unqualifiedRelation)
  {
    // First test whether the relation already exists (in which case there is no need to create a "speculative" relation
    RelationIndex::right_const_iterator i = relations.right.find(unqualifiedRelation);
    if(i != relations.right.end())
    {
      return i->second;
    }

    // No concrete relation exists for the given pair, so create a speculative one
    SemanticId qualifiedCodomain = unqualifiedRelation.codomain;
    if(unqualifiedRelation.domain != OSIX::SEMANTICID_EPSILON)
    {
      // TODO: SHOULD unqualifiedRelation.domain == OSIX::SEMANTICID_EPSILON be possible.
      //       Probably not - an error should be given if not.

      qualifiedCodomain = CreateQualifiedId(unqualifiedRelation);
      domainIndexUCodomains.insert(std::make_pair(unqualifiedRelation.domain, unqualifiedRelation.codomain));
      domainIndexQCodomains.insert(std::make_pair(unqualifiedRelation.domain, qualifiedCodomain));
      domainIndexSpeculativeQCodomains.insert(std::make_pair(unqualifiedRelation.domain, qualifiedCodomain));
      relations.insert(RelationIndexValue(qualifiedCodomain, unqualifiedRelation));

      // Add to domains
      //queryEnvironment.push

      // Add the speculative flag to the relation (opposite of concrete)
      symbolProperties.insert(std::make_pair(qualifiedCodomain, SymbolProperties(false, QueryNone)));
    }

#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "DeclareSpeculativeRelation ("
               << unqualifiedRelation.domain <<  "->" << unqualifiedRelation.codomain
               << " = " << qualifiedCodomain << ')' << std::endl;
#endif
    return qualifiedCodomain;
  }

  SemanticId SemanticDBImplementation::DeclareSpeculativeAnonymousRelation(SemanticId domain, SemanticId anonymousCodomain)
  {
    return DeclareSpeculativeAnonymousRelation(Relation(domain, anonymousCodomain));
  }

  SemanticId SemanticDBImplementation::DeclareSpeculativeAnonymousRelation(const Relation& unqualifiedRelation)
  {
    if(unqualifiedRelation.domain != OSIX::SEMANTICID_EPSILON)
    {
      domainIndexUCodomains.insert(std::make_pair(unqualifiedRelation.domain, unqualifiedRelation.codomain));
      domainIndexQCodomains.insert(std::make_pair(unqualifiedRelation.domain, unqualifiedRelation.codomain));
      domainIndexSpeculativeQCodomains.insert(std::make_pair(unqualifiedRelation.domain, unqualifiedRelation.codomain));
      relations.insert(RelationIndexValue(unqualifiedRelation.codomain, unqualifiedRelation));

      // Add the speculative flag to the relation (opposite of concrete)
      symbolProperties.insert(std::make_pair(unqualifiedRelation.codomain, SymbolProperties(false, QueryNone)));
    }

#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "DeclareSpeculativeAnonymousRelation ("
               << unqualifiedRelation.domain <<  "->" << unqualifiedRelation.codomain
               << " = " << unqualifiedRelation.codomain << ')' << std::endl;
#endif
    return unqualifiedRelation.codomain;
  }

  void SemanticDBImplementation::OpenQueryArgument(QueryType queryType)
  {
    // Change the query state
    OSI_ASSERT(environmentState.top() == BUILD_QUERY_SOURCE);
    environmentState.pop();
    environmentState.push(static_cast<EnvironmentState>(BUILD_QUERY_ARGUMENT | queryType));
  }

  void SemanticDBImplementation::CreateQuery(QueryType type, SemanticId qualifiedId)
  {
    // Set a flag indicating that the id represents a query
    // (Only if the symbol properties do not yet exist in the set, definitions always take precedence to queries).

    /*IdPropertiesIterator i = symbolProperties.find(qualifiedId);
    if(i == symbolProperties.end())
    {*/
      // TODO: What if more than one query is put in the same context... ???? We need some way to resolve that...
      symbolProperties.insert(std::make_pair(qualifiedId, SymbolProperties(true, type)));
    /*}*/
  }

  /*SemanticId SemanticDBImplementation::CreateQuery(QueryType type, SemanticId domain, SemanticId argument)
  {
    const Query query(type, domain, argument);
    QueryIndex::right_iterator i = queries.right.find(query);

    // Declare or get the query id representing this query
    SemanticId queryId;
    if(i == queries.right.end())
    {
      queryId = DeclareAnonymousSymbol();
      queries.insert(QueryIndexValue(queryId, query));
    }

    return queryId;
  }*/
}

#endif
#endif
