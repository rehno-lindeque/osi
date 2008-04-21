#ifdef __SAMPLES_MATRIX3_H__
#ifndef __SAMPLES_MATRIX3_HXX__
#define __SAMPLES_MATRIX3_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    MATRIX3.HXX
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
_Matrix3<Type> operator * (Type arg1, const _Matrix3<Type>& arg2)
{
  return arg2 * arg1;
}

template<class Type>
_Matrix3<Type>::_Matrix3(const Type* f)
{
  set(f);
}

template<class Type>
_Matrix3<Type>::_Matrix3(const _Matrix3<Type>& m)
{
  set((Type*)m);
}

template<class Type>
_Matrix3<Type>::_Matrix3(const _Vector3<Type>& col1, const _Vector3<Type>& col2, const _Vector3<Type>& col3)
{
  set(col1, col2, col3);
}

template<class Type>
_Matrix3<Type>::_Matrix3(Type _00, Type _01, Type _02,
                         Type _10, Type _11, Type _12,
                         Type _20, Type _21, Type _22)
{
  set(_00, _01, _02,
	    _10, _11, _12,
	    _20, _21, _22);
}

template<class Type>
void _Matrix3<Type>::set(Type _00, Type _01, Type _02,
                         Type _10, Type _11, Type _12,
                         Type _20, Type _21, Type _22)
{
  Matrix3::_00 = _00; Matrix3::_01 = _01; Matrix3::_02 = _02;
  Matrix3::_10 = _10; Matrix3::_11 = _11; Matrix3::_12 = _12;
  Matrix3::_20 = _20; Matrix3::_21 = _21; Matrix3::_22 = _22;
}

template<class Type>
void _Matrix3<Type>::set(const Type* f)
{
  memcpy(m, f, 3*3*sizeof(Type));
}

template<class Type>
void _Matrix3<Type>::set(const _Vector3<Type>& col1, const _Vector3<Type>& col2, const _Vector3<Type>& col3)
{
  set(col1(0), col2(0), col3(0),
      col1(1), col2(1), col3(1),
      col1(2), col2(2), col3(2));
}

template<class Type>
Type& _Matrix3<Type>::operator () (uint row, uint col) 
{ 
  return m[row][col]; 
}

template<class Type>
Type _Matrix3<Type>::operator  () (uint row, uint col) const
{ 
  return m[row][col]; 
} 

template<class Type>
Type& _Matrix3<Type>::operator () (uint n) 
{ 
  return m[0][n]; 
}

template<class Type>
Type _Matrix3<Type>::operator  () (uint n) const
{ 
  return m[0][n]; 
} 

template<class Type>
_Matrix3<Type>::operator const Type* () const
{
  return (Type*)m;
}

#endif
#endif
