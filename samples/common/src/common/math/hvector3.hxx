#ifdef __SAMPLES_HVECTOR3_H__
#ifndef __SAMPLES_HVECTOR3_HXX__
#define __SAMPLES_HVECTOR3_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    HVECTOR3.HXX
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
inline _HVector3<Type> operator * (const Type arg1, const _HVector3<Type>& arg2)
{ 
  return arg2 * arg1;
}

template<class Type>
_HVector3<Type>::_HVector3(const Type* f)
{
  HVector3::x = f[0]; HVector3::y = f[1]; HVector3::z = f[2]; HVector3::w = 0.0f;
}

template<class Type>
_HVector3<Type>::_HVector3(const _Vector3<Type>& xyz)
{
  HVector3::x = xyz(0); HVector3::y = xyz(1); HVector3::z = xyz(2); HVector3::w = 0.0f;
}

template<class Type>
_HVector3<Type>::_HVector3(const _HCoord3<Type>& arg)
{
  HVector3::x = arg.x; HVector3::y = arg.y; HVector3::z = arg.z; HVector3::w = 0.0f;
}

template<class Type>
_HVector3<Type>::_HVector3(Type x, Type y, Type z)
{
  HVector3::x = x; HVector3::y = y; HVector3::z = z; HVector3::w = 0.0f;
}

template<class Type>
_HVector3<Type>::~_HVector3()
{}

template<class Type>
_HVector3<Type>& _HVector3<Type>::operator *= (const _Matrix4<Type>& m)
{
  setTransform(m);
  return *this;
}

template<class Type>
_HVector3<Type>& _HVector3<Type>::setTransform(const _Matrix4<Type>& m)
{  
  this->HVector3::_HVector3( m(0,0) * HVector3::x + m(1,0) * HVector3::y + m(2,0) * HVector3::z,
                             m(0,1) * HVector3::x + m(1,1) * HVector3::y + m(2,1) * HVector3::z,
                             m(0,2) * HVector3::x + m(1,2) * HVector3::y + m(2,2) * HVector3::z );
  return *this;
}

template<class Type>
_HVector3<Type> _HVector3<Type>::getTransform(const _Matrix4<Type>& m)
{
  return _HVector3<Type>(m(0,0) * HVector3::x + m(1,0) * HVector3::y + m(2,0) * HVector3::z,
                         m(0,1) * HVector3::x + m(1,1) * HVector3::y + m(2,1) * HVector3::z,
                         m(0,2) * HVector3::x + m(1,2) * HVector3::y + m(2,2) * HVector3::z); 
}

template<class Type>
_HVector3<Type>& _HVector3<Type>::normalize()
{
  return *this /= HVector3::getLength();
}

#endif
#endif
