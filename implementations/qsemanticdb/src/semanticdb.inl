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
    scheduler(schedule),
    //evalId(OSIX::SEMANTICID_INVALID),
    activeDomainId(OSIX::SEMANTICID_EPSILON),
    errorStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out)),
    warnStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out)),
    infoStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out))
#ifdef QSEMANTICDB_DEBUG_VISUALIZE
    ,schedulerDebugVisualizer(*this, scheduler)
#endif
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
    //activeQueries.push(evalId);
    //evalId = OSIX::SEMANTICID_INVALID;


    //evalId = root;
    scheduler.Reset();
    scheduler.Push(root);
    scheduler.Commit();
  }

  SemanticId SemanticDBImplementation::Eval()
  {
    QSEMANTICDB_DEBUG_VERBOSE_PRINT(infoStream << "  Eval: ")

    // Invariant Condition: The scheduler should always be finished with its previous iteration of work when the next iteration begins.
    OSI_ASSERT(scheduler.Done());

    // If the schedule is empty we are done
    if(schedule.Empty())
    {
      QSEMANTICDB_DEBUG_VISUALIZE_SCHEDULE("Eval_Finished")
      QSEMANTICDB_DEBUG_VERBOSE_PRINT("(Finished)" << std::endl)
      return OSIX::SEMANTICID_INVALID;
    }

    QSEMANTICDB_DEBUG_VISUALIZE_SCHEDULE("Eval")

    // Fetch the next symbol to return to the evaluator from the scheduler
    //QSEMANTICDB_DEBUG_VERBOSE_PRINT("FrontFront " << schedule.Begin()->frontIndex << ' ' << schedule.Begin()->queue << ' ' << schedule.Begin()->queue->size() << " " << schedule.RootBranches() << " " << schedule.Begin()->Size() << std::endl)
    SemanticId evalId = schedule.Front();

    // If the fetched symbol needs to be evaluated internally for the next iteration, then evaluate it
    if(schedule.RootBranches() == 1 && schedule.Begin()->Size() == 1)
    {
      // Reset the scheduler
      scheduler.Reset();

      // Invariant Condition: When the branch only has one symbol and no branches, then Front() (i.e. the next symbol to return to the evaluator) is the same as Back() (i.e. the next symbol to evaluate internally).
      //OSI_ASSERT(schedule.Front() == scheduler.Back());
      EvalInternal(evalId);
    }

    // Remove the symbol to be returned from the schedule
    //infoStream << schedule.Begin()->Size() << '-';
    schedule.PopFront();
    //infoStream << schedule.Begin()->Size();

    QSEMANTICDB_DEBUG_VISUALIZE_SCHEDULE("Eval_AfterPopFront")

    QSEMANTICDB_DEBUG_VERBOSE_PRINT('(' << evalId << ')' << std::endl)
    return evalId;
  }

  SemanticId SemanticDBImplementation::GetEvalSymbol()
  {
    return schedule.Front();
  }

  void SemanticDBImplementation::EndEvaluation()
  {
    /*if(activeQueries.empty())
    {
      evalId = OSIX::SEMANTICID_INVALID;
    }
    else
    {
      evalId = activeQueries.top();
      activeQueries.pop();
    }*/


    /* OLD:
    if(!evaluationQueries.empty())
      evaluationQueries.pop();*/
    /* NEW:
    scheduler.Pop();//*/
  }


  /*SemanticId SemanticDBImplementation::GetActiveQuery()
  {
    return evalId;
  }*/

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
}

#endif
#endif
