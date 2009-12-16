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
    
    SemanticId DeclareSymbol(const char* name);    
    SemanticId DeclareRelation(const Relation& relation);    
    SemanticId DeclareOpenDomain(const char* name);
    void CloseDomain(const char* name);
    void CloseDomain();

  protected:
    
    // (string, SemanticID) mappings
    typedef std::pair<std::string, SemanticId> StringIdPair;
    class StringIdAllocator : public std::allocator<StringIdPair>
    {
       pointer allocate (size_type num, const void* = 0) 
       {
           // print message and allocate memory with global new
           pointer ret = (pointer)(::operator new(num*sizeof(StringIdPair)));
           ret->second = SemanticId(-1);
           return ret;
       }
    };
    typedef std::unordered_map<std::string, SemanticId, std::hash<std::string>, std::equal_to<std::string>, StringIdAllocator> StringIdMap;
    
    
    // The map of all tokens and their ids (in any context)
    StringIdMap tokens;
    SemanticId nextTokenId;  // The next id to be assigned to a token in DeclareGlobal
    
    // Global domain of all literals (whose hash function are not equal to their own values)
    // (Note: this is equivalent to a perfect hash map since hash values that may contain conflicts are mapped
    // to permanent tokens that do not contain conflicts)
    std::vector<SemanticId> epsilonStrings;
        
    std::vector<SemanticId> epsilonDomain;        // Global domain of all symbols
    std::vector<OSIX::SemanticDB::Relation> relations;  // Database of relations
    std::stack<SemanticId> environment;           // Environment of open domains
    SemanticId activeDomainId;                    // The last opened domain

    // Constants
    static const SemanticId epsilonId;            // The global domain "epsilon"
    
    // Declare an atomic symbol in the global domain "epsilon"
    SemanticId DeclareGlobal(const char* name);
    
    // A helper for declaring a relation from an associative pair of ids
    SemanticId DeclareRelation(SemanticId domain, SemanticId codomain);
  };
}

/*                                   INCLUDES                               */
#include "semanticdb.inl"

#endif
