//////////////////////////////////////////////////////////////////////////////
//
//    API.CPP
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    IMPLEMENTATION:

    TODO:
*/

/*                              COMPILER MACROS                             */
#ifdef _MSC_VER
# pragma warning(push)
#endif
/*                                 INCLUDES                                 */
#include "api.h"

#ifndef OSI_STATIC_C_BUILD
# include "api.inl"
# ifdef _DEBUG
#   include "apidbg.inl"
# endif
#endif

/*                                  MACROS                                  */
#ifdef _MSC_VER
# pragma warning(pop)
#endif

/*                                  GLOBALS                                 */
const OSIX::SemanticId QSemanticDB::SemanticDBImplementation::invalidId = ~OSIX::SemanticId(0);
const char* const QSemanticDB::SemanticDBImplementation::epsilonName = "Epsilon";
const OSIX::SemanticId QSemanticDB::SemanticDBImplementation::epsilonId = ~OSIX::SemanticId(0) - 1;