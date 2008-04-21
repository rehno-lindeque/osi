#ifndef __SAMPLES_VECTOR4_H__
#define __SAMPLES_VECTOR4_H__
//////////////////////////////////////////////////////////////////////////////
//
//    VECTOR4.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      4-Dimensional vector class
    
    DEPENDENCIES:
      math.h must be used
*/

/*                                FUNCTIONS                                 */
template<class Type>
inline _Vector4<Type> operator * (const Type arg1, const _Vector4<Type>& arg2);

/*                                CLASSES                                   */
template<class Type = float>
class _Vector4
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
  inline _Vector4() {}
  inline _Vector4(const Type* f);
  inline _Vector4(const _Vector4& v);
  inline _Vector4(Type x, Type y, Type z, Type w);

  // destructor
  inline ~_Vector4();

  // access grants
  inline Type& operator () (uint n);
  inline Type  operator () (uint n) const;
  inline void get(Type& xOut, Type& yOut, Type& zOut, Type& wOut) const;
  
  // type casts & assignments
  inline operator Type* () const;

  //assignment operators
  inline _Vector4& operator *= (const _Matrix4<Type>& m);    
  inline _Vector4& operator += (const _Vector4& arg);
  inline _Vector4& operator -= (const _Vector4& arg);
  inline _Vector4& operator *= (const Type arg);
  inline _Vector4& operator /= (const Type arg);
  
  // unary operators
  inline _Vector4 operator - () const;

  // binary operators
  inline _Vector4 operator + (const _Vector4& arg) const;
  inline _Vector4 operator - (const _Vector4& arg) const;
  inline _Vector4 operator * (const Type arg)  const;
  template<Type> friend inline _Vector4<Type> operator * (const Type arg1, const _Vector4<Type>& arg2);
  inline _Vector4 operator / (const Type arg)  const;
  
  // conversion operators
  template<class NewType> inline operator _Vector4<NewType> ();

  // vector operations
  inline static Type dot(const _Vector4& v1, const _Vector4& v2); 
  inline Type getLength() const;
  inline Type getSquareLength() const;
  inline _Vector4& normalize();
  inline _Vector4 getNormalized() const;
  inline _Vector4& setTransform(const _Matrix4<Type>& arg);
  inline _Vector4 getTransform(const _Matrix4<Type>& arg);
};

typedef _Vector4<float> Vector4;
typedef _Vector4<float> Vector4f;
typedef _Vector4<double> Vector4d;

#endif
