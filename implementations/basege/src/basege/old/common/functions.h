#ifndef __BASEGE_FUNCTIONS_H__
#define __BASEGE_FUNCTIONS_H__
//////////////////////////////////////////////////////////////////////////////
//
//    FUNCTIONS.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Defines common functions for use in OpenGE backends.
*/

/*                                FUNCTIONS                                */
template<typename Type> inline Type min(const Type& a, const Type& b) { return a>b?b:a; }
template<typename Type> inline Type max(const Type& a, const Type& b) { return a<b?b:a; }

#endif
