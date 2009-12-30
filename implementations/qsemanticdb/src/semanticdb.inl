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
  SemanticDBImplementation::SemanticDBImplementation() : 
    nextSymbolId(0),
    activeQueryId(OSIX::SEMANTICID_INVALID),
    activeDomainId(OSIX::SEMANTICID_EPSILON),
    errorStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out)),
    warnStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out)),
    infoStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out))
  {
      environment.push(OSIX::SEMANTICID_EPSILON);
  }

  SemanticDBImplementation::~SemanticDBImplementation()
  {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "Shutdown QSemanticDB implementation...";
#endif
    
    errorStream.flush();
    warnStream.flush();
    infoStream.flush();

    delete errorStream.rdbuf(null);
    delete warnStream.rdbuf(null);
    delete infoStream.rdbuf(null);
  }
  
  SemanticId SemanticDBImplementation::DeclareSymbol(const char* name)
  {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "DeclareSymbol (" << name << ")" << std::endl;
#endif
    SemanticId qualifiedId = DeclareRelation(activeDomainId, GlobalSymbol(name));
    return qualifiedId;
  }

  SemanticId SemanticDBImplementation::DeclareAnonymousSymbol()
  {
    SemanticId anonymousId = nextSymbolId;
    ++nextSymbolId;
    anonDomain.push_back(anonymousId);
    return anonymousId;
  }
  
  SemanticId SemanticDBImplementation::DeclareRelation(const Relation& unqualifiedRelation)
  {    
    SemanticId qualifiedCodomain = unqualifiedRelation.codomain;
    if(unqualifiedRelation.domain != OSIX::SEMANTICID_EPSILON)
    {
      qualifiedCodomain = CreateQualifiedId(unqualifiedRelation);
      domainIndexUCodomains.insert(std::make_pair(unqualifiedRelation.domain, unqualifiedRelation.codomain));
      domainIndexQCodomains.insert(std::make_pair(unqualifiedRelation.domain, qualifiedCodomain));
      relations.insert(RelationIndexValue(qualifiedCodomain, unqualifiedRelation));
    }

#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "DeclareRelation (" 
            << unqualifiedRelation.domain <<  "->" << unqualifiedRelation.codomain
            << " = " << qualifiedCodomain << ')' << std::endl;
#endif    
    return qualifiedCodomain;
  }
  
  //SemanticId SemanticDBImplementation::SelectRelation(const Relation& relation)
  void SemanticDBImplementation::SelectRelation(const Relation& unqualifiedRelation, IdVector& qualifiedCodomains)
  {
    //size_t initialSize = qualifiedCodomains.size();

#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "Select " << unqualifiedRelation.domain << '.' << unqualifiedRelation.codomain << std::endl;
#endif
    IdMultiIndexIterator i = domainIndexUCodomains.lower_bound(unqualifiedRelation.domain);
    while (i != domainIndexUCodomains.end() && i->first == unqualifiedRelation.domain)
    {
      // Check whether this qualified codomain matches the relation.
      // (May be either an exact or an inexact match. I.e. the codmain may refer to additional codomains as long as it is qualified by the same domain)
      if(unqualifiedRelation.codomain == i->second)
      {
        RelationIndex::right_iterator iRelation = relations.right.find(unqualifiedRelation);
        OSI_ASSERT(iRelation != relations.right.end());

#ifdef QSEMANTICDB_DEBUG_VERBOSE
        infoStream << "\tMatched " << iRelation->second << " = " << i->first << '.' << i->second << std::endl;
#endif
        qualifiedCodomains.push_back(iRelation->second);
      }

      // Increment iterator
      ++i;
    }

    // Check whether anything was selected
    //if(qualifiedCodomains.size() == initialSize)
  }
  
  SemanticId SemanticDBImplementation::DeclareOpenDomain(const char* name)
  {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "DeclareOpenDomain (" << name << ")" << std::endl;
#endif
    SemanticId domainId = DeclareSymbol(name);
    environment.push(domainId);
    activeDomainId = domainId;
    return domainId;
  }
  
  void SemanticDBImplementation::CloseDomain(const char* name)
  {
    // todo: test if name matches the opened domain
    CloseDomain();
  }
  
  void SemanticDBImplementation::CloseDomain()
  {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "CloseDomain" << std::endl;
#endif
    environment.pop();
    activeDomainId = environment.top();
  }
  
  void SemanticDBImplementation::Init()
  {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "...Initialized QSemanticDB" << std::endl;
#endif
  }

  /*SemanticId SemanticDBImplementation::ConjunctMorphism(SemanticId domain, SemanticId crossSection)
  {
    const Query query(QueryConjunctMorphism, domain, crossSection);
    QueryIndex::right_iterator i = queries.right.find(query);
    
    // Declare or get the query id representing this query
    SemanticId queryId;
    if (i == queries.right.end())
    {
      queryId = DeclareAnonymousSymbol();
      queries.insert(QueryIndexValue(queryId, query));
    }

    return queryId;
  }*/

  SemanticId SemanticDBImplementation::SelectionDisjunct(SemanticId domain, SemanticId selection)
  {
    return CreateQuery(QuerySelectionDisjunct, domain, selection);
  }

  SemanticId SemanticDBImplementation::SelectionExclusiveDisjunct(SemanticId domain, SemanticId selection)
  {
    return CreateQuery(QuerySelectionExclusiveDisjunct, domain, selection);
  }

  SemanticId SemanticDBImplementation::SelectionConjunct(SemanticId domain, SemanticId selection)
  {
    return CreateQuery(QuerySelectionConjunct, domain, selection);
  }
  
  SemanticId SemanticDBImplementation::SelectionStrictConjunct(SemanticId domain, SemanticId selection)
  {
    return CreateQuery(QuerySelectionStrictConjunct, domain, selection);
  }

  SemanticId SemanticDBImplementation::MutationDisjunct(SemanticId domain, SemanticId mutation)
  {
    return CreateQuery(QueryMutationDisjunct, domain, mutation);
  }

  SemanticId SemanticDBImplementation::MutationExclusiveDisjunct(SemanticId domain, SemanticId mutation)
  {
    return CreateQuery(QueryMutationExclusiveDisjunct, domain, mutation);
  }

  SemanticId SemanticDBImplementation::MutationConjunct(SemanticId domain, SemanticId mutation)
  {
    return CreateQuery(QueryMutationConjunct, domain, mutation);
  }

  SemanticId SemanticDBImplementation::MutationStrictConjunct(SemanticId domain, SemanticId mutation)
  {
    return CreateQuery(QueryMutationStrictConjunct, domain, mutation);
  }

  void SemanticDBImplementation::BeginEvaluation(SemanticId query)
  {
    evaluationQueries.push(query);
    activeQueries.push(activeQueryId);
    activeQueryId = OSIX::SEMANTICID_INVALID;
  }

  SemanticId SemanticDBImplementation::Eval()
  {
    // Advance the active query
    if (activeQueryId == OSIX::SEMANTICID_INVALID)
      activeQueryId = evaluationQueries.top();
    else
      activeQueryId = GetUnqualifiedCodomain(activeQueryId);
    return activeQueryId;
  }

  void SemanticDBImplementation::EndEvaluation()
  {
    if (activeQueries.empty())
    {
      activeQueryId = OSIX::SEMANTICID_INVALID;
    }
    else
    {
      activeQueryId = activeQueries.top();
      activeQueries.pop();
    }

    if (!evaluationQueries.empty())
      evaluationQueries.pop();      
  }

  SemanticId SemanticDBImplementation::GetActiveQuery()
  {
    return activeQueryId;
  }

  SemanticId SemanticDBImplementation::GetDomain(SemanticId qualifiedId)
  {
    RelationIndex::left_iterator i = relations.left.find(qualifiedId);
    if(i == relations.left.end())
      return OSIX::SEMANTICID_INVALID;
    return i->second.domain;
  }
  
  const char* SemanticDBImplementation::GetString(SemanticId symbol)
  {
    return 0;
  }

  int16 SemanticDBImplementation::GetInt16(SemanticId symbol)
  {
    return int16(GetUnqualifiedCodomain(symbol));
  }

  int32 SemanticDBImplementation::GetInt32(SemanticId symbol)
  {
    return int32(GetUnqualifiedCodomain(symbol));
  }

  int64 SemanticDBImplementation::GetInt64(SemanticId symbol)
  {
    // todo
    //GetCodomain(symbol);
    return 0;
  }

  float SemanticDBImplementation::GetFloat(SemanticId symbol)
  {
    SemanticId codomain = GetUnqualifiedCodomain(symbol);
    return reinterpret_cast<const float&>(codomain);
  }

  double SemanticDBImplementation::GetDouble(SemanticId symbol)
  {
    //todo
    return 0.0;
  }

  /*void EvalQuery(SemanticId query)
  {
     //IdMultiIndexRange r1 = domainIndexUCodomains.equal_range(domain);
    //IdMultiIndexRange r = domainIndexUCodomains.equal_range(crossSection);

    /*for(IdMultiIndexIterator i1 = r1.first; i1 != r1.second; ++i1)
    {
      for(IdMultiIndexIterator i2 = r2.first; i2 != r2.second; ++i2)
      {
        if(i1->second == i2->second)
        {
          DeclareRelation(i1->second, i2->second);
        }
      }
    }*/

    /*for(IdMultiIndexIterator i = r.first; i != r.second; ++i)
    {
      IdVector qualifiedCodomains;
      SelectExtendedRelations(Relation(domain, GetDomain(i->second)), qualifiedCodomains);

      for (IdVector::iterator iCodomain = qualifiedCodomains.begin(); iCodomain != qualifiedCodomains.end(); ++iCodomain)
      {
        SemanticId qualifiedCodomain = *iCodomain;
      }
    }*
  }*/
  
  void SemanticDBImplementation::SetErrorStream(FILE* stream)
  {
    errorStream.flush();
    delete errorStream.rdbuf(new STDEXT_NAMESPACE::stdio_filebuf<char>(stream, std::ios::out));
  }

  void SemanticDBImplementation::SetWarningStream(FILE* stream)
  {
    warnStream.flush();
    delete warnStream.rdbuf(new STDEXT_NAMESPACE::stdio_filebuf<char>(stream, std::ios::out));
  }

  void SemanticDBImplementation::SetInfoStream(FILE* stream)
  {
    infoStream.flush();
    delete infoStream.rdbuf(new STDEXT_NAMESPACE::stdio_filebuf<char>(stream, std::ios::out));
  }
  
  SemanticId SemanticDBImplementation::GlobalSymbol(const char* name)
  {
    OSI_ASSERT(name != null);
    if(name == null)
        return OSIX::SEMANTICID_INVALID;
    
    const std::string nameString(name);
    StringIdMap::iterator i = tokens.find(name);
    if(i != tokens.end())
      return i->second;
    
    SemanticId &id = tokens[nameString];
    id = nextSymbolId;
    epsilonDomain.push_back(id);
    epsilonStrings[id] = nameString;
    ++nextSymbolId;
    
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "GlobalSymbol (" << name << ',' << id << ")" << std::endl;
#endif

    return id;
  }
  
  SemanticId SemanticDBImplementation::DeclareRelation(SemanticId domain, SemanticId codomain)
  {
    return DeclareRelation(Relation(domain, codomain));
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

  SemanticId SemanticDBImplementation::GetUnqualifiedCodomain(SemanticId qualifiedId)
  {
    RelationIndex::left_iterator i = relations.left.find(qualifiedId);
    if(i == relations.left.end())
      return OSIX::SEMANTICID_INVALID;
    return i->second.codomain;
  }

  bool SemanticDBImplementation::GetUnqualifiedRelation(SemanticId qualifiedId, Relation& unqualifiedRelation)
  {
    RelationIndex::left_iterator i = relations.left.find(qualifiedId);
    if(i == relations.left.end())
      return false;
    unqualifiedRelation = i->second;
    return true;
  }
  
  bool SemanticDBImplementation::HasStaticRelations(SemanticId domain)
  {
    return domainIndexQCodomains.find(domain) != domainIndexQCodomains.end();
  }
  
  SemanticId SemanticDBImplementation::CreateQualifiedId(const Relation& relation)
  {
    SemanticId id = nextSymbolId;
    OSI_ASSERT(id < firstReservedId);
    ++nextSymbolId;
    return id;
  }

  SemanticId SemanticDBImplementation::CreateQuery(QueryType type, SemanticId domain, SemanticId argument)
  {
    const Query query(type, domain, argument);
    QueryIndex::right_iterator i = queries.right.find(query);

    // Declare or get the query id representing this query
    SemanticId queryId;
    if (i == queries.right.end())
    {
      queryId = DeclareAnonymousSymbol();
      queries.insert(QueryIndexValue(queryId, query));
    }

    return queryId;
  }

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

  /*void SemanticDBImplementation::SplitRelation(SemanticId relationId, Relation& qualifiedRelation)
  {
  }*/
  
#ifdef _DEBUG
  void SemanticDBImplementation::DebugOutputEnvironment()
  {
    using std::endl;
    
    // Output relations
    infoStream << "Unqualified Relations: (qualifiedCodomain = domain.unqualifiedCodomain)" << endl
               << "----------------------" << endl;
    for(RelationIndex::iterator i = relations.begin(); i != relations.end(); ++i)
      infoStream << i->left << " = " << i->right.domain << '.' << i->right.codomain << endl;
    infoStream << endl;
    
    
    // Output complete environment
    infoStream << "Environment: (domain -> (qualifiedCodomain = domain.unqualifiedCodomain))" << endl
               << "------------" << endl;
    infoStream << epsilonName << '(' << OSIX::SEMANTICID_EPSILON << ") -> " << endl
               << "[" << endl;
    for(IdVector::iterator i = epsilonDomain.begin(); i != epsilonDomain.end(); ++i)
    {
      DebugOutputSymbolEnvironment(OSIX::SEMANTICID_EPSILON, *i, 1);
    }
    infoStream << "]" << endl;
  }
  
  void SemanticDBImplementation::DebugOutputSymbolEnvironment(SemanticId domain, SemanticId qualifiedCodomain, uint indent)
  {    
    using std::endl;
    
    SemanticId unqualifiedCodomain = GetUnqualifiedCodomain(qualifiedCodomain);
    
    // Write the symbol
    for(uint c = 0; c < indent*2; ++c) infoStream << ' ';
    if(domain == OSIX::SEMANTICID_EPSILON)
    {
      //infoStream << epsilonName << ".";
      std::map<SemanticId, std::string>::iterator i = epsilonStrings.find(qualifiedCodomain);
      if(i == epsilonStrings.end())
        infoStream << "Anonymous";
      else
        infoStream << i->second;
      infoStream << '(' << qualifiedCodomain << ')' << " = " << qualifiedCodomain;
    }
    else if(unqualifiedCodomain == OSIX::SEMANTICID_INVALID)
      infoStream << "Invalid (no tail for " << domain << " -> " << qualifiedCodomain << ")";
    else
    {
      std::map<SemanticId, std::string>::iterator i = epsilonStrings.find(unqualifiedCodomain);
      if(i == epsilonStrings.end())
        infoStream << "Anonymous";
      else
        infoStream << i->second;
      infoStream << '(' << qualifiedCodomain << " = " << domain << '.' << unqualifiedCodomain << ')';
    }
    
    // Get the parent:symbol id
    //SemanticId id = GetRelationId(parent, symbol);
    
    // Check whether the qualified codomain has relations to other codomains (I.e. whether qualifiedCodomain is a domain in another context)
    //IdMultiIndexIterator i = domainIndexQCodomains.lower_bound(qualifiedCodomain);
    const IdMultiIndexRange  r = domainIndexQCodomains.equal_range(qualifiedCodomain);
    //if(i == domainIndexQCodomains.end() || i->first != qualifiedCodomain)
    if(r.first == r.second)
    {
      infoStream << endl;
      return;
    }
    
    // Recursively output the child relations
    infoStream << " -> ";
    
    // Determine whether there is more than one relation from this symbol
    IdMultiIndexIterator iNext = r.first; ++iNext;
    bool hasMultipleRelations = (r.second != iNext);

    if(hasMultipleRelations)
    {
      infoStream << endl;
      for(uint c = 0; c < indent; ++c) infoStream << "  ";
      infoStream << '[' << endl;
    }

    for(IdMultiIndexIterator i = r.first; i != r.second; ++i)
      DebugOutputSymbolEnvironment(qualifiedCodomain, i->second, hasMultipleRelations? indent+1 : 0);

    if(hasMultipleRelations)
    {
      for(uint c = 0; c < indent; ++c) infoStream << "  ";
      infoStream << ']' << endl;
    }
  }
#endif
}

#endif
#endif
