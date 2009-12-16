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
#elif defined(QSEMANTICDB_DEBUG_VERBOSE)
# undef QSEMANTICDB_DEBUG_VERBOSE
#endif
        
/*                                 INCLUDES                                 */
// OpenParser
#include <osix/semanticdb/semanticdb.hpp>
#ifdef _DEBUG
# include <osix/semanticdb/semanticdbdbg.hpp>
#endif

// BaseParser
#include <basesemanticdb/basesemanticdb.h>

// C++ Std
/*#include <algorithm>
#include <list>*/
#include <vector>
/*#include <stack>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <fstream>*/
#include <string>
#include <unordered_map>
/*#include <unordered_set>*/

// C++ Std extensions
#ifdef _MSC_VER
# include <ext/stdio_filebuf.h>?
#else
# include <ext/stdio_filebuf.h>
#endif

// CLib
//#include <memory.h>

// Boost
//#include <boost/bimap.hpp>



namespace QSemanticDB
{
/*                            FORWARD DECLARATIONS                          */
  class SemanticDBImplementation;

/*                                   TYPES                                  */
  typedef OSIX::SemanticDB::Relation Relation;
  typedef OSIX::SemanticId SemanticId;
  
/*                                  CLASSES                                 */  
  class SemanticDB : public BaseSemanticDB::SemanticDB
  {
  public:
    SemanticDBImplementation* semanticDB;

    // Constructor
    FORCE_INLINE SemanticDB() {}
       
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
// QSemanticDB
#include "semanticdb.h"

// Inline implementation
#ifdef OSI_STATIC_C_BUILD
# include "api.inl"
# ifdef _DEBUG
#   include "apidbg.inl"
# endif
#endif

#endif
