#ifndef __BASE_HCOORD3_H__
#define __BASE_HCOORD3_H__
//////////////////////////////////////////////////////////////////////////////
//
//    HCOORD3.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      3-dimensional homogeneous coordinates (vector or point)

    USAGE:
      To use vector-specific operations, cast to (_HVector3&)

    DEPENDENCIES:
      math.h must be used
*/

/*                                FUNCTIONS                                 */
template<class Type>
inline _HCoord3<Type> operator * (const Type arg1, const _HCoord3<Type>& arg2);

/*                                CLASSES                                   */
template<class Type = float>
class _HCoord3
{
protected:

#pragma pack(push, 1)

  union
  {
    struct { Type x, y, z, w; };
    struct { Type _0,	_1,	_2, _3; };
    Type v[4];
  };

#pragma pack(pop)

public:
  // constructors
  inline _HCoord3() {}
  inline _HCoord3(const Type* f);
  inline _HCoord3(const _Vector3<Type>& xyz, Type w);
  inline _HCoord3(const _Vector3<Type>& v);
  inline _HCoord3(const _HCoord3& v);
  inline _HCoord3(Type x, Type y, Type z, Type w);

  // destructors
  inline ~_HCoord3();

  // access grants
  inline Type& operator () (uint n);
  inline Type  operator () (uint n) const;
  inline void get(Type& xOut, Type& yOut, Type& zOut, Type& wOut) const;
  inline void get(Type& xOut, Type& yOut, Type& zOut) const;

  // type casts & assignments
  inline operator Type* () const;

  // assignment operators
  inline _HCoord3& operator *= (const _Matrix3<Type>& m);
  inline _HCoord3& operator *= (const _Matrix4<Type>& m);
  inline _HCoord3& operator += (const _HVector3<Type>& arg);
  //??? inline _HCoord3& operator -= (const _HCoord3& arg);     // this is invalid for _HVector3 -= _HPoint3
  inline _HCoord3& operator -= (const _HVector3<Type>& arg);
  inline _HCoord3& operator *= (const Type arg);
  inline _HCoord3& operator /= (const Type arg);

  // unary operators
  inline _HCoord3 operator - () const; // technically this is invalid for _HPoint3, but for efficiency reasons this is implemented never-the-less (Usually _HPoint3 would have to be cast to _HVector3 and back)

  // binary operators
  inline _HCoord3 operator + (const _HVector3<Type>& arg) const;
  inline _HCoord3 operator - (const _HVector3<Type>& arg) const;
  inline _HCoord3 operator * (const Type arg)  const;
  template<Type> friend inline _HCoord3<Type> operator * (const Type arg1, const _HCoord3<Type>& arg2);
  inline _HCoord3 operator / (const Type arg)  const;

  // conversion operators
  template<class NewType> inline operator _HCoord3<NewType> ();
  inline operator _Vector3<Type> ();

  // coordinate operations
  inline static Type dot(const _HCoord3& v1, const _HCoord3& v2);
  inline static _HVector3<Type> cross(const _HCoord3& v1, const _HCoord3& v2);
  inline _HVector3<Type>& normalize() const;
  inline _HVector3<Type> getNormalized() const;
  inline Type getLength() const; // invalid for _HPoint3, but implemented for efficiency reasons
  inline _HCoord3& setTransform(const _Matrix3<Type>& m);
  inline _HCoord3& setTransform(const _Matrix4<Type>& m);
  inline _HCoord3 getTransform(const _Matrix3<Type>& m);
  inline _HCoord3 getTransform(const _Matrix4<Type>& m);
};

typedef _HCoord3<float> HCoord3;
typedef _HCoord3<float> HCoord3f;
typedef _HCoord3<double> HCoord3d;

#endif
