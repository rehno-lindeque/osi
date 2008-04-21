#ifdef __BASEGE_UNITQUATERNION_H__
#ifndef __BASEGE_UNITQUATERNION_HXX__
#define __BASEGE_UNITQUATERNION_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    UNITQUATERNION.HXX
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
_UnitQuaternion<Type>::_UnitQuaternion(const _UnitQuaternion<Type>& q)
{
  UnitQuaternion::x = q.x;
  UnitQuaternion::y = q.y;
  UnitQuaternion::z = q.z;
  UnitQuaternion::w = q.w;
}

template<class Type>
_UnitQuaternion<Type>::_UnitQuaternion(const Type* q)
{
  UnitQuaternion::x = q[0];
  UnitQuaternion::y = q[1];
  UnitQuaternion::z = q[2];
  UnitQuaternion::w = q[3];
}

template<class Type>
_UnitQuaternion<Type>::_UnitQuaternion(const _Vector3<Type>& axis, Type angle)
{
  Type SIN_DIV_2 = sin( angle / 2 );  

 	UnitQuaternion::x = axis(0) * SIN_DIV_2;
	UnitQuaternion::y = axis(1) * SIN_DIV_2;
	UnitQuaternion::z = axis(2) * SIN_DIV_2;
  UnitQuaternion::w = cos( angle / 2 );
}

template<class Type>
_UnitQuaternion<Type>::_UnitQuaternion(Type x, Type y, Type z, Type w)
{
  UnitQuaternion::x = x;
  UnitQuaternion::y = y;
  UnitQuaternion::z = z;
  UnitQuaternion::w = w;
}

template<class Type>
const _Vector3<Type>& _UnitQuaternion<Type>::getAARotationAxis() const
{
  return (const _Vector3<Type>&)(Type*)UnitQuaternion::Q;
}

template<class Type>
Type _UnitQuaternion<Type>::getAARotationAngle() const
{
  return 2*acos(UnitQuaternion::w);
}

template<class Type>
inline _Matrix4<Type> _UnitQuaternion<Type>::getTransformation() const
{
	const Type& x = UnitQuaternion::x;
	const Type& y = UnitQuaternion::y;
	const Type& z = UnitQuaternion::z;
	const Type& w = UnitQuaternion::w;
  _Matrix4<Type> ret( 1 - 2*(y*y+z*z), 2*(x*y + z*w),     2*(x*z - y*w),	    0,
                      2*(x*y - z*w),   1 - 2*(x*x + z*z), 2*(z*y + x*w),     0,
                      2*(x*z + y*w),   2*(y*z - x*w),     1 - 2*(x*x + y*y), 0,
	                    0,               0,                 0,                 1 );
  return ret;
}

//template<class Type>
//_UnitQuaternion<Type>::operator _Matrix4<Type> () const
//{
//	return getTransformation();
//}

template<class Type>
_UnitQuaternion<Type> _UnitQuaternion<Type>::operator * (const _UnitQuaternion<Type>& arg) const
{
	Type& x = UnitQuaternion::x;
	Type& y = UnitQuaternion::y;
	Type& z = UnitQuaternion::z;
	Type& w = UnitQuaternion::w;
  //todo: check correctness
  return _UnitQuaternion<Type>( w*arg.x + x*arg.w + y*arg.z - z*arg.y,
	                              w*arg.y + y*arg.w + z*arg.x - x*arg.z,
	                              w*arg.z + z*arg.w + x*arg.y - y*arg.x,
                                w*arg.w - x*arg.x - y*arg.y - z*arg.z );
}

template<class Type>
_UnitQuaternion<Type>& _UnitQuaternion<Type>::operator *= (const _UnitQuaternion<Type>& arg)
{
  return *this = *this * arg;  
}


template<class Type>
Type _UnitQuaternion<Type>::operator () (uint n) const
{
  return UnitQuaternion::Q[n];
}

/*template<class Type>
Type _UnitQuaternion<Type>::operator () (CoordinateIndex n) const
{
  return Q[n];
}*/

#endif
#endif
