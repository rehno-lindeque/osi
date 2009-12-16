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
  
#ifdef _DEBUG
  void SemanticDBImplementation::DebugOutputEnvironment()
  {
    infoStream << "Environment:" << std::endl
               << "------------" << std::endl;
  }
#endif
}

#endif
#endif
