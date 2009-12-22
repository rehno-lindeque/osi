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
    SemanticId relationId = DeclareRelation(activeDomainId, DeclareGlobal(name));
    codomains.insert(std::make_pair(activeDomainId, relationId));
    return relationId;
  }
  
  SemanticId SemanticDBImplementation::DeclareRelation(const Relation& relation)
  {    
    SemanticId id = relation.codomain;
    if (relation.domain != epsilonId)
    {
      id = CreateRelationId(relation);
      //tailIds.insert(std::make_pair(relation.domain, id));
      tailIds.insert(std::make_pair(id, relation.codomain));
    }
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "DeclareRelation (" 
            << relation.domain <<  "->" << relation.codomain
            << " = " << id << ')' << std::endl;
#endif    
    return id;
  }
  
  SemanticId SemanticDBImplementation::DeclareOpenDomain(const char* name)
  {
#ifdef QSEMANTICDB_DEBUG_VERBOSE
    infoStream << "DeclareOpenDomain (" << name << ")" << std::endl;
#endif
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
  
  SemanticId SemanticDBImplementation::DeclareGlobal(const char* name)
  {
    OSI_ASSERT(name != null);
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
    infoStream << "DeclareGlobal (" << name << ',' << id << ")" << std::endl;
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
  
  SemanticId SemanticDBImplementation::GetTailId(SemanticId relationId)
  {
    RelationTailIdMap::iterator i = tailIds.find(relationId);
    if (i == tailIds.end())
      return invalidId;
    return i->second;
  }
  
  bool SemanticDBImplementation::HasStaticRelations(SemanticId domain)
  {
    return codomains.find(domain) != codomains.end();
  }
  
  SemanticId SemanticDBImplementation::CreateRelationId(const Relation& relation)
  {
    SemanticId id = nextTokenId;
    ++nextTokenId;
    return id;
  }
  
#ifdef _DEBUG
  void SemanticDBImplementation::DebugOutputEnvironment()
  {
    using std::endl;
    
    // Output tail ids
    infoStream << "Tail Ids: (relation:tail)" << endl
               << "---------" << endl;
    for (RelationTailIdMap::iterator i = tailIds.begin(); i != tailIds.end(); ++i)
      infoStream << i->first << ':' << i->second << endl;
    infoStream << endl;
    
    // Output codomains
    infoStream << "Codomains:" << endl
               << "----------" << endl;
    for (std::multimap<SemanticId, SemanticId>::iterator i = codomains.begin(); i != codomains.end(); ++i)
      infoStream << i->first << ':' << i->second << endl;
    infoStream << endl;
    
    // Output complete environment
    infoStream << "Environment: (domain -> relation)(codomain)" << endl
               << "------------" << endl;
    infoStream << epsilonName << '(' << epsilonId << ") -> " << endl
               << "[" << endl;
    for (std::vector<SemanticId>::iterator i = epsilonDomain.begin(); i != epsilonDomain.end(); ++i)
    {
      DebugOutputSymbolEnvironment(epsilonId, *i, 1);
    } 
    infoStream << "]" << endl;
  }
  
  void SemanticDBImplementation::DebugOutputSymbolEnvironment(SemanticId parent, SemanticId symbol, uint indent)
  {    
    using std::endl;
    
    SemanticId tailId = GetTailId(symbol);
    
    // Write the symbol
    for (uint c = 0; c < indent*2; ++c) infoStream << ' ';
    if (parent == epsilonId)
      infoStream << epsilonName << "->" << epsilonStrings[symbol] << '(' << symbol << ')' << " = " << symbol;
    else if(tailId == invalidId)
      infoStream << "Invalid (no tail for " << parent << " -> " << symbol << ")";
    else
      infoStream << epsilonStrings[tailId] << '(' << parent << "->" << tailId << " = " << symbol << ')';
    
    // Get the parent:symbol id
    //SemanticId id = GetRelationId(parent, symbol);
    
    // Check whether the parent:symbol domain has any relations
    std::multimap<SemanticId, SemanticId>::iterator i = codomains.lower_bound(symbol);
    if (i == codomains.end() || i->first != symbol)
    {
      infoStream << endl;
      return;
    }
    
    // Recursively output the child relations
    infoStream << " ->" << endl;

    for (uint c = 0; c < indent; ++c) infoStream << "  "; 
    infoStream << '[' << endl;
    
    while (i != codomains.end() && i->first == symbol)
    {
      DebugOutputSymbolEnvironment(i->first, i->second, indent+1);
      ++i;
    }

    for (uint c = 0; c < indent; ++c) infoStream << "  "; 
    infoStream << ']' << endl;
  }
#endif
}

#endif
#endif
