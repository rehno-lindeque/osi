#ifdef __SAMPLES_QUATERNION_H__
#ifndef __SAMPLES_QUATERNION_HXX__
#define __SAMPLES_QUATERNION_HXX__
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
  return q[n];
}

template<class Type>
Type  _Quaternion<Type>::operator () (uint n) const
{
  return q[n];
}

template<class Type>
void _Quaternion<Type>::get(Type& xOut, Type& yOut, Type& zOut, Type& wOut) const
{
  xOut = x;
  yOut = y;
  zOut = z;
  wOut = w;
}

template<class Type>
_Quaternion<Type>::operator Type* () const
{
  return (Type*)q;
}

#endif
#endif
