#ifndef __QSEMANTICDB_API_H__
#define __QSEMANTICDB_API_H__
//////////////////////////////////////////////////////////////////////////////
//
//    API.H
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      QSemanticDB API for OpenSemanticDB 0.1.
*/
/*                              COMPILER MACROS                             */
// Extended standard library flags
#ifdef _MSC_VER
# define STDEXT_NAMESPACE stdext
#else
# define STDEXT_NAMESPACE __gnu_cxx
#endif

// Verbose debugging flags
#ifdef _DEBUG
# define QSEMANTICDB_DEBUG_VERBOSE
//# define QSEMANTICDB_DEBUG_DETAILEDEVAL
# ifdef QSEMANTICDB_DEBUG_VERBOSE
#   define QSEMANTICDB_DEBUG_EVALOUTPUT
# endif
#else
# undef QSEMANTICDB_DEBUG_VERBOSE
# undef QSEMANTICDB_DEBUG_DETAILEDEVAL
# undef QSEMANTICDB_DEBUG_EVALOUTPUT
#endif

/*                                 INCLUDES                                 */
// OpenSemanticDB
#include <osix/semanticdb/semanticdb.hpp>
#ifdef _DEBUG
# include <osix/semanticdb/semanticdbdbg.hpp>
#endif

// BaseSemanticDB
#include <basesemanticdb/basesemanticdb.h>
#include <basesemanticdb/orderedrelation.h>

// C++ Std
#include <vector>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <queue>

// C++ Std extensions
#ifdef _MSC_VER
# include <ext/stdio_filebuf.h>?
#else
# include <ext/stdio_filebuf.h>
#endif

// Boost
//#define BOOST_BIMAP_DISABLE_SERIALIZATION
//#define BOOST_NO_EXCEPTIONS
#include <boost/bimap/bimap.hpp>

// CLib
//#include <memory.h>

// Boost
//#include <boost/bimap.hpp>


/*                            FORWARD DECLARATIONS                          */

/*                                   TYPES                                  */
namespace QSemanticDB
{
  typedef OSIX::SemanticDB::Relation Relation;
  typedef OSIX::SemanticId SemanticId;
}

/*                                   INCLUDES                               */
// QSemanticDB
#include "schedule.h"
#include "semanticdb.h"

/*                                  CLASSES                                 */
namespace QSemanticDB
{
  class SemanticDB : public BaseSemanticDB::SemanticDB
  {
  public:
    SemanticDBImplementation semanticDB;

    // Constructor
    INLINE SemanticDB() {}

    // Initialization
    //void Init();

    // Semantic DB implementation
    SemanticDBImplementation* operator-> ();

    // Debug classes
#   ifdef _DEBUG
      class SemanticDBDbg : public OSIX::SemanticDBDbg
      {
      public:
        INLINE SemanticDB& GetSemanticDB() { return *(SemanticDB*)(((uint8*)this) - offsetof(SemanticDB, semanticDBDbg)); }
      } semanticDBDbg;
#   endif
  };
}

/*                                   INCLUDES                               */

// Inline implementation
#ifdef OSI_STATIC_C_BUILD
# include "api.inl"
# ifdef _DEBUG
#   include "apidbg.inl"
# endif
#endif

#endif
