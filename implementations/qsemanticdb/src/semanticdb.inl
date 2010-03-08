
#include "semanticdb.h"

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
    evalQueryId(OSIX::SEMANTICID_INVALID),
    activeDomainId(OSIX::SEMANTICID_EPSILON),
    errorStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out)),
    warnStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out)),
    infoStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out))
  {
      domainEnvironment.push(OSIX::SEMANTICID_EPSILON);
      environmentState.push(BUILD_DECLARATION);
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
    infoStream << "DeclareSymbol (" << name << ")";
#endif
    SemanticId qualifiedId = OSIX::SEMANTICID_INVALID;
    if(environmentState.top() == BUILD_DECLARATION)
    {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
      infoStream << " [Declaration]" << std::endl;
#endif
      SemanticId symbol = GlobalSymbol(name);

      // Create the declaration if it does not yet exist
      // (Or replace a query or speculative symbol that already exists)
      qualifiedId = GetQualifiedCodomain(OrderedRelation(activeDomainId, symbol));
      if(qualifiedId == OSIX::SEMANTICID_INVALID)
      {
        qualifiedId = DeclareRelation(activeDomainId, symbol);
      }
      else
      {
        // Overwrite the properties of the relation if any exists (note that declarations override queries and speculative symbols)
        IdPropertiesMap::iterator i = symbolProperties.find(qualifiedId);
        if(i != symbolProperties.end())
        {
          SymbolProperties& properties = i->second;
          properties.concrete = true;
          properties.query = QueryNone;
        }
      }
    }
    else if(environmentState.top() == BUILD_QUERY_SOURCE)
    {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
      infoStream << " [Query Source]" << std::endl;
#endif
      qualifiedId = DeclareSpeculativeRelation(activeDomainId, GlobalSymbol(name));
      queryEnvironment.push(qualifiedId);
    }
    else if((environmentState.top() & BUILD_QUERY_ARGUMENT) == BUILD_QUERY_ARGUMENT)
    {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
      infoStream << " [Query Argument]" << std::endl;
#endif
      SemanticId symbol = GlobalSymbol(name);
      qualifiedId = GetQualifiedCodomain(OrderedRelation(queryEnvironment.top(), symbol));
#ifdef QSEMANTICDB_DEBUG_VERBOSE
      infoStream << "Temporary: TOP QUERY = " << queryEnvironment.top() << " SYMBOL = " << symbol << " QUALIFIED ARGUMENT = " << qualifiedId << std::endl;
#endif
      if(qualifiedId == OSIX::SEMANTICID_INVALID)
      {
        qualifiedId = DeclareRelation(queryEnvironment.top(), symbol);
        CreateQuery(static_cast<QueryType>(environmentState.top() & ~BUILD_QUERY_ARGUMENT), qualifiedId);
      }
    }

    OSI_ASSERT(qualifiedId != OSIX::SEMANTICID_INVALID);
    return qualifiedId;
  }

  SemanticId SemanticDBImplementation::AnonymousSymbol()
  {
    // Create the symbol
    SemanticId anonymousId = nextSymbolId;
    ++nextSymbolId;
    //anonDomain.push_back(anonymousId);
    anonDomain.insert(anonymousId);

#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "Anonymous (" << anonymousId << ")";
#endif

    SemanticId qualifiedAnonymousId = anonymousId;

    // If we are constructing a Set-Query, then create a speculative relation
    // (See also DeclareSymbol)
    if(environmentState.top() == BUILD_DECLARATION)
    {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
      infoStream << " [Declaration]" << std::endl;
#endif
      /*
      SemanticId symbol = GlobalSymbol(name);

      // Create the declaration if it does not yet exist
      // (Or replace a query or speculative symbol that already exists)
      qualifiedId = GetQualifiedCodomain(OrderedRelation(activeDomainId, symbol));
      if(qualifiedId == OSIX::SEMANTICID_INVALID)
      {
        qualifiedId = DeclareRelation(activeDomainId, symbol);
      }
      else
      {
        // Overwrite the properties of the relation if any exists (note that declarations override queries and speculative symbols)
        IdPropertiesMap::iterator i = symbolProperties.find(qualifiedId);
        if(i != symbolProperties.end())
        {
          SymbolProperties& properties = i->second;
          properties.concrete = true;
          properties.query = QueryNone;
        }
      }*/
    }
    else if (environmentState.top() == BUILD_QUERY_SOURCE)
    {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
      infoStream << " [Query Source]" << std::endl;
#endif
      /*
      if(activeDomainId != OSIX::SEMANTICID_EPSILON)
      {
        // An anonymous id is always fully qualified
        // (See also DeclareRelation)
        domainIndexUCodomains.insert(std::make_pair(activeDomainId, anonymousId));
        domainIndexQCodomains.insert(std::make_pair(activeDomainId, anonymousId));
        relations.insert(RelationIndexValue(anonymousId, OrderedRelation(activeDomainId, anonymousId)));
      }*/

      qualifiedAnonymousId = DeclareSpeculativeAnonymousRelation(activeDomainId, anonymousId);
      OSI_ASSERT(qualifiedAnonymousId == anonymousId);
      queryEnvironment.push(qualifiedAnonymousId);
    }
    else
    {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
      infoStream << " [UNKNOWN ENVIRONMENT STATE!]" << std::endl;
#endif
    }
    return qualifiedAnonymousId;
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

    // Set the environment state
    environmentState.push(BUILD_DECLARATION);
    domainEnvironment.push(domainId);
    activeDomainId = domainId;

    return domainId;
  }

  void SemanticDBImplementation::OpenDomain(SemanticId domain)
  {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "OpenDomain (" << domain << ")" << std::endl;
#endif

    // Set the environment state
    environmentState.push(BUILD_DECLARATION);
    domainEnvironment.push(domain);
    activeDomainId = domain;
  }

  void SemanticDBImplementation::OpenHiddenDomain(SemanticId domain)
  {
    //  In a set-query, the query domain will have this structure (with the anonymous domain being opened by the user)
    //  query_result -> anonymous -> { HIDDEN -> query_set }
    //                            speculative-> query_argument

#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "OpenHiddenDomain (" << domain << ")" << std::endl;
#endif

    /*// Open a relation for the anonymous symbol
    //anonSymbol = AnonymousSymbol;
    OpenDomain(domain);*/

    // Declare a relation for the hidden symbol
    //SemanticId hiddenDomain = DeclareRelation(activeDomainId, INTERNALID_HIDDEN);
    SemanticId hiddenDomain = DeclareRelation(domain, INTERNALID_HIDDEN);

    // Set the environment state
    environmentState.push(BUILD_DECLARATION);
    domainEnvironment.push(hiddenDomain);
    activeDomainId = hiddenDomain;
  }

  void SemanticDBImplementation::CloseDomain(SemanticId domain)
  {
    // todo: test if id matches the opened domain
    CloseDomain();
  }

  void SemanticDBImplementation::CloseDomain()
  {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "CloseDomain" << std::endl;
#endif

    // Reset the environment state
    OSI_ASSERT(environmentState.top() == BUILD_DECLARATION);
    environmentState.pop();
    domainEnvironment.pop();
    activeDomainId = domainEnvironment.top();
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
    if(i == queries.right.end())
    {
      queryId = DeclareAnonymousSymbol();
      queries.insert(QueryIndexValue(queryId, query));
    }

    return queryId;
  }*/

  void SemanticDBImplementation::BeginQuery()
  {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "BeginQuery" << std::endl;
#endif

    // Change the environment state into a "query source" state
    environmentState.push(BUILD_QUERY_SOURCE);

    /* TODO: This should be moved to DeclareSymbol/DeclareRelation...


    // This is similar to DeclareOpenDomain except that the id is speculative
    //SemanticId speculativeDomainId = CreateSpeculativeId(domain);
    SemanticId speculativeDomainId = DeclareSpeculativeRelation(activeDomainId, domain); // NOTE: (BUSY) This could return either a concrete id or a speculative id...
    domainEnvironment.push(speculativeDomainId);
    activeDomainId = speculativeDomainId;
    //return domainId;

    // Is this still necessary?
    queryDomains.push(domain);*/



    /* OLD:
    activeDomainId = OSIX::SEMANTICID_EPSILON;
    domainEnvironment.push(activeDomainId);//*/
  }

  SemanticId SemanticDBImplementation::EndQuery()
  {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "EndQuery" << std::endl;
#endif

    // Remove the query state from the environment
    OSI_ASSERT((environmentState.top() & BUILD_QUERY_ARGUMENT) == BUILD_QUERY_ARGUMENT);
    environmentState.pop();

    /* TODO: Move this ....

    // This is similar to CloseDomain
    domainEnvironment.pop();
    activeDomainId = domainEnvironment.top();

    // Is this still necessary?
    queryDomains.pop();
    */

    // TEMP:
    queryEnvironment.pop();
    return 0;



    /* OLD:
    activeDomainId = domainEnvironment.top();
    domainEnvironment.pop();//*/
  }

  void SemanticDBImplementation::SelectionDisjunct()
  {
    OpenQueryArgument(QuerySelectionDisjunct);
  }

  void SemanticDBImplementation::SelectionExclusiveDisjunct()
  {
    OpenQueryArgument(QueryMutationExclusiveDisjunct);
  }

  void SemanticDBImplementation::SelectionConjunct()
  {
    OpenQueryArgument(QuerySelectionConjunct);
  }

  void SemanticDBImplementation::SelectionStrictConjunct()
  {
    OpenQueryArgument(QuerySelectionStrictConjunct);
  }

  void SemanticDBImplementation::MutationDisjunct()
  {
    OpenQueryArgument(QueryMutationDisjunct);
  }

  void SemanticDBImplementation::MutationExclusiveDisjunct()
  {
    OpenQueryArgument(QueryMutationExclusiveDisjunct);
  }

  void SemanticDBImplementation::MutationConjunct()
  {
    OpenQueryArgument(QueryMutationConjunct);
  }

  void SemanticDBImplementation::MutationStrictConjunct()
  {
    OpenQueryArgument(QueryMutationStrictConjunct);
  }

  void SemanticDBImplementation::BeginEvaluation(SemanticId root)
  {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "Begin evaluation ";
    IdStringMap::iterator i = epsilonStrings.find(root);
    if(i == epsilonStrings.end())
      infoStream << "(Unknown Id)";
    else
      infoStream << '(' << i->second << ')';
    infoStream << std::endl;
#endif

    //evaluationQueries.push(root);
    //activeQueries.push(evalQueryId);
    //evalQueryId = OSIX::SEMANTICID_INVALID;
    evalQueryId = root;
  }

  SemanticId SemanticDBImplementation::Eval()
  {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "  Eval: ";
#endif

    SemanticId result = EvalInternal();

#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << std::endl;
#endif

    return result;
  }

  void SemanticDBImplementation::EndEvaluation()
  {
    /*if(activeQueries.empty())
    {
      evalQueryId = OSIX::SEMANTICID_INVALID;
    }
    else
    {
      evalQueryId = activeQueries.top();
      activeQueries.pop();
    }*/

    if(!evaluationQueries.empty())
      evaluationQueries.pop();
  }


  SemanticId SemanticDBImplementation::GetActiveQuery()
  {
    return evalQueryId;
  }

  SemanticId SemanticDBImplementation::GetDomain(SemanticId qualifiedId)
  {
    RelationIndex::left_iterator i = relations.left.find(qualifiedId);
    //* NEW: all id's belong in the global epsilon domain
    if(i == relations.left.end())
      return OSIX::SEMANTICID_EPSILON;//*/
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

  SemanticId SemanticDBImplementation::GetQualifiedCodomain(const OrderedRelation& unqualifiedRelation) const
  {
    RelationIndex::right_const_iterator i = relations.right.find(unqualifiedRelation);
    return i == relations.right.end() ? OSIX::SEMANTICID_INVALID : i->second;
  }

  bool SemanticDBImplementation::HasStaticRelations(SemanticId domain) const
  {
    return domainIndexQCodomains.find(domain) != domainIndexQCodomains.end();
  }

  /*bool SemanticDBImplementation::DoesRelationExist(const OrderedRelation& unqualifiedRelation) const
  {
    return relations.right.find(unqualifiedRelation) != relations.right.end();
  }*/

  SemanticId SemanticDBImplementation::CreateQualifiedId(const Relation& relation)
  {
    SemanticId id = nextSymbolId;
    OSI_ASSERT(id < firstReservedId);
    ++nextSymbolId;
    return id;
  }

  const SemanticDBImplementation::SymbolProperties& SemanticDBImplementation::GetProperties(SemanticId domain) const
  {
    static const SymbolProperties defaultProperties;
    IdPropertiesMap::const_iterator i = symbolProperties.find(domain);
    if(i == symbolProperties.end())
      return defaultProperties;
    return i->second;
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

  SemanticId SemanticDBImplementation::EvalInternal()
  {
    SemanticId currentEvalId = evalQueryId;

    if(evalQueryId != OSIX::SEMANTICID_INVALID)
    {
      // Ignore hidden domains
      if (GetUnqualifiedCodomain(evalQueryId) == INTERNALID_HIDDEN)
      {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
        infoStream << "IGNORE HIDDEN ID (" << evalQueryId << ")";
#endif
        EvalContinue();
        return EvalInternal();
      }

      // Add parent codomain edges of this id to the evaluation stack
      EvalScheduleCodomains();

      // Check whether the item represents an atom, a query, a speculative domain or a hidden id
      IdPropertiesIterator i = symbolProperties.find(evalQueryId);

      if(i == symbolProperties.end() || (i->second.query == QueryNone && i->second.concrete))
      {
        EvalSymbol(evalQueryId);
      }
      else if(!i->second.concrete)
      {
        EvalContinue();
        EvalInternal();
      }
      else if(i->second.query == QuerySelectionConjunct || i->second.query == QuerySelectionStrictConjunct)
      {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
        infoStream << "SELECT CONJUNCT";
        if(i->second.query == QuerySelectionStrictConjunct)
          infoStream << " (STRICT)";
#endif

        // Get the domain and the unqualified codomain of the query (i.e. as an unqualified relation)
        evalQueryId = ResolveConjunctSelection(evalQueryId);
        if (evalQueryId == OSIX::SEMANTICID_INVALID && i->second.query == QuerySelectionStrictConjunct)
        {
          //errorStream << "TODO: Assertion failed...";
          infoStream << " TODO: Assertion failed...";
        }
      }
      else
      {
        // TODO: Unhandled type of query....
        evalQueryId = OSIX::SEMANTICID_INVALID;
      }
    }

    // Set up for the next evaluation
    if(evalQueryId == OSIX::SEMANTICID_INVALID)
      EvalContinue();

    return evalQueryId;
  }

  void SemanticDBImplementation::EvalContinue()
  {
    if (evaluationQueries.empty())
    {
      evalQueryId = OSIX::SEMANTICID_INVALID;
      return;
    }

    //* If there are any queries left on the evaluation stack, then evaluate them
    evalQueryId = evaluationQueries.top();
    evaluationQueries.pop();
  }

  void SemanticDBImplementation::EvalScheduleCodomains()
  {
    const IdMultiIndexRange r = domainIndexQCodomains.equal_range(evalQueryId);
    for(IdMultiIndexIterator i = r.first; i != r.second; ++i)
      evaluationQueries.push(i->second);
  }

  SemanticId SemanticDBImplementation::EvalSymbol(SemanticId symbol)
  {
    //* Determine whether this id has child codomains and evaluate them
    SemanticId unqualifiedCodomain = GetUnqualifiedCodomain(symbol);
    if(unqualifiedCodomain == OSIX::SEMANTICID_INVALID)
    {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
      IdStringMap::iterator i = epsilonStrings.find(symbol);
      if(i == epsilonStrings.end())
        infoStream << "(Unknown Atom Id)";
      else
        infoStream << "(Atom: " << i->second << ")";
#endif
    }
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    else
    {
      infoStream << "(Qualified Id: " << symbol << ")";
    }
#endif
    evalQueryId = unqualifiedCodomain;//*/
  }

  SemanticId SemanticDBImplementation::ResolveContext(SemanticId domain)
  {
    // Resolve the speculative domain on the left-hand side
    while(domain != OSIX::SEMANTICID_INVALID)
    {
      // Test whether this domain is concrete
      if(GetProperties(domain).concrete)
        break;

      // Get the  domain of this symbol
      domain = GetDomain(domain);
    }
    return domain;
  }

  SemanticId SemanticDBImplementation::ResolveRelation(OrderedRelation& relation)
  {
    // Try to find a concrete relation
    RelationIndex::right_const_iterator i = relations.right.find(relation);
    if(i != relations.right.end())
      return i->second;

    // Get all speculative relations that involve the domain
    IdMultiIndexRange rSpecCodomains = domainIndexSpeculativeQCodomains.equal_range(relation.domain);
    for(IdMultiIndexIterator iSpecCodomain = rSpecCodomains.first; iSpecCodomain != rSpecCodomains.second; ++iSpecCodomain)
    {
      // If the query has the correct unqualified codomain then perform the query to test whether the codomain will be returned
      // TODO: At the moment we simply assume that the query is a selection
      //       We'll need to put a test in here somewhere to determine this later
      SemanticId speculativeCodomain = iSpecCodomain->second;
      RelationIndex::right_const_iterator iQueryCodomain = relations.right.find(OrderedRelation(speculativeCodomain, relation.codomain));
      if(iQueryCodomain != relations.right.end())
      {
        // Do the selection query
        SemanticId currentEvalId = evalQueryId;
        evalQueryId = iQueryCodomain->second;
        SemanticId result = EvalInternal();
        if(result != OSIX::SEMANTICID_INVALID)
          return result;
      }
    }

    // Try to resolve the relation in a parent context
    if(relation.domain != OSIX::SEMANTICID_EPSILON && relation.domain != OSIX::SEMANTICID_INVALID)
    {
      relation.domain = GetDomain(relation.domain);
      return ResolveRelation(relation);
    }

    // All symbols exist in the global domain epsilon, so just return the unqualified codomain itself if no qualified codomain could be found
    return relation.codomain;
  }

  SemanticId SemanticDBImplementation::ResolveConjunctSelection(SemanticId query)
  {
    // Split the query into a speculative domain and an unqualified query argument
    RelationIndex::left_const_iterator iQueryRelation = relations.left.find(query);
    if(iQueryRelation == relations.left.end())
    {
      errorStream << "FATAL ERROR: Could not find the relation for a query in the database. This should never happen and should be reported as bug!" << std::endl;
      return OSIX::SEMANTICID_INVALID;
    }
    OrderedRelation queryRelation = iQueryRelation->second;

#if defined(QSEMANTICDB_DEBUG_VERBOSE) && defined(QSEMANTICDB_DEBUG_EVALOUTPUT)
    infoStream << std::endl << "(TEMPORARY NOTE: Query relation = " << queryRelation.domain << "->" << queryRelation.codomain << ")" << std::endl;
#endif

    // If the domain has a hidden domain, then perform the query on the hidden domain instead
    //if (anonDomain.find(queryRelation.domain) != anonDomain.end() && )
    SemanticId hiddenCodomain = GetQualifiedCodomain(OrderedRelation(queryRelation.domain, INTERNALID_HIDDEN));
    if (hiddenCodomain != OSIX::SEMANTICID_INVALID)
    {
      queryRelation.domain = hiddenCodomain;

#if defined(QSEMANTICDB_DEBUG_VERBOSE) && defined(QSEMANTICDB_DEBUG_EVALOUTPUT)
      infoStream << "(TEMPORARY NOTE: Resolving relation in the hidden domain (" << hiddenCodomain << "))" << std::endl;
      infoStream << "(TEMPORARY NOTE: QUERY = " << queryRelation.domain << "->" << queryRelation.codomain << ")" << std::endl;
#endif

      // Try to find a relation
      SemanticId result = OSIX::SEMANTICID_INVALID;
      //result = ResolveRelation(queryRelation);
      RelationIndex::right_const_iterator i = relations.right.find(queryRelation);
      if(i != relations.right.end())
        result = i->second;

#if defined(QSEMANTICDB_DEBUG_VERBOSE) && defined(QSEMANTICDB_DEBUG_EVALOUTPUT)
      infoStream << "(TEMPORARY NOTE: QUERY RESULT = " << result << ")" << std::endl;
#endif

      return result;
    }

    // Split the speculative domain into a domain and an unqualified codomain
    RelationIndex::left_const_iterator iDomainRelation = relations.left.find(queryRelation.domain);
    if(iDomainRelation == relations.left.end())
    {
      errorStream << "ERROR (TODO): Could not find a query domain in the database. Not yet sure what to do in this case..." << std::endl;
      return OSIX::SEMANTICID_INVALID;
    }
    OrderedRelation domainRelation = iDomainRelation->second;

#if defined(QSEMANTICDB_DEBUG_VERBOSE) && defined(QSEMANTICDB_DEBUG_EVALOUTPUT)
    infoStream << "(TEMPORARY NOTE: Speculative domain relation = " << domainRelation.domain << "->" << domainRelation.codomain << ")" << std::endl;
#endif

    // Resolve the speculative domain of the query to get a concrete domain
    domainRelation.domain = ResolveContext(domainRelation.domain);

#if defined(QSEMANTICDB_DEBUG_VERBOSE) && defined(QSEMANTICDB_DEBUG_EVALOUTPUT)
    infoStream << "(TEMPORARY NOTE: Concrete domain relation = " << domainRelation.domain << "->" << domainRelation.codomain << ")" << std::endl;
#endif

    // Resolve the selection itself
    SemanticId result = OSIX::SEMANTICID_INVALID;
    while(domainRelation.domain != OSIX::SEMANTICID_EPSILON)
    {
      // Find a suitable domain for the query
      domainRelation.domain = GetDomain(domainRelation.domain);
      queryRelation.domain = ResolveRelation(domainRelation);

#if defined(QSEMANTICDB_DEBUG_VERBOSE) && defined(QSEMANTICDB_DEBUG_EVALOUTPUT)
      infoStream << "(TEMPORARY NOTE: QUERY DOMAIN = " << domainRelation.domain << "->" << domainRelation.codomain << ")" << std::endl;
      infoStream << "(TEMPORARY NOTE: QUERY = " << queryRelation.domain << "->" << queryRelation.codomain << ")" << std::endl;
#endif

      result = ResolveRelation(queryRelation);

#if defined(QSEMANTICDB_DEBUG_VERBOSE) && defined(QSEMANTICDB_DEBUG_EVALOUTPUT)
      infoStream << "(TEMPORARY NOTE: QUERY = " << queryRelation.domain << "->" << queryRelation.codomain << ")" << std::endl;
      infoStream << "(TEMPORARY NOTE: QUERY RESULT = " << result << ")" << std::endl;
#endif

      // If the query's domain could not be resolved, then the query has failed.
      if(queryRelation.domain != OSIX::SEMANTICID_EPSILON)
        return result;
    }

    return OSIX::SEMANTICID_INVALID;
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
      IdStringMap::iterator i = epsilonStrings.find(qualifiedCodomain);
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
      IdStringMap::iterator i = epsilonStrings.find(unqualifiedCodomain);
      if(i == epsilonStrings.end())
        infoStream << "Anonymous";
      else
        infoStream << i->second;

      // Test  whether the qualified codomain is concrete (if not, then this is an (unresolved) query)
      IdPropertiesIterator iProperties = symbolProperties.find(qualifiedCodomain);
      bool concrete = iProperties == symbolProperties.end() || iProperties->second.concrete;
      const char* relationOperator = "->";
      switch(iProperties->second.query)
      {
      case QuerySelectionDisjunct:                    relationOperator = "~"; break;
      case QuerySelectionExclusiveDisjunct:           relationOperator = "!"; break;
      case QuerySelectionConjunct:                    relationOperator = "."; break;
      case QuerySelectionStrictConjunct:              relationOperator = ":"; break;
      case QueryMutationDisjunct:                     relationOperator = "~~"; break;
      case QueryMutationExclusiveDisjunct:            relationOperator = "!!"; break;
      case QueryMutationConjunct:                     relationOperator = ".."; break;
      case QueryMutationStrictConjunct:               relationOperator = "::"; break;
      case QueryNone:                                 break;
      }


      infoStream << (concrete? "(" : "(? ") << qualifiedCodomain << " = " << domain << relationOperator << unqualifiedCodomain << (concrete? ")" : " ?)");
    }

    // Check whether the qualified codomain has relations to other codomains (I.e. whether qualifiedCodomain is a domain in another context)
    const IdMultiIndexRange r = domainIndexQCodomains.equal_range(qualifiedCodomain);
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
