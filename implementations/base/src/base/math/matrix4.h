#ifndef __BASE_MATRIX4_H__
#define __BASE_MATRIX4_H__
//////////////////////////////////////////////////////////////////////////////
//
//    MATRIX4.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      4x4 Matrix class

    DEPENDENCIES:
      math.h must be used

    TODO:
      + Possibly move the transformation operations to a LinearTransformation
        class inherriting from Matrix4, since it isn't "pure" (in the
        object-oriented sense: a matrix is a math class not a transformation)
*/

/*                                FUNCTIONS                                 */
template<class Type>
inline _Matrix4<Type> operator * (Type arg1, const _Matrix4<Type>& arg2);

/*                                CLASSES                                   */
template<class Type = float>
class _Matrix4
{
protected:
	typedef _Matrix4<Type> Matrix4;

#pragma pack(push,1)

  union
  {
    struct
    {
      union { struct { Type _00, _01, _02, _03; }; };
      union { struct { Type _10, _11, _12, _13; }; };
      union { struct { Type _20, _21, _22, _23; }; };
      union { struct { Type _30, _31, _32, _33; }; };
    };
    Type m[4][4];
  };

#pragma pack(pop)

public:
   // constructors
  inline _Matrix4() {}
  inline _Matrix4(const Type* f);
  inline _Matrix4(const _Matrix4& m);
  inline _Matrix4(Type _00, Type _01, Type _02, Type _03,
                  Type _10, Type _11, Type _12, Type _13,
                  Type _20, Type _21, Type _22, Type _23,
                  Type _30, Type _31, Type _32, Type _33);

  inline void set(Type _00, Type _01, Type _02, Type _03,
                  Type _10, Type _11, Type _12, Type _13,
                  Type _20, Type _21, Type _22, Type _23,
                  Type _30, Type _31, Type _32, Type _33);
  inline void set(const Type* f);

  // destructor
  inline ~_Matrix4() {}

  // access grants
  inline Type& operator() (uint row, uint col);
  inline Type  operator() (uint row, uint col) const;
  inline Type& operator() (uint n);
  inline Type  operator() (uint n) const;

  // type casts
  inline operator const Type* () const;

  // assignment operators
  inline _Matrix4& operator *= (const _Matrix4& m);
  // todo: inline _Matrix4& operator /= (const _Matrix4& m); // Matrix reduction i.e. reduce [ this | m ] ~ [ I | Result ], returning Result (Note I / Matrix will give the matrix inverse Matrix^-1)
  inline _Matrix4& operator += (const _Matrix4& m);
  inline _Matrix4& operator -= (const _Matrix4& m);
  inline _Matrix4& operator *= (Type arg);
  inline _Matrix4& operator /= (Type arg);

  // binary operators
  inline _Matrix4 operator * (const _Matrix4& m) const; //Matrix concatenation
  // todo: inline Matrix4 operator / (const Matrix4& m); // Matrix reduction i.e. reduce [ this | m ] ~ [ I | Result ], returning Result (Note I / Matrix will give the matrix inverse Matrix^-1)
  inline _Matrix4 operator + (const _Matrix4& m) const;
  inline _Matrix4 operator - (const _Matrix4& m) const;
  inline _Matrix4 operator * (Type arg) const;
  inline _Matrix4 operator / (Type arg) const;

#ifndef MSVC_BUILD
  template<Type>
#endif
  friend _Matrix4 operator * (Type arg, const _Matrix4& m);

  // matrix operations
  inline _Matrix4& inverse();
  inline _Matrix4& transpose();
  inline Type getDeterminant() const;

  // transformation operations
  inline _Matrix4& setIdentity();
  inline _Matrix4& setClear();
  inline _Matrix4& setView(const _Vector3<Type>& Eye, const _Vector3<Type>& LookAt, const _Vector3<Type>& Up); /* Up must be normalized */
  inline _Matrix4& setPerspectiveProjectionFOV(Type YFOV, Type AspectRatio, Type ZNearPlane, Type ZFarPlane);
  inline _Matrix4& setPerspectiveProjection(Type ViewWidth, Type ViewHeight, Type ZNearPlane, Type ZFarPlane);
  inline _Matrix4& setPerspectiveProjection(Type MinX, Type MaxX, Type MinY, Type MaxY, Type MinZ, Type MaxZ);
  inline _Matrix4& setOrthoProjection(Type ViewWidth, Type ViewHeight, Type ZNearPlane, Type ZFarPlane);
  inline _Matrix4& setOrthoProjection(Type MinX, Type MaxX, Type MinY, Type MaxY, Type MinZ, Type MaxZ);
  inline _Matrix4& setTranslation(Type x, Type y, Type z);
  inline _Matrix4& setTranslation(_Vector3<Type>& v);
  inline _Matrix4& setRotationX(Type angle);
  inline _Matrix4& setRotationY(Type angle);
  inline _Matrix4& setRotationZ(Type angle);
  inline _Matrix4& setAxisRotation(const _Vector3<Type>& Axis, Type angle);
  inline _Matrix4& setScaling(Type x, Type y, Type z);
};

typedef _Matrix4<float> Matrix4;
typedef _Matrix4<float> Matrix4f;
typedef _Matrix4<double> Matrix4d;

#endif
