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
    OSI_INLINE_METHOD SemanticDB() {}
    OSI_INLINE_METHOD SemanticDB(const SemanticDB&) {}
    
  public:
    /* symbols */
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL DeclareSymbol(const OSchar* name);
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL GlobalSymbol(const OSchar* name);
    
    /* relations */
    struct Relation
    {
      SemanticId domain;
      SemanticId codomain;
      OSI_INLINE_METHOD Relation(SemanticId domain, SemanticId codomain) : domain(domain), codomain(codomain) {}
    };
    
    /** declarations */
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL DeclareRelation(const Relation& relation);
    OSI_INLINE_METHOD SemanticId OSI_API_CALL DeclareRelation(SemanticId domain, SemanticId codomain) { return DeclareRelation(Relation(domain, codomain)); }
    
    /** queries **/
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL SelectRelation(const Relation& relation);
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL SelectRelation(SemanticId domain, SemanticId codomain) { return SelectRelation(Relation(domain, codomain)); }
    
    /* domains */
    OSI_DYNAMIC_METHOD SemanticId OSI_API_CALL DeclareOpenDomain(const OSchar* name);
    OSI_DYNAMIC_METHOD void OSI_API_CALL CloseDomain(const OSchar* name);
    OSI_DYNAMIC_METHOD void OSI_API_CALL CloseDomain();
    
    /* debug */
    OSI_DYNAMIC_METHOD void* OSI_API_CALL DebugInit();
  };

  /* initialization */
  SemanticDB* OSI_API_CALL SemanticDBInit();
}

#endif
