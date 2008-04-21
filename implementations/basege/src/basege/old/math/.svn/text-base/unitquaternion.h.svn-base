#ifndef __BASEGE_UNITQUATERNION_H__
#define __BASEGE_UNITQUATERNION_H__
//////////////////////////////////////////////////////////////////////////////
//
//    UNITQUATERNION.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      Unit quaternion class
    
    DEPENDENCIES:
      Quaternion base class
      math.h must be used
*/

/*                                CLASSES                                   */
template<class Type = float>
class _UnitQuaternion : protected _Quaternion<Type>
{
protected:
	typedef _UnitQuaternion<Type> UnitQuaternion;
public:
  inline _UnitQuaternion() {}
  inline _UnitQuaternion(const _UnitQuaternion& q);
  inline _UnitQuaternion(const Type* q);
  inline _UnitQuaternion(const _Vector3<Type>& axis, Type angle); // axis must be a unit vector
  inline _UnitQuaternion(Type x, Type y, Type z, Type w);

  // get axis-angle rotations
  inline const _Vector3<Type>& getAARotationAxis() const;
  inline Type getAARotationAngle() const;
  
  // get transformation matrix
  inline _Matrix4<Type> getTransformation() const;

  // type casts
  //inline operator _Matrix4<Type> () const;
  inline operator const Type*() const { return (Type*)UnitQuaternion::Q; }

  // unary operators
  inline _UnitQuaternion operator - () const { return _UnitQuaternion(UnitQuaternion::x, UnitQuaternion::y, UnitQuaternion::z, -UnitQuaternion::w); } // todo: don't know if this is mathematically correct, but it's usefull

  // binary operators  
  inline _UnitQuaternion operator * (const _UnitQuaternion& arg) const;

  //assignment operators  
  inline _UnitQuaternion& operator *= (const _UnitQuaternion& arg);

  // accessors  
  inline Type operator () (uint n) const;
  //inline Type operator () (CoordinateIndex n) const;
};

typedef _UnitQuaternion<float> UnitQuaternion;
typedef _UnitQuaternion<float> UnitQuaternionf;

#endif
