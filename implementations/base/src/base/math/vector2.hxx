#ifdef __BASE_VECTOR2_H__
#ifndef __BASE_VECTOR2_HXX__
#define __BASE_VECTOR2_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    VECTOR2.HXX
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
inline _Vector2<Type> operator * (const Type arg1, const _Vector2<Type>& arg2)
{
  return arg2 * arg1;
}

template<class Type>
_Vector2<Type>::_Vector2(const Type* f)
{
  set(f);
}

template<class Type>
_Vector2<Type>::_Vector2(Type x, Type y)
{
  set(x, y);
}

template<class Type>
_Vector2<Type>::_Vector2(const _Vector2<Type>& arg)
{
  set(arg.x, arg.y);
}

template<class Type>
void _Vector2<Type>::set(Type x, Type y)
{
  Vector2::x = x; Vector2::y = y;
}

template<class Type>
void _Vector2<Type>::set(const Type* f)
{
  Vector2::x = f[0]; Vector2::y = f[1];
}

template<class Type>
_Vector2<Type>::~_Vector2()
{}

template<class Type>
Type& _Vector2<Type>::operator () (uint n)
{
  return v[n];
}

template<class Type>
Type _Vector2<Type>::operator () (uint n) const
{
  return v[n];
}

template<class Type>
void _Vector2<Type>::get(Type& xOut, Type& yOut) const
{
  xOut = x; yOut = y;
}

template<class Type>
_Vector2<Type>::operator Type* () const
{
  return (Type*)v;
}

template<class Type>
_Vector2<Type> _Vector2<Type>::operator - () const
{
  return _Vector2<Type>(-x,-y);
}

template<class Type>
_Vector2<Type> _Vector2<Type>::operator + (const _Vector2<Type>& arg) const
{
  return _Vector2<Type>(x+arg.x, y+arg.y);
}

template<class Type>
_Vector2<Type> _Vector2<Type>::operator - (const _Vector2<Type>& arg) const
{
  return _Vector2<Type>(x-arg.x, y-arg.y);
}

template<class Type>
_Vector2<Type> _Vector2<Type>::operator * (const Type arg)  const
{
  return _Vector2<Type>(x*arg, y*arg);
}

template<class Type>
_Vector2<Type> _Vector2<Type>::operator / (const Type arg)  const
{
  return _Vector2<Type>(x/arg, y/arg);
}

template<class Type>
_Vector2<Type>& _Vector2<Type>::operator += (const _Vector2<Type>& arg)
{
  x += arg.x; y += arg.y;
  return *this;
}

template<class Type>
_Vector2<Type>& _Vector2<Type>::operator -= (const _Vector2<Type>& arg)
{
  x -= arg.x; y -= arg.y;
  return *this;
}

template<class Type>
_Vector2<Type>& _Vector2<Type>::operator *= (const Type arg)
{
  x *= arg; y *= arg;
  return *this;
}

template<class Type>
_Vector2<Type>& _Vector2<Type>::operator /= (const Type arg)
{
  x /= arg; y /= arg;
  return *this;
}

template<class Type>
template<class NewType>
_Vector2<Type>::operator _Vector2<NewType> ()
{
  return _Vector2<NewType>((NewType)x, (NewType)y);
}

template<class Type>
Type _Vector2<Type>::dot(const _Vector2<Type>& v1, const _Vector2<Type>& v2)
{
  return v1.x*v2.x + v1.y*v2.y;
}

template<class Type>
Type _Vector2<Type>::getLength() const
{
  return sqrt(x*x + y*y);
}

template<class Type>
Type _Vector2<Type>::getSquareLength() const
{
  return x*x + y*y;
}

template<class Type>
_Vector2<Type>& _Vector2<Type>::normalize()
{
  return *this /= getLength();
}

template<class Type>
_Vector2<Type> _Vector2<Type>::getNormalized() const
{
  return *this / getLength();
}

#endif
#endif
