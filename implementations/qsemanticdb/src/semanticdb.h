#ifndef __QSEMANTICDB_SEMANTICDB_H__
#define __QSEMANTICDB_SEMANTICDB_H__
//////////////////////////////////////////////////////////////////////////////
//
//    SEMANTICDB.H
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      QSemanticDB base implementation.
*/

/*                              COMPILER MACROS                             */
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4311)
# pragma warning(disable:4312)
#endif

namespace QSemanticDB
{
/*                                  CLASSES                                 */
  class SemanticDBImplementation : public Base::Object
  {
  public:
    // Constructor
    INLINE SemanticDBImplementation();

    //Destructor
    virtual ~SemanticDBImplementation();
    
    OSIX::SemanticId DeclareSymbol(const char* name);    
    OSIX::SemanticId DeclareRelation(OSIX::SemanticDB::Relation& relation);    
    OSIX::SemanticId DeclareOpenDomain(const char* name);
    void CloseDomain(const char* name);
    void CloseDomain();

  protected:
    // Global domain of all literals (whose hash function are not equal to their own values)
    // (Note: this is equivalent to a perfect hash map since hash values that may contain conflicts are mapped
    // to permanent tokens that do not contain conflicts)
    std::vector<OSIX::SemanticId> epsilonStrings;
        
    std::vector<OSIX::SemanticId> epsilonDomain;        // Global domain of all symbols
    std::vector<OSIX::SemanticDB::Relation> relations;  // Database of relations
    std::stack<OSIX::SemanticId> environment;           // Environment of open domains
    OSIX::SemanticId activeDomainId;                    // The last opened domain

    // Constants
    static const OSIX::SemanticId epsilonId;            // The global domain "epsilon"
    
    // Declare an atomic symbol in the global domain "epsilon"
    void DeclareGlobal(const char* name);
    
    // A helper for declaring a relation from an associative pair of ids
    OSIX::SemanticId DeclareRelation(SemanticId domain, OSIX::SemanticId codomain);
  };
}

/*                                   INCLUDES                               */
#include "semanticdb.inl"

#endif
