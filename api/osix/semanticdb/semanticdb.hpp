#ifndef __OSIX_SEMANTICDB_HPP__
#define __OSIX_SEMANTICDB_HPP__
//////////////////////////////////////////////////////////////////////////////
//
//    SEMANTICDB.HPP
//
//    Ver 0.1 (2009-08-21)
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Main OpenParser C++ interface.
*/
/*                                 INCLUDES                                 */
#include "../../osi/os.hpp"

/*                                 CONSTANTS                                */
#define SEMANTICDB_VERSION "0.1"

namespace OSIX
{
  /*                                   TYPES                                  */
  typedef OSuint32 SemanticId;

  /*                                     API                                  */
  class SemanticDB
  {
  protected:
    inline SemanticDB() {}
    inline SemanticDB(const SemanticDB&) {}
    
  public:
    struct Relation
    {
      SemanticId domain;
      SemanticId codomain;
      inline Relation(SemanticId domain, SemanticId codomain) : domain(domain), codomain(codomain) {}
    };
    
    SemanticId DeclareSymbol(const OSchar* name);    
    SemanticId DeclareRelation(const Relation& relation);    
    SemanticId DeclareOpenDomain(const OSchar* name);
    void CloseDomain(const OSchar* name);
    void CloseDomain();    
  };

  SemanticDB* OSI_API_CALL semanticDBInit();
}

#endif
