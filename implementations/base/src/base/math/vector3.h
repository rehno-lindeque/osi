#ifndef __BASE_VECTOR3_H__
#define __BASE_VECTOR3_H__
//////////////////////////////////////////////////////////////////////////////
//
//    VECTOR3.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      3-Dimensional vector class

    DEPENDENCIES:
      math.h must be used
*/

/*                                FUNCTIONS                                 */
template<class Type>
inline _Vector3<Type> operator * (Type arg1, const _Vector3<Type>& arg2);

/*                                 CLASSES                                  */
template<class Type = float>
class _Vector3
{
protected:
	typedef _Vector3<Type> Vector3;

#pragma pack(push, 1)

  union
  {
    struct { Type x, y, z; };
    struct { Type _0,	_1,	_2; };
    Type v[3];
  };

#pragma pack(pop)

public:
  // constructors
  inline _Vector3() {}
  inline _Vector3(const Type* f);
  inline _Vector3(const _Vector3& v);
  inline _Vector3(Type x, Type y, Type z);

  inline void set(Type x, Type y, Type z);
  inline void set(const Type* f);

  // destructor
  inline ~_Vector3();

  // access grants
  inline Type& operator () (uint n);
  inline Type  operator () (uint n) const;
  inline void get(Type& xOut, Type& yOut, Type& zOut);

  // type casts & assignments
  inline operator Type* () const;

  // assignment operators
  inline _Vector3& operator *= (const _Matrix3<Type>& m);
  inline _Vector3& operator *= (const _Matrix4<Type>& m);
  inline _Vector3& operator += (const _Vector3& arg);
  inline _Vector3& operator -= (const _Vector3& arg);
  inline _Vector3& operator *= (const Type arg);
  inline _Vector3& operator /= (const Type arg);

  // unary operators
  inline _Vector3 operator - () const;

  // binary operators
  inline _Vector3 operator + (const _Vector3& arg) const;
  inline _Vector3 operator - (const _Vector3& arg) const;
  inline _Vector3 operator * (const Type arg)  const;
  inline _Vector3 operator / (const Type arg)  const;

/*#ifndef MSVC_BUILD
  template<Type>
#endif
  friend inline _Vector3 operator* (Type arg1, const _Vector3& arg2);*/

  // conversion operators
  template<class NewType> inline operator _Vector3<NewType> ();

  // vector operations
  inline static Type dot(const _Vector3& v1, const _Vector3& v2);
  inline static _Vector3 cross(const _Vector3& v1, const _Vector3& v2);
  inline Type getLength() const;
  inline Type getSquareLength() const;
  inline _Vector3& normalize();
  inline _Vector3 getNormalized() const;
  inline _Vector3& setTransform(const _Matrix3<Type>& arg);
  inline _Vector3& setTransform(const _Matrix4<Type>& arg);
  inline _Vector3 getTransform(const _Matrix3<Type>& arg);
  inline _Vector3 getTransform(const _Matrix4<Type>& arg);

  //todo: inline _VectorT3& transpose() { return *(VectorT3*)this; }
};


template<class Type = float>
class _VectorT3 : public _Vector3<Type>
{
};

typedef _Vector3<float> Vector3;
typedef _Vector3<float> Vector3f;
typedef _Vector3<double> Vector3d;
typedef _VectorT3<float> VectorT3;
typedef _VectorT3<float> VectorT3f;
typedef _VectorT3<double> VectorT3d;

#endif
