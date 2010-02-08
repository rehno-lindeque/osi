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

  enum QueryType
  {
    QueryNone,
    QuerySelectionDisjunct,
    QuerySelectionExclusiveDisjunct,
    QuerySelectionConjunct,
    QuerySelectionStrictConjunct,
    //QuerySelectionStrictExclusiveDisjunct,
    QueryMutationDisjunct,
    QueryMutationExclusiveDisjunct,
    QueryMutationConjunct,
    QueryMutationStrictConjunct
    //QueryMutationStrictExclusiveDisjunct,
    //Effect
  };

  struct Query
  {
    inline Query(QueryType type, SemanticId subject, SemanticId queryArgument) : type(type), subject(subject), queryArgument(queryArgument) {}
    QueryType type;
    SemanticId subject;
    SemanticId queryArgument;
  };
  inline bool operator == (const Query& arg1, const Query& arg2)
  {
    return arg1.type == arg2.type && arg1.subject == arg2.subject && arg1.queryArgument == arg2.queryArgument;
  }
  inline bool operator < (const Query& arg1, const Query& arg2)
  {
    return arg1.type < arg2.type || ((arg1.type == arg2.type && arg1.subject < arg2.subject) || (arg1.type == arg2.type && arg1.subject == arg2.subject && arg1.queryArgument < arg2.queryArgument));
  }
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
    // Types
    typedef std::vector<SemanticId> IdVector;
    typedef std::stack<SemanticId> IdStack;

    // Constructor
    INLINE SemanticDBImplementation();

    //Destructor
    virtual ~SemanticDBImplementation();

    SemanticId DeclareSymbol(const char* name);
    SemanticId GlobalSymbol(const char* name);
    SemanticId DeclareAnonymousSymbol();
    SemanticId DeclareRelation(const Relation& unqualifiedRelation);
    //SemanticId SelectRelation(const Relation& relation);
    void SelectRelation(const Relation& unqualifiedRelation, IdVector& qualifiedCodomains);
    void SelectExtendedRelation(const Relation& unqualifiedRelation, IdVector& qualifiedCodomains);
    SemanticId DeclareOpenDomain(const char* name);
    void CloseDomain(const char* name);
    void CloseDomain();

    // Builtin morphisms (todo: move this to "queries")
    //SemanticId ConjunctMorphism(SemanticId domain, SemanticId crossSection);

    // queries
    void BeginQuery();
    SemanticId EndQuery();

    void SelectionDisjunct();
    void SelectionExclusiveDisjunct();
    void SelectionConjunct();
    void SelectionStrictConjunct();

    void MutationDisjunct();
    void MutationExclusiveDisjunct();
    void MutationConjunct();
    void MutationStrictConjunct();

    // Evaluation
    void BeginEvaluation(SemanticId root);
    SemanticId Eval();
    void EndEvaluation();
    SemanticId GetActiveQuery();

    SemanticId GetDomain(SemanticId symbol);
    const char* GetString(SemanticId symbol);
    int16 GetInt16(SemanticId symbol);
    int32 GetInt32(SemanticId symbol);
    int64 GetInt64(SemanticId symbol);
    float GetFloat(SemanticId symbol);
    double GetDouble(SemanticId symbol);

    // Initialization
    void Init();

    // Message streams
    INLINE void SetErrorStream(FILE* stream);
    INLINE void SetWarningStream(FILE* stream);
    INLINE void SetInfoStream(FILE* stream);

  protected:
    // (string, SemanticID) mappings
    typedef std::pair<std::string, SemanticId> StringIdPair;
    typedef std::unordered_map<std::string, SemanticId, std::hash<std::string>, std::equal_to<std::string> > StringIdMap;

    // The map of all tokens and their ids (in any context)
    StringIdMap tokens;
    SemanticId nextSymbolId;  // The next id to be assigned to a token in DeclareGlobal

    // Global domain of all literals (whose hash function are not equal to their own values)
    // (Note: this is equivalent to a perfect hash map since hash values that may contain conflicts are mapped
    // to permanent tokens that do not contain conflicts)
    typedef std::map<SemanticId, std::string> IdStringMap;
    IdStringMap epsilonStrings;

    // A mapping from one id to multiple ids
    typedef std::multimap<SemanticId, SemanticId> IdMultiIndex;
    typedef IdMultiIndex::iterator IdMultiIndexIterator;
    typedef std::pair<IdMultiIndexIterator, IdMultiIndexIterator> IdMultiIndexRange;

    // A bidirectional mapping between unqualified relations (domain, codomain) pairs and qualified codomain ids
    typedef boost::bimaps::bimap<SemanticId, OrderedRelation> RelationIndex;
    typedef RelationIndex::value_type RelationIndexValue;

    // A list of all queries
    typedef boost::bimaps::bimap<SemanticId, Query> QueryIndex;
    typedef QueryIndex::value_type QueryIndexValue;

    // Additional symbol properties
    struct SymbolProperties { SymbolProperties(bool concrete = true, QueryType query = QueryNone) : concrete(concrete), query(query) {} bool concrete; QueryType query; };
    typedef std::pair<SemanticId, SymbolProperties> IdPropertiesPair;
    typedef std::map<SemanticId, SymbolProperties> IdPropertiesMap;
    typedef IdPropertiesMap::iterator IdPropertiesIterator;

    // The state of the current building environment
    enum EnvironmentState
    {
      BUILD_DECLARATION,
      BUILD_QUERY_SOURCE,
      BUILD_QUERY_ARGUMENT = 0x08000000
    };
    typedef std::stack<EnvironmentState> EnvironmentStateStack;

    // Indexes / Database
    IdVector epsilonDomain;                             // Global domain of all (named) symbols
    IdVector anonDomain;                                // Global domain of all anonymous symbols
    RelationIndex relations;                            // Bidirectional mapping between unqualified relations (domain, codomain) pairs and qualified codomain ids.
    IdMultiIndex domainIndexUCodomains;                 // All mappings from domain id to unqualified codomains
    IdMultiIndex domainIndexQCodomains;                 // All mappings from domain id to qualified codomains
    QueryIndex queries;                                 // All queries in the database (these may be memoized later on)
    IdPropertiesMap symbolProperties;                   // Additional properties for each symbol

    // Evaluation
    IdStack evaluationQueries;                          // Stack of evaluation queries
    //IdStack activeQueries;                              // Stack of queries in progress
    SemanticId evalQueryId;                           // The last query returned by Eval

    // Environment
    IdStack domainEnvironment;                          // Environment of open domains
    EnvironmentStateStack environmentState;             // State of the environment (e.g. building a query etc)
    SemanticId activeDomainId;                          // The last opened domain
    IdStack queryEnvironment;                           // TODO: Environment of speculative symbols
    //IdStack queryDomains;

    // Streams for messages to be output
    std::ostream errorStream;
    std::ostream warnStream;
    std::ostream infoStream;

    // Constants
    static const char* const epsilonName;               // Name string of the global domain "Epsilon"
    static const SemanticId firstReservedId;            // The first id that is reserved for built-in ids

    // A helper for declaring a relation from an associative pair of ids
    SemanticId DeclareRelation(SemanticId domain, SemanticId codomain);

    // Declare a speculative relation from an associative pair of ids
    SemanticId DeclareSpeculativeRelation(SemanticId domain, SemanticId codomain);
    SemanticId DeclareSpeculativeRelation(const Relation& unqualifiedRelation);

    // Put the builder into the correct state for accepting query arguments
    void OpenQueryArgument(QueryType queryType);

    // Get the semantic id represention the relation from domain to codomain
    //SemanticId GetRelationId(SemanticId domain, SemanticId codomain);

    // Get the semantic id represention the relation from domain to codomain
    //SemanticId GetTailId(SemanticId relationId);

    // Get the unqualified id from a qualified id (i.e. the tail part such that a.b -> b)
    SemanticId GetUnqualifiedCodomain(SemanticId qualifiedId);

    // Get the unqualified relation
    bool GetUnqualifiedRelation(SemanticId qualifiedId, Relation& unqualifiedRelation);

    // Get the qualified codomain from a unqualified relation
    // Return invalid id if the relation does not exist
    SemanticId GetQualifiedCodomain(const OrderedRelation& unqualifiedRelation) const;

    // Test whether any relations exist from the given fully qualified domain
    bool HasStaticRelations(SemanticId domain) const;

    // Test whether a relation already exists
    //bool DoesRelationExist(const OrderedRelation& unqualifiedRelation) const;

    // Create an id for the relation using both the domain and codomain
    SemanticId CreateQualifiedId(const Relation& relation);

    // Get the properties associated with a symbol
    const SymbolProperties& GetProperties(SemanticId domain) const;

    // Add a query to the database
    //SemanticId CreateQuery(QueryType type,SemanticId domain, SemanticId argument);
    void CreateQuery(QueryType type, SemanticId relation);

///////////////////////////////////////////////////
// EVALUATION stuff

    // Internal eval function
    SemanticId EvalInternal();

    // Continue evaluation (set up for the next iteration)
    void EvalContinue();

    // Schedule codomain edges for later evaluation (push it onto the evaluation stack)
    void EvalScheduleCodomains();

    // Evaluate a symbol (non-query)
    SemanticId EvalSymbol(SemanticId symbol);

    // Resolve a speculative context with the first concrete parent context
    SemanticId ResolveContext(SemanticId domain);

    // Resolve a relation
    // Modifies the domain part of the unqualified relation until the codomain can be found in it
    // Returns the first qualified codomain corresponding to the unqualified codomain in a parent context
    SemanticId ResolveRelation(OrderedRelation& relation);

    // Resolve a selection at compile-time
    SemanticId ResolveConjunctSelection(SemanticId symbol);

    // Declare a speculative Id
    //DeclareSpeculativeId(QueryType)

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
