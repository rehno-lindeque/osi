#ifndef __BASE_HVECTOR3_H__
#define __BASE_HVECTOR3_H__
//////////////////////////////////////////////////////////////////////////////
//
//    HVECTOR3.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      3-dimensional vector in homogeneous coordinates

    DEPENDENCIES:
      math.h must be used
*/

/*                                FUNCTIONS                                 */
template<class Type>
FORCE_INLINE _HVector3<Type> operator * (const Type arg1, const _HVector3<Type>& arg2);

/*                                CLASSES                                   */
template<class Type = float>
class _HVector3 : public _HCoord3<Type>
{
protected:
	typedef _HVector3<Type> HVector3;
public:
  // constructors
  FORCE_INLINE _HVector3() {}
  FORCE_INLINE _HVector3(const Type* f);
  FORCE_INLINE _HVector3(const _Vector3<Type>& xyz);
  FORCE_INLINE _HVector3(const _HCoord3<Type>& v);
  FORCE_INLINE _HVector3(Type x, Type y, Type z);

  // destructors
  FORCE_INLINE ~_HVector3();

  // assignment operators (these are slightly optimized for _HVector3)
  FORCE_INLINE _HVector3& operator *= (const _Matrix4<Type>& m);

  // unary operators
  //inline _HVector3 operator - () const; (implemented in HCoord3 for efficiency reasons)

  // conversion operators
  template<class NewType> inline operator _HVector3<NewType> ();

  // vector operations
  FORCE_INLINE _HVector3& setTransform(const _Matrix4<Type>& m);
  FORCE_INLINE _HVector3 getTransform(const _Matrix4<Type>& m); // (optimized for _HVector3)
  FORCE_INLINE _HVector3& normalize();
};

typedef _HVector3<float> HVector3;
typedef _HVector3<float> HVector3f;
typedef _HVector3<double> HVector3d;

#endif
