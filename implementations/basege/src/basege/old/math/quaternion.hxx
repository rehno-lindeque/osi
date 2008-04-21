#ifdef __BASEGE_QUATERNION_H__
#ifndef __BASEGE_QUATERNION_HXX__
#define __BASEGE_QUATERNION_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    QUATERNION.HXX
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
Type& _Quaternion<Type>::operator () (uint n)
{
  return Q[n];
}

template<class Type>
Type  _Quaternion<Type>::operator () (uint n) const
{
  return Q[n];
}

template<class Type>
void _Quaternion<Type>::get(Type& xOut, Type& yOut, Type& zOut, Type& wOut) const
{
  xOut = x;
  yOut = y;
  zOut = z;
  wOut = w;
}

#endif
#endif
