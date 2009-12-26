
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
    nextTokenId(0),
    errorStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out)),
    warnStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out)),
    infoStream(new STDEXT_NAMESPACE::stdio_filebuf<char>(stdout, std::ios::out))
  {
      environment.push(epsilonId);
      activeDomainId = epsilonId;
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
  
  SemanticId SemanticDBImplementation::DeclareRelation(const Relation& unqualifiedRelation)
  {    
    SemanticId qualifiedCodomain = unqualifiedRelation.codomain;
    if (unqualifiedRelation.domain != epsilonId)
    {
      qualifiedCodomain = CreateQualifiedId(unqualifiedRelation);
      domainIndexUCodomains.insert(std::make_pair(unqualifiedRelation.domain, unqualifiedRelation.codomain));
      domainIndexQCodomains.insert(std::make_pair(unqualifiedRelation.domain, qualifiedCodomain));
      relations.insert(RelationIndex::relation(qualifiedCodomain, unqualifiedRelation));
    }

#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "DeclareRelation (" 
            << unqualifiedRelation.domain <<  "->" << unqualifiedRelation.codomain
            << " = " << qualifiedCodomain << ')' << std::endl;
#endif    
    return qualifiedCodomain;
  }
  
  //SemanticId SemanticDBImplementation::SelectRelation(const Relation& relation)
  void SemanticDBImplementation::SelectRelation(const Relation& unqualifiedRelation, std::vector<SemanticId>& qualifiedCodomains)
  {
    //size_t initialSize = qualifiedCodomains.size();

#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "Select " << unqualifiedRelation.domain << '.' << unqualifiedRelation.codomain << std::endl;
#endif
    IdMultiIndex::iterator i = domainIndexUCodomains.lower_bound(unqualifiedRelation.domain);
    while (i != domainIndexUCodomains.end() && i->first == unqualifiedRelation.domain)
    {
      // Check whether this qualified codomain matches the relation.
      // (May be either an exact or an inexact match. I.e. the codmain may refer to additional codomains as long as it is qualified by the same domain)
      if (unqualifiedRelation.codomain == i->second)
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
    //if (qualifiedCodomains.size() == initialSize)
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
    if (name == null)
        return invalidId;
    
    const std::string nameString(name);
    StringIdMap::iterator i = tokens.find(name);
    if (i != tokens.end())
      return i->second;
    
    SemanticId &id = tokens[nameString];
    id = nextTokenId;
    epsilonDomain.push_back(id);
    epsilonStrings[id] = nameString;
    ++nextTokenId;
    
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
    if (i == relations.end())
      return invalidId;
    return i->second;
  }*/
  
  /*SemanticId SemanticDBImplementation::GetTailId(SemanticId relationId)
  {
    RelationTailIdMap::iterator i = unqualifiedCodomains.find(relationId);
    if (i == unqualifiedCodomains.end())
      return invalidId;
    return i->second;
  }*/

  SemanticId SemanticDBImplementation::GetUnqualifiedId(SemanticId qualifiedId)
  {
    RelationIndex::left_iterator i = relations.left.find(qualifiedId);
    if (i == relations.left.end())
      return invalidId;
    return i->second.codomain;
  }
  
  bool SemanticDBImplementation::HasStaticRelations(SemanticId domain)
  {
    return domainIndexQCodomains.find(domain) != domainIndexQCodomains.end();
  }
  
  SemanticId SemanticDBImplementation::CreateQualifiedId(const Relation& relation)
  {
    SemanticId id = nextTokenId;
    ++nextTokenId;
    return id;
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
    for (RelationIndex::iterator i = relations.begin(); i != relations.end(); ++i)
      infoStream << i->left << " = " << i->right.domain << '.' << i->right.codomain << endl;
    infoStream << endl;
    
    
    // Output complete environment
    infoStream << "Environment: (domain -> (qualifiedCodomain = domain.unqualifiedCodomain))" << endl
               << "------------" << endl;
    infoStream << epsilonName << '(' << epsilonId << ") -> " << endl
               << "[" << endl;
    for (std::vector<SemanticId>::iterator i = epsilonDomain.begin(); i != epsilonDomain.end(); ++i)
    {
      DebugOutputSymbolEnvironment(epsilonId, *i, 1);
    }
    infoStream << "]" << endl;
  }
  
  void SemanticDBImplementation::DebugOutputSymbolEnvironment(SemanticId domain, SemanticId qualifiedCodomain, uint indent)
  {    
    using std::endl;
    
    SemanticId unqualifiedCodomain = GetUnqualifiedId(qualifiedCodomain);
    
    // Write the symbol
    for (uint c = 0; c < indent*2; ++c) infoStream << ' ';
    if (domain == epsilonId)
    {
      //infoStream << epsilonName << ".";
      std::map<SemanticId, std::string>::iterator i = epsilonStrings.find(qualifiedCodomain);
      if (i == epsilonStrings.end())
        infoStream << "Anonymous";
      else
        infoStream << i->second;
      infoStream << '(' << qualifiedCodomain << ')' << " = " << qualifiedCodomain;
    }
    else if(unqualifiedCodomain == invalidId)
      infoStream << "Invalid (no tail for " << domain << " -> " << qualifiedCodomain << ")";
    else
    {
      std::map<SemanticId, std::string>::iterator i = epsilonStrings.find(unqualifiedCodomain);
      if (i == epsilonStrings.end())
        infoStream << "Anonymous";
      else
        infoStream << i->second;
      infoStream << '(' << qualifiedCodomain << " = " << domain << '.' << unqualifiedCodomain << ')';
    }
    
    // Get the parent:symbol id
    //SemanticId id = GetRelationId(parent, symbol);
    
    // Check whether the qualified codomain has relations to other codomains (I.e. whether qualifiedCodomain is a domain in another context)
    //IdMultiIndex::iterator i = domainIndexQCodomains.lower_bound(qualifiedCodomain);
    const std::pair<IdMultiIndex::iterator, IdMultiIndex::iterator>  r = domainIndexQCodomains.equal_range(qualifiedCodomain);
    //if (i == domainIndexQCodomains.end() || i->first != qualifiedCodomain)
    if(r.first == r.second)
    {
      infoStream << endl;
      return;
    }
    
    // Recursively output the child relations
    infoStream << " -> ";
    
    // Determine whether there is more than one relation from this symbol
    IdMultiIndex::iterator iNext = r.first; ++iNext;
    bool hasMultipleRelations = (r.second != iNext);

    if (hasMultipleRelations)
    {
      infoStream << endl;
      for (uint c = 0; c < indent; ++c) infoStream << "  ";
      infoStream << '[' << endl;
    }

    for (IdMultiIndex::iterator i = r.first; i != r.second; ++i)
      DebugOutputSymbolEnvironment(qualifiedCodomain, i->second, hasMultipleRelations? indent+1 : 0);

    if (hasMultipleRelations)
    {
      for (uint c = 0; c < indent; ++c) infoStream << "  ";
      infoStream << ']' << endl;
    }
  }
#endif
}

#endif
#endif
