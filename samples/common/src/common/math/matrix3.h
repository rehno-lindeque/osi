#ifndef __SAMPLES_MATRIX3_H__
#define __SAMPLES_MATRIX3_H__
//////////////////////////////////////////////////////////////////////////////
//
//    MATRIX3.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      3x3 Matrix class
    
    DEPENDENCIES:
      math.h must be used
*/

/*                                FUNCTIONS                                 */
template<class Type>
inline _Matrix3<Type> operator * (Type arg1, const _Matrix3<Type>& arg2);

/*                                CLASSES                                   */
template<class Type = float>
class _Matrix3
{
protected:
	typedef _Matrix3<Type> Matrix3;

#pragma pack(push,1)

  union
  {
    struct
    {
      union { struct { Type _00, _01, _02; }; }; 
      union { struct { Type _10, _11, _12; }; };
      union { struct { Type _20, _21, _22; }; };
    };
    Type m[3][3];
  };

#pragma pack(pop)

public:
   // constructors
  inline _Matrix3() {}
  inline _Matrix3(const Type* f);
  inline _Matrix3(const _Matrix3& m);
  inline _Matrix3(Type _00, Type _01, Type _02,
                  Type _10, Type _11, Type _12,
                  Type _20, Type _21, Type _22);
  inline _Matrix3(const _Vector3<Type>& col1, const _Vector3<Type>& col2, const _Vector3<Type>& col3);

  inline void set(Type _00, Type _01, Type _02,
                  Type _10, Type _11, Type _12,
                  Type _20, Type _21, Type _22);
  inline void set(const Type* f);
  inline void set(const _Vector3<Type>& col1, const _Vector3<Type>& col2, const _Vector3<Type>& col3);
  
  // destructor
  inline ~_Matrix3() {}

  // access grants
  inline Type& operator() (uint row, uint col);
  inline Type  operator() (uint row, uint col) const;
  inline Type& operator() (uint n);
  inline Type  operator() (uint n) const;

  // type casts
  inline operator const Type* () const;

  // assignment operators
  // todo (See Matrix4)

  // binary operators
  // todo (See Matrix4)

  // matrix operations
  // todo (See Matrix4)
  
  // transformation operations
  // todo (See Matrix4)
};

typedef _Matrix3<float> Matrix3;
typedef _Matrix3<float> Matrix3f;
typedef _Matrix3<double> Matrix3d;

#endif
