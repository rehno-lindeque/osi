#ifdef __BASE_HVECTOR3_H__
#ifndef __BASE_HVECTOR3_HXX__
#define __BASE_HVECTOR3_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    HVECTOR3.HXX
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
FORCE_INLINE  _HVector3<Type> operator * (const Type arg1, const _HVector3<Type>& arg2)
{
  return arg2 * arg1;
}

template<class Type>
FORCE_INLINE _HVector3<Type>::_HVector3(const Type* f)
{
  set(f[0], f[1], f[2], 0.0f);
}

template<class Type>
FORCE_INLINE _HVector3<Type>::_HVector3(const _Vector3<Type>& xyz)
{
  set(xyz(0), xyz(1), xyz(2), 0.0f);
}

template<class Type>
FORCE_INLINE _HVector3<Type>::_HVector3(const _HCoord3<Type>& arg)
{
  set(arg.x, arg.y, arg.z, 0.0f);
}

template<class Type>
FORCE_INLINE _HVector3<Type>::_HVector3(Type x, Type y, Type z)
{
  set(x, y, z, 0.0f);
}

template<class Type>
FORCE_INLINE _HVector3<Type>::~_HVector3()
{}

template<class Type>
FORCE_INLINE _HVector3<Type>& _HVector3<Type>::operator *= (const _Matrix4<Type>& m)
{
  setTransform(m);
  return *this;
}

template<class Type>
FORCE_INLINE _HVector3<Type>& _HVector3<Type>::setTransform(const _Matrix4<Type>& m)
{
  set(m(0,0) * HVector3::x + m(1,0) * HVector3::y + m(2,0) * HVector3::z,
      m(0,1) * HVector3::x + m(1,1) * HVector3::y + m(2,1) * HVector3::z,
      m(0,2) * HVector3::x + m(1,2) * HVector3::y + m(2,2) * HVector3::z );
  return *this;
}

template<class Type>
FORCE_INLINE _HVector3<Type> _HVector3<Type>::getTransform(const _Matrix4<Type>& m)
{
  return _HVector3<Type>(m(0,0) * HVector3::x + m(1,0) * HVector3::y + m(2,0) * HVector3::z,
                         m(0,1) * HVector3::x + m(1,1) * HVector3::y + m(2,1) * HVector3::z,
                         m(0,2) * HVector3::x + m(1,2) * HVector3::y + m(2,2) * HVector3::z);
}

template<class Type>
FORCE_INLINE _HVector3<Type>& _HVector3<Type>::normalize()
{
  return *this /= HVector3::getLength();
}

#endif
#endif
