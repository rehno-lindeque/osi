#ifndef __BASE_FUNCTIONS_H__
#define __BASE_FUNCTIONS_H__
//////////////////////////////////////////////////////////////////////////////
//
//    FUNCTIONS.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Defines common functions for use in OSI backends.
*/

/*                                FUNCTIONS                                */
template<typename Type> FORCE_INLINE Type min(const Type& a, const Type& b) { return a>b?b:a; }
template<typename Type> FORCE_INLINE Type max(const Type& a, const Type& b) { return a<b?b:a; }

#if defined(_MSC_VER) && defined(_ASSERT)
# define OSI_ASSERT(condition) _ASSERT(condition)
#else
//todo: find a gnu assert alternative...
//# define OSI_ASSERT(condition) _assert(condition)
# include <assert.h>
# define OSI_ASSERT(condition) assert(condition)
#endif

#ifdef _DEBUG
# define OSI_DEBUG(statement) statement
#else
# define OSI_DEBUG(statement)
#endif

#endif
