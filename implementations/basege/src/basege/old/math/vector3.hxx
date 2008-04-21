#ifdef __BASEGE_VECTOR3_H__
#ifndef __BASEGE_VECTOR3_HXX__
#define __BASEGE_VECTOR3_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    VECTOR3.HXX
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
_Vector3<Type> operator * (const Type arg1, const _Vector3<Type>& arg2)
{ 
  return arg2 * arg1;
}

template<class Type>
_Vector3<Type>::_Vector3(const Type* f)
{
  set(f);
}

template<class Type>
_Vector3<Type>::_Vector3(const _Vector3<Type>& arg)
{
  set((float*)arg);
}

template<class Type>
_Vector3<Type>::_Vector3(Type x, Type y, Type z) 
{
  set(x, y, z);
}

template<class Type>
void _Vector3<Type>::set(Type x, Type y, Type z)
{
  Vector3::x = x; Vector3::y = y; Vector3::z = z;
}

template<class Type>
void _Vector3<Type>::set(const Type* f)
{
  Vector3::x = f[0]; Vector3::y = f[1]; Vector3::z = f[2];
}

template<class Type>
_Vector3<Type>::~_Vector3()
{}

template<class Type>
Type& _Vector3<Type>::operator () (uint n)
{
  return v[n];
}

template<class Type>
Type _Vector3<Type>::operator () (uint n) const
{
  return v[n];
}

template<class Type>
void _Vector3<Type>::get(Type& xOut, Type& yOut, Type& zOut)
{
  xOut = x; yOut = y; zOut = z;
}

template<class Type>
_Vector3<Type>::operator Type* () const
{
  return (Type*)v;
}

template<class Type>
_Vector3<Type>& _Vector3<Type>::operator *= (const _Matrix3<Type>& m)
{
  setTransform(m);
  return *this;
}

template<class Type>
_Vector3<Type>& _Vector3<Type>::operator *= (const _Matrix4<Type>& m)
{
  setTransform(m);
  return *this;
}

template<class Type>
_Vector3<Type> _Vector3<Type>::operator - () const
{
  return _Vector3<Type>(-x,-y,-z);
}

template<class Type>
_Vector3<Type> _Vector3<Type>::operator + (const _Vector3<Type>& arg) const
{
  return _Vector3<Type>(x+arg.x, y+arg.y, z+arg.z); 
}

template<class Type>
_Vector3<Type> _Vector3<Type>::operator - (const _Vector3<Type>& arg) const
{
  return _Vector3<Type>(x-arg.x, y-arg.y, z-arg.z);
}

template<class Type>
_Vector3<Type> _Vector3<Type>::operator * (const Type arg)  const
{
  return _Vector3<Type>(x*arg, y*arg, z*arg);
}

template<class Type>
_Vector3<Type> _Vector3<Type>::operator / (const Type arg)  const
{
  return _Vector3<Type>(x/arg, y/arg, z/arg);
}

template<class Type>
_Vector3<Type>& _Vector3<Type>::operator += (const _Vector3<Type>& arg)
{
  x += arg.x; y += arg.y; z += arg.z;
  return *this;
}

template<class Type>
_Vector3<Type>& _Vector3<Type>::operator -= (const _Vector3<Type>& arg)
{
  x -= arg.x; y -= arg.y; z -= arg.z;
  return *this;
}

template<class Type>
_Vector3<Type>& _Vector3<Type>::operator *= (const Type arg)
{
  x *= arg; y *= arg; z *= arg;
  return *this;
}

template<class Type>
_Vector3<Type>& _Vector3<Type>::operator /= (const Type arg)
{
  x /= arg; y /= arg; z /= arg;
  return *this;
}

template<class Type>
template<class NewType>
_Vector3<Type>::operator _Vector3<NewType> ()
{ 
  return _Vector3<NewType>((NewType)x, (NewType)y, (NewType)z);
}

template<class Type>
Type _Vector3<Type>::dot(const _Vector3<Type>& v1, const _Vector3<Type>& v2)
{  
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template<class Type>
_Vector3<Type> _Vector3<Type>::cross(const _Vector3<Type>& v1, const _Vector3<Type>& v2)
{
  const Type& x1 = v1.x, y1 = v1.y, z1 = v1.z;
  const Type& x2 = v2.x, y2 = v2.y, z2 = v2.z;
  return _Vector3<Type>(y1*z2-z1*y2, -x1*z2+z1*x2, x1*y2-y1*x2);
}

template<class Type>
Type _Vector3<Type>::getLength() const
{
  return sqrt(x*x + y*y + z*z);
}

template<class Type>
Type _Vector3<Type>::getSquareLength() const
{
  return x*x + y*y + z*z;
}

template<class Type>
_Vector3<Type>& _Vector3<Type>::normalize()
{
  return *this /= getLength();
}

template<class Type>
_Vector3<Type> _Vector3<Type>::getNormalized() const
{
  return *this / getLength();
}

template<class Type>
_Vector3<Type>& _Vector3<Type>::setTransform(const _Matrix3<Type>& arg)
{  
  this->_Vector3<Type>::_Vector3( arg(0,0) * x + arg(1,0) * y + arg(2,0) * z + arg(3,0),
                    arg(0,1) * x + arg(1,1) * y + arg(2,1) * z + arg(3,1),
                    arg(0,2) * x + arg(1,2) * y + arg(2,2) * z + arg(3,2));
  return *this;
}

template<class Type>
_Vector3<Type>& _Vector3<Type>::setTransform(const _Matrix4<Type>& arg)
{  
  this->_Vector3<Type>::_Vector3( arg(0,0) * x + arg(1,0) * y + arg(2,0) * z + arg(3,0),
                    arg(0,1) * x + arg(1,1) * y + arg(2,1) * z + arg(3,1),
                    arg(0,2) * x + arg(1,2) * y + arg(2,2) * z + arg(3,2));
  return *this;
}

template<class Type>
_Vector3<Type> _Vector3<Type>::getTransform(const _Matrix3<Type>& arg)
{
  return _Vector3<Type>(arg(0,0) * x + arg(1,0) * y + arg(2,0) * z + arg(3,0),
                 arg(0,1) * x + arg(1,1) * y + arg(2,1) * z + arg(3,1),
                 arg(0,2) * x + arg(1,2) * y + arg(2,2) * z + arg(3,2)); 
}

template<class Type>
_Vector3<Type> _Vector3<Type>::getTransform(const _Matrix4<Type>& arg)
{
  return _Vector3<Type>(arg(0,0) * x + arg(1,0) * y + arg(2,0) * z + arg(3,0),
                 arg(0,1) * x + arg(1,1) * y + arg(2,1) * z + arg(3,1),
                 arg(0,2) * x + arg(1,2) * y + arg(2,2) * z + arg(3,2)); 
}

#endif
#endif
