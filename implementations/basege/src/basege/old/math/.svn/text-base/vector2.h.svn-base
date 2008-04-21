#ifndef __BASERE_VECTOR2_H__
#define __BASERE_VECTOR2_H__
//////////////////////////////////////////////////////////////////////////////
//
//    VECTOR2.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      2-Dimensional vector class
    
    DEPENDENCIES:
      math.h must be used
*/

/*                                FUNCTIONS                                 */
template<class Type> 
inline _Vector2<Type> operator * (const Type arg1, const _Vector2<Type>& arg2);

/*                                 CLASSES                                  */
template<class Type = float>
class _Vector2
{
protected:
  typedef _Vector2<Type> Vector2;
	
#pragma pack(push, 1)

  union
  {     
    struct { Type x, y; };
    struct { Type _0,	_1; };
    Type v[2];
  };

#pragma pack(pop)

public:
  // constructors    
  inline _Vector2() {}
  inline _Vector2(const Type* f);
  inline _Vector2(const _Vector2& v);
  inline _Vector2(Type x, Type y);  

  // destructor
  inline ~_Vector2();

  //access grants
  inline Type& operator () (uint n);
  inline Type  operator () (uint n) const;
  inline void get(Type& xOut, Type& yOut) const;

  // type casts & assignments
  inline operator Type* () const;

  // assignment operators
  inline _Vector2& operator += (const _Vector2& arg);
  inline _Vector2& operator -= (const _Vector2& arg);
  inline _Vector2& operator *= (const Type arg);
  inline _Vector2& operator /= (const Type arg);

  // unary operators
  inline _Vector2 operator - () const;

  // binary operators
  inline _Vector2 operator + (const _Vector2& arg) const;
  inline _Vector2 operator - (const _Vector2& arg) const;
  inline _Vector2 operator * (const Type arg)  const;
  inline _Vector2 operator / (const Type arg)  const;

#ifndef MSVC_BUILD
  template<Type>
#endif
  friend inline _Vector2 operator* (const Type arg1, const _Vector2& arg2);

  // conversion operators
  template<class NewType> inline operator _Vector2<NewType> ();

  // vector operations
  inline static Type dot(const _Vector2& v1, const _Vector2& v2);   
  inline Type getLength() const;
  inline Type getSquareLength() const;
  inline _Vector2& normalize();
  inline _Vector2 getNormalized() const;
};

typedef _Vector2<float> Vector2;
typedef _Vector2<float> Vector2f;
typedef _Vector2<double> Vector2d;

#endif
