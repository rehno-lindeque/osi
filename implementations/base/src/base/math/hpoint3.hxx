#ifdef __BASE_HPOINT3_H__
#ifndef __BASE_HPOINT3_HXX__
#define __BASE_HPOINT3_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    HPOINT3.HXX
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
_HPoint3<Type>::_HPoint3(const Type* f)
{
  HPoint3::x = f[0]; HPoint3::y = f[1]; HPoint3::z = f[2]; HPoint3::w = 1.0f;
}

template<class Type>
_HPoint3<Type>::_HPoint3(const _Vector3<Type>& xyz)
{
  HPoint3::x = xyz(0); HPoint3::y = xyz(1); HPoint3::z = xyz(2); HPoint3::w = 1.0f;
}

template<class Type>
_HPoint3<Type>::_HPoint3(const _HCoord3<Type>& v)
{
  HPoint3::x = v.x; HPoint3::y = v.y; HPoint3::z = v.z; HPoint3::w = 1.0f;
}

template<class Type>
_HPoint3<Type>::_HPoint3(Type x, Type y, Type z)
{
  HPoint3::x = x; HPoint3::y = y; HPoint3::z = z; HPoint3::w = 1.0f;
}

template<class Type>
_HPoint3<Type>::~_HPoint3()
{}

template<class Type>
_HVector3<Type> _HPoint3<Type>::operator - (const _HPoint3& arg) const
{
  return _HVector3<Type>(HPoint3::x - arg.x, HPoint3::y - arg.y, HPoint3::z - arg.z);
}

template<class Type>
_HPoint3<Type>& _HPoint3<Type>::operator *= (const _Matrix4<Type>& m)
{
  setTransform(m);
  return *this;
}

template<class Type>
template<class NewType>
_HPoint3<Type>::operator _HPoint3<NewType> ()
{
  return _HPoint3<NewType>((NewType)_HPoint3::x, (NewType)_HPoint3::y, (NewType)_HPoint3::z);
}

template<class Type>
_HPoint3<Type>& _HPoint3<Type>::setTransform(const _Matrix4<Type>& m)
{
  this->HPoint3::_HPoint3( m(0,0) * HPoint3::x + m(1,0) * HPoint3::y + m(2,0) * HPoint3::z + m(3,0),
                           m(0,1) * HPoint3::x + m(1,1) * HPoint3::y + m(2,1) * HPoint3::z + m(3,1),
                           m(0,2) * HPoint3::x + m(1,2) * HPoint3::y + m(2,2) * HPoint3::z + m(3,2) );
  return *this;
}

template<class Type>
_HPoint3<Type> _HPoint3<Type>::getTransform(const _Matrix4<Type>& m)
{
  return HPoint3(m(0,0) * HPoint3::x + m(1,0) * HPoint3::y + m(2,0) * HPoint3::z + m(3,0),
                 m(0,1) * HPoint3::x + m(1,1) * HPoint3::y + m(2,1) * HPoint3::z + m(3,1),
                 m(0,2) * HPoint3::x + m(1,2) * HPoint3::y + m(2,2) * HPoint3::z + m(3,2));
}

#endif
#endif
