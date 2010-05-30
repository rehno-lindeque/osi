#ifdef  __QSEMANTICDB_SEMANTICDB_H__
#ifndef __QSEMANTICDB_SEMANTICDB_DEBUG_INL__
#define __QSEMANTICDB_SEMANTICDB_DEBUG_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    SEMANTICDB_DEBUG.INL
//
//    Copyright © 2009-2010, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QSemanticDB
{

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
               << '{';
    for(IdVector::iterator i = epsilonDomain.begin(); i != epsilonDomain.end(); ++i)
    {
      infoStream << endl;
      for(uint c = 0; c < 1; ++c) infoStream << "  ";
      DebugOutputSymbolEnvironment(OSIX::SEMANTICID_EPSILON, *i, 1);
    }
    infoStream << endl << '}' << endl;
  }

  void SemanticDBImplementation::DebugOutputSymbolEnvironment(SemanticId domain, SemanticId qualifiedCodomain, uint indent)
  {
    using std::endl;

    SemanticId unqualifiedCodomain = GetUnqualifiedCodomain(qualifiedCodomain);

    // Write the symbol
    if(domain == OSIX::SEMANTICID_EPSILON)
    {
      //infoStream << epsilonName << ".";
      DebugOutputAtom(qualifiedCodomain);
      infoStream << '(' << qualifiedCodomain << ')' << " = " << qualifiedCodomain;
    }
    else if(unqualifiedCodomain == OSIX::SEMANTICID_INVALID)
      infoStream << "Invalid (no tail for " << domain << " -> " << qualifiedCodomain << ")";
    else
    {
      IdStringMap::iterator i = epsilonStrings.find(unqualifiedCodomain);
      if(i == epsilonStrings.end())
      {
        if(unqualifiedCodomain == INTERNALID_HIDDEN)
          infoStream << "Hidden";
        else
          infoStream << "Anonymous";
      }
      else
        infoStream << i->second;

      // Test  whether the qualified codomain is concrete (if not, then this is an (unresolved) query)
      IdPropertiesIterator iProperties = symbolProperties.find(qualifiedCodomain);
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
      case QueryNone:                                 break;
      }


      infoStream << (concrete? "(" : "(? ") << qualifiedCodomain << " = " << domain << relationOperator << unqualifiedCodomain << (concrete? ")" : " ?)");
    }

    // Check whether the qualified codomain has relations to other codomains (I.e. whether qualifiedCodomain is a domain in another context)
    const IdMultiIndexRange r = domainIndexQCodomains.equal_range(qualifiedCodomain);
    if(r.first == r.second)
      return;

    // Determine whether there is more than one relation from this symbol
    IdMultiIndexIterator iNext = r.first; ++iNext;
    bool hasMultipleRelations = (r.second != iNext);

    // Recursively output the child relations
    infoStream << " -> ";

    if(hasMultipleRelations)
    {
      infoStream << endl;
      for(uint c = 0; c < indent; ++c) infoStream << "  ";
      infoStream << '{';
    }

    for(IdMultiIndexIterator i = r.first; i != r.second; ++i)
    {
      if(hasMultipleRelations)
      {
        infoStream << endl;
        for(uint c = 0; c < indent+1; ++c) infoStream << "  ";
      }
      DebugOutputSymbolEnvironment(qualifiedCodomain, i->second, hasMultipleRelations? indent+1 : indent);
    }

    if(hasMultipleRelations)
    {
      infoStream << endl;
      for(uint c = 0; c < indent; ++c) infoStream << "  ";
      infoStream << '}';
    }
  }

  void SemanticDBImplementation::DebugOutputAtom(SemanticId atom)
  {
      IdStringMap::iterator i = epsilonStrings.find(atom);
      if(i == epsilonStrings.end())
        infoStream << "Anonymous";
      else
        infoStream << i->second;
  }
#endif
}

#endif
#endif
