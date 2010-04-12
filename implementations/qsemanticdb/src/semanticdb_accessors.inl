#ifdef  __QSEMANTICDB_SEMANTICDB_H__
#ifndef __QSEMANTICDB_SEMANTICDB_ACCESSORS_INL__
#define __QSEMANTICDB_SEMANTICDB_ACCESSORS_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    SEMANTICDB_ACCESSORS.INL
//
//    Copyright © 2009-2010, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QSemanticDB
{
  SemanticId SemanticDBImplementation::GetUnqualifiedCodomain(SemanticId qualifiedId)
  {
    RelationIndex::left_iterator i = relations.left.find(qualifiedId);
    if(i == relations.left.end())
      return OSIX::SEMANTICID_INVALID;
    return i->second.codomain;
  }

  bool SemanticDBImplementation::GetUnqualifiedRelation(SemanticId qualifiedId, Relation& unqualifiedRelation) const
  {
    RelationIndex::left_const_iterator i = relations.left.find(qualifiedId);
    if(i == relations.left.end())
      return false;
    unqualifiedRelation = i->second;
    return true;
  }

  inline SemanticId SemanticDBImplementation::GetQualifiedCodomain(SemanticId domain, SemanticId unqualifiedCodomain) const
  {
    return GetQualifiedCodomain(OrderedRelation(domain, unqualifiedCodomain));
  }

  SemanticId SemanticDBImplementation::GetQualifiedCodomain(const OrderedRelation& unqualifiedRelation) const
  {
    RelationIndex::right_const_iterator i = relations.right.find(unqualifiedRelation);
    return i == relations.right.end()? OSIX::SEMANTICID_INVALID : i->second;
  }

  inline SemanticId SemanticDBImplementation::GetConcreteQualifiedCodomain(SemanticId domain, SemanticId unqualifiedCodomain) const
  {
    return GetConcreteQualifiedCodomain(OrderedRelation(domain, unqualifiedCodomain));
  }

  SemanticId SemanticDBImplementation::GetConcreteQualifiedCodomain(const OrderedRelation& unqualifiedRelation) const
  {
    RelationIndex::right_const_iterator i = relations.right.find(unqualifiedRelation);
    if(i == relations.right.end())
      return OSIX::SEMANTICID_INVALID;

    OSid qualifiedCodomain = i->second;

    IdPropertiesConstIterator iProperties = symbolProperties.find(qualifiedCodomain);
    if(iProperties == symbolProperties.end())
      return qualifiedCodomain; // Symbol is concrete if no codomain could be found

    const SymbolProperties &properties = iProperties->second;
    return properties.concrete? qualifiedCodomain : OSIX::SEMANTICID_INVALID;
  }

  bool SemanticDBImplementation::HasStaticRelations(SemanticId domain) const
  {
    return domainIndexQCodomains.find(domain) != domainIndexQCodomains.end();
  }

  const SemanticDBImplementation::SymbolProperties& SemanticDBImplementation::GetProperties(SemanticId domain) const
  {
    static const SymbolProperties defaultProperties;
    IdPropertiesMap::const_iterator i = symbolProperties.find(domain);
    if(i == symbolProperties.end())
      return defaultProperties;
    return i->second;
  }

  bool SemanticDBImplementation::Decompose(QualifiedSymbol& qualifiedSymbol, SemanticId symbolId) const
  {
    // Split the query symbol into a speculative domain and the unqualified query argument
    RelationIndex::left_const_iterator iRelation = relations.left.find(qualifiedSymbol);
    if(iRelation == relations.left.end())
      return false; // Could not find the relation in the database. symbolId is either an atom or an undefined id.
    const OrderedRelation& relation = iRelation->second;

    // Construct the result
    qualifiedSymbol = symbolId;
    qualifiedSymbol.domain = relation.domain;
    qualifiedSymbol.codomain = relation.codomain;
    return true;
  }

  bool SemanticDBImplementation::Decompose(QuerySymbol& querySymbol, SemanticId symbolId) const
  {
    // Split the query symbol into a speculative domain and the unqualified query argument
    RelationIndex::left_const_iterator iQueryRelation = relations.left.find(symbolId);
    if(iQueryRelation == relations.left.end())
      return false; // FATAL ERROR: Could not find the relation for a query in the database. This should never happen and should be reported as bug!
    const OrderedRelation& queryRelation = iQueryRelation->second;

    // Split the speculative domain into the parent domain and the query domain
    RelationIndex::left_const_iterator iDomainRelation = relations.left.find(queryRelation.domain);
    if(iDomainRelation == relations.left.end())
      return false; // ERROR (TODO): Could not find a query domain in the database. Not yet sure what to do in this case...;
    const OrderedRelation& domainRelation = iDomainRelation->second;

    // Construct the result
    querySymbol = symbolId;
    querySymbol.speculativeDomain = queryRelation.domain;
    querySymbol.speculativeDomain.parentDomain = domainRelation.domain;
    querySymbol.speculativeDomain.queryDomain = domainRelation.codomain;
    querySymbol.argument = queryRelation.codomain;
    return true;
  }

  /*SemanticId SemanticDBImplementation::GetRelationId(SemanticId domain, SemanticId codomain)
  {
    RelationIdMap::iterator i = relations.find(Relation(domain, codomain));
    if(i == relations.end())
      return invalidId;
    return i->second;
  }*/

  /*SemanticId SemanticDBImplementation::GetTailId(SemanticId relationId)
  {
    RelationTailIdMap::iterator i = unqualifiedCodomains.find(relationId);
    if(i == unqualifiedCodomains.end())
      return invalidId;
    return i->second;
  }*/

  /*bool SemanticDBImplementation::DoesRelationExist(const OrderedRelation& unqualifiedRelation) const
  {
    return relations.right.find(unqualifiedRelation) != relations.right.end();
  }*/

  /*bool SemanticDBImplementation::MatchDomain(SemanticId qualifiedCodomain, SemanticId matchDomain)
  {

    Relation splitRelation(id, invalidId);
    do
    {
      UnqualifiedSplit(splitRelation.domain, splitRelation);
      OSI_ASSERT(splitRelation.domain >= id && splitRelation.domain != invalidId); // Make some assumptions based on the id assignment algorithm
    } while (splitRelation.domain != relation.domain);
    return (splitRelation.codomain == id);
    infoStream << "TODO..." << std::endl;
    return false;
  }*/
}

#endif
#endif
