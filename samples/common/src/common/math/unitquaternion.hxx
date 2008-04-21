#ifdef __SAMPLES_UNITQUATERNION_H__
#ifndef __SAMPLES_UNITQUATERNION_HXX__
#define __SAMPLES_UNITQUATERNION_HXX__
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
  set((const Type*)q);
}

template<class Type>
_UnitQuaternion<Type>::_UnitQuaternion(const Type* q)
{
  set(q);
}

template<class Type>
_UnitQuaternion<Type>::_UnitQuaternion(const _Vector3<Type>& axis, Type angle)
{
  set(axis, angle);
}

template<class Type>
_UnitQuaternion<Type>::_UnitQuaternion(Type x, Type y, Type z, Type w)
{
  set(x, y, z, w);
}

template<class Type>
void _UnitQuaternion<Type>::set(const Type* q)
{
  UnitQuaternion::x = q[0];
  UnitQuaternion::y = q[1];
  UnitQuaternion::z = q[2];
  UnitQuaternion::w = q[3];
}

template<class Type>
void _UnitQuaternion<Type>::set(const _Vector3<Type>& axis, Type angle)
{
  Type halfAngle = ((Type)0.5)*angle;
  Type sinHalfAngle = sin(halfAngle);  

 	UnitQuaternion::x = axis(0) * sinHalfAngle;
	UnitQuaternion::y = axis(1) * sinHalfAngle;
	UnitQuaternion::z = axis(2) * sinHalfAngle;
  UnitQuaternion::w = cos(halfAngle);
}

template<class Type>
void _UnitQuaternion<Type>::set(Type x, Type y, Type z, Type w)
{
  UnitQuaternion::x = x;
  UnitQuaternion::y = y;
  UnitQuaternion::z = z;
  UnitQuaternion::w = w;
}

//template<class Type>
//_UnitQuaternion<Type>::operator _Matrix4<Type> () const
//{
//	return getTransformation();
//}

template<class Type>
_UnitQuaternion<Type> _UnitQuaternion<Type>::operator * (const _UnitQuaternion<Type>& arg) const
{
	const Type& x = UnitQuaternion::x;
	const Type& y = UnitQuaternion::y;
	const Type& z = UnitQuaternion::z;
	const Type& w = UnitQuaternion::w;
  
  return _UnitQuaternion<Type>(w*arg.x + x*arg.w + y*arg.z - z*arg.y,
	                             w*arg.y + y*arg.w + z*arg.x - x*arg.z,
	                             w*arg.z + z*arg.w + x*arg.y - y*arg.x,
                               w*arg.w - x*arg.x - y*arg.y - z*arg.z);
}

template<class Type>
_UnitQuaternion<Type>& _UnitQuaternion<Type>::operator *= (const _UnitQuaternion<Type>& arg)
{
  return *this = *this * arg;  
}


template<class Type>
Type _UnitQuaternion<Type>::operator () (uint n) const
{
  return UnitQuaternion::q[n];
}

/*template<class Type>
Type _UnitQuaternion<Type>::operator () (CoordinateIndex n) const
{
  return q[n];
}*/

template<class Type>
const _Vector3<Type>& _UnitQuaternion<Type>::getAARotationAxis() const
{
  // note: This will return a null vector if no rotation is defined (i.e. the angle of the axis-angle rotation is 0 and thus the unit quaternion is an identity)
  return (const _Vector3<Type>&)(Type*)UnitQuaternion::q;
}

template<class Type>
Type _UnitQuaternion<Type>::getAARotationAngle() const
{
  return 2*acos(UnitQuaternion::w);
}

template<class Type>
void _UnitQuaternion<Type>::getTransformation(_Matrix4<Type>& transformation) const
{
	const Type& x = UnitQuaternion::x;
	const Type& y = UnitQuaternion::y;
	const Type& z = UnitQuaternion::z;
	const Type& w = UnitQuaternion::w;

  transformation.set(1 - 2*(y*y+z*z), 2*(x*y + z*w),     2*(x*z - y*w),	    0,
                     2*(x*y - z*w),   1 - 2*(x*x + z*z), 2*(z*y + x*w),     0,
                     2*(x*z + y*w),   2*(y*z - x*w),     1 - 2*(x*x + y*y), 0,
	                   0,               0,                 0,                 1);
}

template<class Type>
void _UnitQuaternion<Type>::getTransformation(_Matrix3<Type>& transformation) const
{
	const Type& x = UnitQuaternion::x;
	const Type& y = UnitQuaternion::y;
	const Type& z = UnitQuaternion::z;
	const Type& w = UnitQuaternion::w;

  transformation.set(1 - 2*(y*y+z*z), 2*(x*y + z*w),     2*(x*z - y*w),
                     2*(x*y - z*w),   1 - 2*(x*x + z*z), 2*(z*y + x*w),
                     2*(x*z + y*w),   2*(y*z - x*w),     1 - 2*(x*x + y*y));
}

template<class Type>
void _UnitQuaternion<Type>::getXRotationAxis(_Vector3<Type>& axis) const
{
  axis.set(1 - 2*(y*y+z*z), 2*(x*y - z*w), 2*(x*z + y*w)); // 1st column of the transformation matrix
}

template<class Type>
void _UnitQuaternion<Type>::getYRotationAxis(_Vector3<Type>& axis) const
{
  axis.set(2*(x*y + z*w), 1 - 2*(x*x + z*z), 2*(y*z - x*w)); // 2nd column of the transformation matrix
}

template<class Type>
void _UnitQuaternion<Type>::getZRotationAxis(_Vector3<Type>& axis) const
{
  axis.set(2*(x*z - y*w), 2*(z*y + x*w), 1 - 2*(x*x + y*y)); // 3rd column of the transformation matrix
}

template<class Type>
void _UnitQuaternion<Type>::setTransformation(const _Matrix3<Type>& transformation)
{
  const Type trace = transformation(0,0) + transformation(1,1) + transformation(2,2);  

  //// Quick root
  if(trace >= (Type)0.0)
  {
    Type s = sqrt(trace + (Type)1.0);
    w = ((Type)0.5) * s;
    s = ((Type)0.5) / s;
    x = (transformation(2,1) - transformation(1,2)) * s;
    y = (transformation(0,2) - transformation(2,0)) * s;
    z = (transformation(1,0) - transformation(0,1)) * s;
    return;
  }

  //// Get the the biggest diagonal element
  if(transformation(0,0) > transformation(1,1) && transformation(0,0) > transformation(2,2))
  {
    // X element
    Type s = sqrt((Type)1.0 + transformation(0,0) - transformation(1,1) - transformation(2,2));
    x = ((Type)0.5) * s;
    s = ((Type)0.5) / s;
    w = (transformation(2,1) - transformation(1,2)) * s;
    y = (transformation(0,1) + transformation(1,0)) * s;
    z = (transformation(2,0) + transformation(0,2)) * s;
    return;
  }
  
  if(transformation(1,1) > transformation(2,2))
  {
    // Y element
    Type s = sqrt((Type)1.0 + transformation(1,1) - transformation(0,0) - transformation(2,2));
    y = ((Type)0.5) * s;
    s = ((Type)0.5) / s;
    w = (transformation(0,2) - transformation(2,0)) * s;
    x = (transformation(0,1) + transformation(1,0)) * s;
    z = (transformation(1,2) + transformation(2,1)) * s;
  }
  else
  {
    // Z element
    Type s = sqrt((Type)1.0 + transformation(2,2) - transformation(0,0) - transformation(1,1));
    z = ((Type)0.5) * s;
    s = ((Type)0.5) / s;
    w = (transformation(1,0) - transformation(0,1)) * s;
    x = (transformation(2,0) + transformation(0,2)) * s;
    y = (transformation(1,2) + transformation(2,1)) * s;
  }
}
  
template <class Type>
void _UnitQuaternion<Type>::setRotationAxes(const _Vector3<Type>& xAxis, const _Vector3<Type>& yAxis, const _Vector3<Type>& zAxis)
{
  const _Matrix3<Type> rotation(xAxis, yAxis, zAxis);
  setTransformation(rotation);
}

#endif
#endif
