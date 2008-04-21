#ifndef __BASEGE_HPOINT3_H__
#define __BASEGE_HPOINT3_H__
//////////////////////////////////////////////////////////////////////////////
//
//    HPOINT3.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      3-dimensional point in homogeneous coordinates
    
    DEPENDENCIES:
      math.h must be used
*/

/*                                CLASSES                                   */
template<class Type = float>
class _HPoint3 : public _HCoord3<Type>
{
protected:
	typedef _HPoint3<Type> HPoint3;
public:
  // constructors
  inline _HPoint3() {}
  inline _HPoint3(const Type* f);
  inline _HPoint3(const _Vector3<Type>& xyz);  
  inline _HPoint3(const _HCoord3<Type>& v);
  inline _HPoint3(Type x, Type y, Type z);

  // destructors
  inline ~_HPoint3();

  // binary operators
  inline _HVector3<Type> operator - (const _HPoint3& arg) const;

  // assignment operators (these are slightly optimized for _HPoint3)
  inline _HPoint3& operator *= (const _Matrix4<Type>& m);
  
  // conversion operators
  template<class NewType> inline operator _HPoint3<NewType> ();

  // point operations
  inline _HPoint3& setTransform(const _Matrix4<Type>& m);
  inline _HPoint3 getTransform(const _Matrix4<Type>& m); // (optimized for _HPoint3)
};

typedef _HPoint3<float> HPoint3;
typedef _HPoint3<float> HPoint3f;
typedef _HPoint3<double> HPoint3d;

#endif
