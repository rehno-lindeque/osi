#ifdef __BASEGE_VECTOR4_H__
#ifndef __BASEGE_VECTOR4_HXX__
#define __BASEGE_VECTOR4_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    VECTOR4.HXX
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
inline _Vector4<Type> operator * (const Type arg1, const _Vector4<Type>& arg2)
{ 
  return arg2 * arg1;
}

template<class Type>
_Vector4<Type>::_Vector4(const Type* f)
{
  _Vector4<Type>::x = f[0]; _Vector4<Type>::y = f[1]; _Vector4<Type>::z = f[2]; _Vector4<Type>::w = f[3];
}

template<class Type>
_Vector4<Type>::_Vector4(const _Vector4<Type>& arg)
{
  x = arg.x; y = arg.y; z = arg.z; w = arg.w;
}

template<class Type>
_Vector4<Type>::_Vector4(Type x, Type y, Type z, Type w) 
{
  _Vector4<Type>::x = x; _Vector4<Type>::y = y; _Vector4<Type>::z = z; _Vector4<Type>::w = w;
}

template<class Type>
_Vector4<Type>::~_Vector4()
{}

template<class Type>
Type& _Vector4<Type>::operator () (uint n)
{
  return v[n];
}

template<class Type>
Type _Vector4<Type>::operator () (uint n) const
{
  return v[n];
}

template<class Type>
void _Vector4<Type>::get(Type& xOut, Type& yOut, Type& zOut, Type& wOut) const
{
  xOut = x; yOut = y; zOut = z; wOut = w;
}

template<class Type>
_Vector4<Type>::operator Type* () const
{
  return (Type*)v;
}

template<class Type>
_Vector4<Type>& _Vector4<Type>::operator *= (const _Matrix4<Type>& m)
{
  setTransform(m);
  return *this;
}

template<class Type>
_Vector4<Type> _Vector4<Type>::operator - () const
{
  return _Vector4<Type>(-x,-y,-z, -w);
}

template<class Type>
_Vector4<Type> _Vector4<Type>::operator + (const _Vector4<Type>& arg) const
{
  return _Vector4<Type>(x+arg.x, y+arg.y, z+arg.z, w+arg.w); 
}

template<class Type>
_Vector4<Type> _Vector4<Type>::operator - (const _Vector4<Type>& arg) const
{
  return _Vector4<Type>(x-arg.x, y-arg.y, z-arg.z, w-arg.w);
}

template<class Type>
_Vector4<Type> _Vector4<Type>::operator * (const Type arg)  const
{
  return _Vector4<Type>(x*arg, y*arg, z*arg, w*arg);
}

template<class Type>
_Vector4<Type> _Vector4<Type>::operator / (const Type arg)  const
{
  return _Vector4<Type>(x/arg, y/arg, z/arg, w/arg);
}

template<class Type>
_Vector4<Type>& _Vector4<Type>::operator += (const _Vector4<Type>& arg)
{
  x += arg.x; y += arg.y; z += arg.z; w += arg.w;
  return *this;
}

template<class Type>
_Vector4<Type>& _Vector4<Type>::operator -= (const _Vector4<Type>& arg)
{
  x -= arg.x; y -= arg.y; z -= arg.z; w -= arg.w;
  return *this;
}

template<class Type>
_Vector4<Type>& _Vector4<Type>::operator *= (const Type arg)
{
  x *= arg; y *= arg; z *= arg; w *= arg;
  return *this;
}

template<class Type>
_Vector4<Type>& _Vector4<Type>::operator /= (const Type arg)
{
  x /= arg; y /= arg; z /= arg; w /= arg;
  return *this;
}

template<class Type>
Type _Vector4<Type>::dot(const _Vector4<Type>& v1, const _Vector4<Type>& v2)
{  
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

template<class Type>
template<class NewType>
_Vector4<Type>::operator _Vector4<NewType> ()
{ 
  return _Vector4<NewType>((NewType)x, (NewType)y, (NewType)z, (NewType)w);
}

template<class Type>
Type _Vector4<Type>::getLength() const
{
  return sqrt(x*x + y*y + z*z + w*w);
}

template<class Type>
Type _Vector4<Type>::getSquareLength() const
{
  return x*x + y*y + z*z + w*w;
}

template<class Type>
_Vector4<Type>& _Vector4<Type>::normalize()
{
  return *this /= getLength();
}

template<class Type>
_Vector4<Type> _Vector4<Type>::getNormalized() const
{
  return *this / getLength();
}

template<class Type>
_Vector4<Type>& _Vector4<Type>::setTransform(const _Matrix4<Type>& arg)
{  
  this->_Vector4<Type>::_Vector4( arg(0,0) * x + arg(1,0) * y + arg(2,0) * z + arg(3,0) * w,
                                  arg(0,1) * x + arg(1,1) * y + arg(2,1) * z + arg(3,1) * w,
                                  arg(0,2) * x + arg(1,2) * y + arg(2,2) * z + arg(3,2) * w,
                                  arg(0,3) * x + arg(1,3) * y + arg(2,3) * z + arg(3,3) * w);
  return *this;
}

template<class Type>
_Vector4<Type> _Vector4<Type>::getTransform(const _Matrix4<Type>& arg)
{
  return _Vector4<Type>(arg(0,0) * x + arg(1,0) * y + arg(2,0) * z + arg(3,0) * w,
                        arg(0,1) * x + arg(1,1) * y + arg(2,1) * z + arg(3,1) * w,
                        arg(0,2) * x + arg(1,2) * y + arg(2,2) * z + arg(3,2) * w,
                        arg(0,3) * x + arg(1,3) * y + arg(2,3) * z + arg(3,3) * w); 
}

#endif
#endif
