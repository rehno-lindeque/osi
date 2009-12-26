#ifndef __BASESEMANTICDB_ORDEREDRELATION_H__
#define __BASESEMANTICDB_ORDEREDRELATION_H__
//////////////////////////////////////////////////////////////////////////////
//
//    ORDEREDRELATION.H
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Sample base classes for OpenSemanticDB backends.

    DEPENDENCIES:
     semanticdb.h or semanticdb.hpp must be included before basesemanticdb.h
*/
/*                              COMPILER MACROS                             */
/*                                 INCLUDES                                 */
/*                            FORWARD DECLERATIONS                          */
/*                                 CLASSES                                  */
namespace BaseSemanticDB
{
  class OrderedRelation : public OSIX::SemanticDB::Relation
  {
  public:
    inline OrderedRelation(const OSIX::SemanticDB::Relation& relation) : Relation(relation) {}
  };

  inline bool operator == (const OrderedRelation& arg1, const OrderedRelation& arg2)
  {
    return arg1.domain == arg2.domain && arg1.codomain == arg2.codomain;
  }
  
  inline bool operator < (const OrderedRelation& arg1, const OrderedRelation& arg2)
  {
    return (arg1.domain < arg2.domain) || (arg1.domain == arg2.domain && arg1.codomain < arg2.codomain);
  }
}

/*                              IMPLEMENTATION                              */

/*                              COMPILER MACROS                             */

#endif
