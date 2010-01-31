
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
    infoStream << "DeclareSymbol (" << name << ")" << std::endl;
#endif
    SemanticId qualifiedId = OSIX::SEMANTICID_INVALID;
    if (environmentState.top() == BUILD_DECLARATION)
    {
      qualifiedId = DeclareRelation(activeDomainId, GlobalSymbol(name));
    }
    else if (environmentState.top() == BUILD_QUERY_SOURCE)
    {
      qualifiedId = DeclareSpeculativeRelation(activeDomainId, GlobalSymbol(name));
      queryEnvironment.push(qualifiedId);
    }
    else if ((environmentState.top() & BUILD_QUERY_ARGUMENT) == BUILD_QUERY_ARGUMENT)
    {
      qualifiedId = DeclareRelation(queryEnvironment.top(), GlobalSymbol(name));
      CreateQuery(static_cast<QueryType>(environmentState.top() & ~BUILD_QUERY_ARGUMENT), qualifiedId);
    }

    OSI_ASSERT(qualifiedId != OSIX::SEMANTICID_INVALID);
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

    // Set the environment state
    environmentState.push(BUILD_DECLARATION);
    domainEnvironment.push(domainId);
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
    if (i == queries.right.end())
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
    //return CreateQuery(QuerySelectionDisjunct, queryEnvironment.top(), selection);
  }

  void SemanticDBImplementation::SelectionExclusiveDisjunct()
  {
    OpenQueryArgument(QueryMutationExclusiveDisjunct);
    //return CreateQuery(QuerySelectionExclusiveDisjunct, queryEnvironment.top(), selection);
  }

  void SemanticDBImplementation::SelectionConjunct()
  {
    OpenQueryArgument(QuerySelectionConjunct);
    //return CreateQuery(QuerySelectionConjunct, queryEnvironment.top(), selection);
  }

  void SemanticDBImplementation::SelectionStrictConjunct()
  {
    OpenQueryArgument(QuerySelectionStrictConjunct);
    //return CreateQuery(QuerySelectionStrictConjunct, queryEnvironment.top(), selection);
  }

  void SemanticDBImplementation::MutationDisjunct()
  {
    OpenQueryArgument(QueryMutationDisjunct);
    //return CreateQuery(QueryMutationDisjunct, queryEnvironment.top(), mutation);
  }

  void SemanticDBImplementation::MutationExclusiveDisjunct()
  {
    OpenQueryArgument(QueryMutationExclusiveDisjunct);
    //return CreateQuery(QueryMutationExclusiveDisjunct, queryEnvironment.top(), mutation);
  }

  void SemanticDBImplementation::MutationConjunct()
  {
    OpenQueryArgument(QueryMutationConjunct);
    //return CreateQuery(QueryMutationConjunct, queryEnvironment.top(), mutation);
  }

  void SemanticDBImplementation::MutationStrictConjunct()
  {
    OpenQueryArgument(QueryMutationStrictConjunct);
    //return CreateQuery(QueryMutationStrictConjunct, queryEnvironment.top(), mutation);
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

    if (evalQueryId != OSIX::SEMANTICID_INVALID)
    {
      // Add parent codomain edges of this id to the evaluation stack
      const IdMultiIndexRange r = domainIndexQCodomains.equal_range(evalQueryId);
      for (IdMultiIndexIterator i = r.first; i != r.second; ++i)
        evaluationQueries.push(i->second);

      //* Determine whether this id has child codomains and evaluate them
      SemanticId unqualifiedCodomain = GetUnqualifiedCodomain(evalQueryId);
      if (unqualifiedCodomain == OSIX::SEMANTICID_INVALID)
      {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
        IdStringMap::iterator i = epsilonStrings.find(evalQueryId);
        if(i == epsilonStrings.end())
          infoStream << "(Unknown Atom Id)";
        else
          infoStream << i->second;
#endif
      }
#ifdef QSEMANTICDB_DEBUG_VERBOSE
      else
      {
        infoStream << "(Qualified Id)";
      }
#endif
      evalQueryId = unqualifiedCodomain;//*/
    }

    //* If there are any queries left on the evaluation stack, then evaluate them
    if (evalQueryId == OSIX::SEMANTICID_INVALID && !evaluationQueries.empty())
    {
      evalQueryId = evaluationQueries.top();
      evaluationQueries.pop();
    }

#ifdef QSEMANTICDB_DEBUG_VERBOSE
      infoStream << std::endl;
#endif

    return evalQueryId;
  }

  void SemanticDBImplementation::EndEvaluation()
  {
    /*if (activeQueries.empty())
    {
      evalQueryId = OSIX::SEMANTICID_INVALID;
    }
    else
    {
      evalQueryId = activeQueries.top();
      activeQueries.pop();
    }*/

    if (!evaluationQueries.empty())
      evaluationQueries.pop();
  }

  SemanticId SemanticDBImplementation::GetActiveQuery()
  {
    return evalQueryId;
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

  SemanticId SemanticDBImplementation::DeclareSpeculativeRelation(SemanticId domain, SemanticId codomain)
  {
    return DeclareSpeculativeRelation(Relation(domain, codomain));
  }

  SemanticId SemanticDBImplementation::DeclareSpeculativeRelation(const Relation& unqualifiedRelation)
  {
    // First test whether the relation already exists (in which case there is no need to create a "speculative" relation
    RelationIndex::right_const_iterator i = relations.right.find(unqualifiedRelation);
    if (i != relations.right.end())
    {
      return i->second;
    }

    // No concrete relation exists for the given pair, so create a speculative one
    SemanticId qualifiedCodomain = unqualifiedRelation.codomain;
    if(unqualifiedRelation.domain != OSIX::SEMANTICID_EPSILON)
    {
      // TODO: SHOULD unqualifiedRelation.domain == OSIX::SEMANTICID_EPSILON be possible.
      //        Probably not - an error should be given if not.

      qualifiedCodomain = CreateQualifiedId(unqualifiedRelation);
      domainIndexUCodomains.insert(std::make_pair(unqualifiedRelation.domain, unqualifiedRelation.codomain));
      domainIndexQCodomains.insert(std::make_pair(unqualifiedRelation.domain, qualifiedCodomain));
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

  /*SemanticId SemanticDBImplementation::CreateQuery(QueryType type, SemanticId domain, SemanticId argument)
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
  }*/

  void SemanticDBImplementation::CreateQuery(QueryType type, SemanticId qualifiedId)
  {
    // Set a flag indicating that the id represents a query
    symbolProperties.insert(std::make_pair(qualifiedId, SymbolProperties(true, type)));
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
      IdPropertiesMap::iterator iProperties = symbolProperties.find(qualifiedCodomain);
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
      }


      infoStream << (concrete? "(" : "(? ") << qualifiedCodomain << " = " << domain << relationOperator << unqualifiedCodomain << (concrete? ")" : " ?)");
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
