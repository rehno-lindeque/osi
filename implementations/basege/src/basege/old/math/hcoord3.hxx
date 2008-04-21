#ifdef __BASEGE_HCOORD3_H__
#ifndef __BASEGE_HCOORD3_HXX__
#define __BASEGE_HCOORD3_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    HCOORD3.HXX
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
inline _HCoord3<Type> operator * (const Type arg1, const _HCoord3<Type>& arg2)
{
  return arg2 * arg1;
}

template<class Type>
_HCoord3<Type>::_HCoord3(const Type* f)
{
  x = f[0]; y = f[1]; z = f[2]; w = f[3];
}

template<class Type>
_HCoord3<Type>::_HCoord3(const _Vector3<Type>& xyz, const Type w) 
{
  _HCoord3<Type>::x = xyz(0); _HCoord3<Type>::y = xyz(1); _HCoord3<Type>::z = xyz(2); _HCoord3<Type>::w = w;
}

template<class Type>
_HCoord3<Type>::_HCoord3(const _Vector3<Type>& v) 
{
  _HCoord3<Type>::x = v(0); _HCoord3<Type>::y = v(1); _HCoord3<Type>::z = v(2); _HCoord3<Type>::w = 0.0f;
}

template<class Type>
_HCoord3<Type>::_HCoord3(const _HCoord3<Type>& v)
{
  x = v.x; y = v.y; z = v.z; w = v.w;
}

template<class Type>
_HCoord3<Type>::_HCoord3(const Type x, const Type y, const Type z, const Type w)
{
  _HCoord3<Type>::x = x; _HCoord3<Type>::y = y; _HCoord3<Type>::z = z; _HCoord3<Type>::w = w;
}

template<class Type>
_HCoord3<Type>::~_HCoord3()
{}

template<class Type>
Type& _HCoord3<Type>::operator () (uint n)
{
  return v[n];
}

template<class Type>
Type _HCoord3<Type>::operator () (uint n) const
{
  return v[n];
}

template<class Type>
void _HCoord3<Type>::get(Type& xOut, Type& yOut, Type& zOut, Type& wOut) const
{
  xOut = x; yOut = y; zOut = z; wOut = w;
}

template<class Type>
void _HCoord3<Type>::get(Type& xOut, Type& yOut, Type& zOut) const
{
  xOut = x; yOut = y; zOut = z;
}

template<class Type>
_HCoord3<Type>::operator Type* () const
{
  return (Type*)v;
}

template<class Type>
_HCoord3<Type>& _HCoord3<Type>::operator *= (const _Matrix3<Type>& m)
{
  setTransform(m);
  return *this;
}

template<class Type>
_HCoord3<Type>& _HCoord3<Type>::operator *= (const _Matrix4<Type>& m)
{
  setTransform(m);
  return *this;
}

template<class Type>
_HCoord3<Type>& _HCoord3<Type>::operator += (const _HVector3<Type>& arg)
{
  x += arg.x; y += arg.y; z += arg.z;
  return *this;
}

/*template<class Type>
_HCoord3<Type>& _HCoord3<Type>::operator -= (const _HCoord3<Type>& arg)
{
  x -= arg.x; y -= arg.y; z -= arg.z;
  return *this;
}*/

template<class Type>
_HCoord3<Type>& _HCoord3<Type>::operator -= (const _HVector3<Type>& arg)
{
  x -= arg.x; y -= arg.y; z -= arg.z;
  return *this;
}

template<class Type>
_HCoord3<Type>& _HCoord3<Type>::operator *= (const Type arg)
{
  x *= arg; y *= arg; z *= arg;
  return *this;
}

template<class Type>
_HCoord3<Type>& _HCoord3<Type>::operator /= (const Type arg)
{
  x /= arg; y /= arg; z /= arg;
  return *this;
}

template<class Type>
_HCoord3<Type> _HCoord3<Type>::operator - () const
{
  return _HCoord3<Type>(-x,-y,-z,w);
}

template<class Type>
_HCoord3<Type> _HCoord3<Type>::operator + (const _HVector3<Type>& arg) const
{
  return _HCoord3<Type>(x+arg.x, y+arg.y, z+arg.z, w); 
}

/*template<class Type>
_HVector3<Type> _HCoord3<Type>::operator - (const _HCoord3<Type>& arg) const
{
  return _HVector3<Type>(x-arg.x, y-arg.y, z-arg.z); 
}*/

template<class Type>
_HCoord3<Type> _HCoord3<Type>::operator - (const _HVector3<Type>& arg) const
{
  return _HCoord3<Type>(x-arg.x, y-arg.y, z-arg.z, w); 
}

template<class Type>
_HCoord3<Type> _HCoord3<Type>::operator * (const Type arg)  const
{
  return _HCoord3<Type>(x*arg, y*arg, z*arg, w);
}

template<class Type>
_HCoord3<Type> _HCoord3<Type>::operator / (const Type arg)  const
{
  return _HCoord3<Type>(x/arg, y/arg, z/arg, w);
}

template<class Type>
template<class NewType>
_HCoord3<Type>::operator _HCoord3<NewType> ()
{ 
  return _HCoord3<NewType>((NewType)x, (NewType)y, (NewType)z, (NewType) w);
}

template<class Type>
Type _HCoord3<Type>::dot(const _HCoord3<Type>& v1, const _HCoord3<Type>& v2)
{  
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template<class Type>
_HVector3<Type> _HCoord3<Type>::cross(const _HCoord3<Type>& v1, const _HCoord3<Type>& v2)
{
  const Type& x1 = v1.x, y1 = v1.y, z1 = v1.z;
  const Type& x2 = v2.x, y2 = v2.y, z2 = v2.z;
  return _HVector3<Type>(y1*z2-z1*y2, -x1*z2+z1*x2, x1*y2-y1*x2);
}

template<class Type>
_HVector3<Type> _HCoord3<Type>::getNormalized() const
{
  return *this / getLength();
}

template<class Type>
Type _HCoord3<Type>::getLength() const
{
  return sqrt(x*x + y*y + z*z);
}

template<class Type>
_HCoord3<Type>& _HCoord3<Type>::setTransform(const _Matrix3<Type>& m)
{  
  this->_HCoord3<Type>::_HCoord3( m(0,0) * x + m(1,0) * y + m(2,0) * z,
                                  m(0,1) * x + m(1,1) * y + m(2,1) * z,
                                  m(0,2) * x + m(1,2) * y + m(2,2) * z );
  return *this;
}

template<class Type>
_HCoord3<Type>& _HCoord3<Type>::setTransform(const _Matrix4<Type>& m)
{  
  this->_HCoord3<Type>::_HCoord3( m(0,0) * x + m(1,0) * y + m(2,0) * z + m(3,0) * w,
                                  m(0,1) * x + m(1,1) * y + m(2,1) * z + m(3,1) * w,
                                  m(0,2) * x + m(1,2) * y + m(2,2) * z + m(3,2) * w,
                                  m(0,3) * x + m(1,3) * y + m(2,3) * z + m(3,3) * w );
  return *this;
}

template<class Type>
_HCoord3<Type> _HCoord3<Type>::getTransform(const _Matrix3<Type>& m)
{
  return _HCoord3<Type>(m(0,0) * x + m(1,0) * y + m(2,0) * z,
                        m(0,1) * x + m(1,1) * y + m(2,1) * z,
                        m(0,2) * x + m(1,2) * y + m(2,2) * z,
                        w ); 
}

template<class Type>
_HCoord3<Type> _HCoord3<Type>::getTransform(const _Matrix4<Type>& m)
{
  return _HCoord3<Type>(m(0,0) * x + m(1,0) * y + m(2,0) * z + m(3,0) * w,
                        m(0,1) * x + m(1,1) * y + m(2,1) * z + m(3,1) * w,
                        m(0,2) * x + m(1,2) * y + m(2,2) * z + m(3,2) * w,
                        m(0,3) * x + m(1,3) * y + m(2,3) * z + m(3,3) * w ); 
}

#endif
#endif
