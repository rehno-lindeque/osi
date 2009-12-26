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
/*                                   TYPES                                  */
  using BaseSemanticDB::OrderedRelation;
/*                                 FUNCTIONS                                */
  /*inline bool operator == (const Relation& arg1, const Relation& arg2)
  {
    return arg1.domain == arg2.domain && arg1.codomain == arg2.codomain;
  }
  
  inline bool operator < (const Relation& arg1, const Relation& arg2)
  {
    return (arg1.domain < arg2.domain) || (arg1.domain == arg2.domain && arg1.codomain < arg2.codomain);
  }
  
  template<typename T> struct CompareLess {};
  template<> struct CompareLess<Relation> 
  { inline bool operator()(const Relation& arg1,const Relation& arg2) { return arg1 < arg2; } };*/
  
  
  /*template<typename T> class Hash {};  
  template<> class Hash<Relation>
  {
    inline size_t operator()(const Relation& arg) const { return std::hash< std::pair<SemanticId, SemanticId> >(std::make_pair(arg.domain, arg.codomain)); }
  };*/
  
/*                                  CLASSES                                 */
  class SemanticDBImplementation : public Base::Object
  {
#ifdef _DEBUG
    friend class ::OSIX::SemanticDBDbg;
#endif
  public:
    // Constructor
    INLINE SemanticDBImplementation();

    //Destructor
    virtual ~SemanticDBImplementation();
    
    SemanticId DeclareSymbol(const char* name);
    SemanticId GlobalSymbol(const char* name);
    SemanticId DeclareRelation(const Relation& unqualifiedRelation);
    //SemanticId SelectRelation(const Relation& relation);
    void SelectRelation(const Relation& unqualifiedRelation, std::vector<SemanticId>& qualifiedCodomains);
    SemanticId DeclareOpenDomain(const char* name);
    void CloseDomain(const char* name);
    void CloseDomain();
    
    // Initialization
    void Init();
    
    // Message streams
    INLINE void SetErrorStream(FILE* stream);
    INLINE void SetWarningStream(FILE* stream);
    INLINE void SetInfoStream(FILE* stream);
    
  protected:    
    // (string, SemanticID) mappings
    typedef std::pair<std::string, SemanticId> StringIdPair;
    /*class StringIdAllocator : public std::allocator<StringIdPair>
    {
       pointer allocate (size_type num, const void* = 0) 
       {
           // print message and allocate memory with global new
           pointer ret = (pointer)(::operator new(num*sizeof(StringIdPair)));
           ret->second = invalidId;
           return ret;
       }
    };
    typedef std::unordered_map<std::string, SemanticId, std::hash<std::string>, std::equal_to<std::string>, StringIdAllocator> StringIdMap;*/
    typedef std::unordered_map<std::string, SemanticId, std::hash<std::string>, std::equal_to<std::string> > StringIdMap;
    
    // (relation, SemanticId)  mappings
    //typedef std::unordered_map<Relation, SemanticId, Hash<Relation> > RelationIdMap;
    //typedef std::map<Relation, SemanticId, CompareLess<Relation> > RelationIdMap;
    
    // Tail ids
    typedef std::map<SemanticId, SemanticId> RelationTailIdMap; // Map from relation id to tail id
    
    // The map of all tokens and their ids (in any context)
    StringIdMap tokens;
    SemanticId nextTokenId;  // The next id to be assigned to a token in DeclareGlobal
    
    // Global domain of all literals (whose hash function are not equal to their own values)
    // (Note: this is equivalent to a perfect hash map since hash values that may contain conflicts are mapped
    // to permanent tokens that do not contain conflicts)
    std::map<SemanticId, std::string> epsilonStrings;

    // A vectors of semantic Id's
    typedef std::vector<SemanticId> IdVector;
    typedef std::stack<SemanticId> IdStack;

    // A mapping from one id to multiple ids
    typedef std::multimap<SemanticId, SemanticId> IdMultiIndex;

    // A relation from relation
    typedef boost::bimaps::bimap<SemanticId, OrderedRelation> RelationIndex;

    // Indexes / Database
    IdVector epsilonDomain;                             // Global domain of all symbols
    RelationIndex relations;                            // Bidirectional mapping between unqualified relations (domain, codomain) pairs and qualified codomain ids.
    IdMultiIndex domainIndexUCodomains;                 // All mappings from domain id to unqualified codomains
    IdMultiIndex domainIndexQCodomains;                 // All mappings from domain id to qualified codomains

    // Environment
    IdStack environment;                                // Environment of open domains
    SemanticId activeDomainId;                          // The last opened domain

    // Streams for messages to be output
    std::ostream errorStream;
    std::ostream warnStream;
    std::ostream infoStream;

    // Constants
    static const SemanticId epsilonId;                  // The global domain "Epsilon"
    static const char* const epsilonName;               // Name string of the global domain "Epsilon"
    static const SemanticId invalidId;                  // An invalid id
    
    // A helper for declaring a relation from an associative pair of ids
    SemanticId DeclareRelation(SemanticId domain, SemanticId codomain);
    
    // Get the semantic id represention the relation from domain to codomain
    SemanticId GetRelationId(SemanticId domain, SemanticId codomain);
    
    // Get the semantic id represention the relation from domain to codomain
    SemanticId GetTailId(SemanticId relationId);

    // Get the unqualified id from a qualified id (i.e. the tail part such that a.b -> b)
    SemanticId GetUnqualifiedId(SemanticId qualifiedId);
    
    // Test whether any relations exist from the given fully typed domain
    bool HasStaticRelations(SemanticId domain);
    
    // Create an id for the relation using both the domain and codomain
    SemanticId CreateQualifiedId(const Relation& relation);

    // Test whether an id matches the relation at some point in the relation.domain set of codomains
    //bool MatchDomain(SemanticId qualifiedCodomain, SemanticId matchDomain);

    /*// Split an id into a qualified relation pair (domain, codomain)
    void QualifiedSplit(SemanticId relationId, Relation& qualifiedRelation);

    // Split an id into a unqualified relation pair (domain, codomain)
    void UnqualifiedSplit(SemanticId relationId, Relation& unqualifiedRelation);*/
    
#ifdef _DEBUG
    // Debugging functions
    void DebugOutputEnvironment();
    void DebugOutputSymbolEnvironment(SemanticId domain, SemanticId qualifiedCodomain, uint indent);
#endif
  };
}

/*                                   INCLUDES                               */
#include "semanticdb.inl"

#endif
