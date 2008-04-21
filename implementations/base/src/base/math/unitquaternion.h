#ifndef __BASE_UNITQUATERNION_H__
#define __BASE_UNITQUATERNION_H__
//////////////////////////////////////////////////////////////////////////////
//
//    UNITQUATERNION.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Unit quaternion class.

    USAGE:
      Axis-angle rotations are assumed to be right-handed.

    TODO:
      + See "Uniform Random Rotations", Ken Shoemake, Graphics Gems III,
        pg. 124-132 for an algorithm for generating random quaternion
        rotations
        http://www.acm.org/pubs/tog/GraphicsGems/gemsiii/urot.c
        http://www.acm.org/pubs/tog/GraphicsGems/gems.html#gemsiii


    ALGORITHMS:
      + Construction of a quaternion from a rotation matrix
        "Quaternion Calculus and Fast Animation", Ken Schoemake, 1987 Siggraph course notes

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
  // constructors
  inline _UnitQuaternion() {}
  inline _UnitQuaternion(const _UnitQuaternion& q);
  inline _UnitQuaternion(const Type* q);
  inline _UnitQuaternion(const _Vector3<Type>& axis, Type angle); // axis must be a unit vector
  inline _UnitQuaternion(Type x, Type y, Type z, Type w);

  inline void set(const Type* q);
  inline void set(const _Vector3<Type>& axis, Type angle);
  inline void set(Type x, Type y, Type z, Type w);

  // type casts
  //inline operator _Matrix4<Type> () const;
  inline operator const Type*() const { return (Type*)_UnitQuaternion::q; }

  // unary operators
  inline _UnitQuaternion operator - () const { return _UnitQuaternion(_UnitQuaternion::x, _UnitQuaternion::y, _UnitQuaternion::z, -_UnitQuaternion::w); } // todo: don't know if this is mathematically correct, but it's usefull

  // binary operators
  inline _UnitQuaternion operator * (const _UnitQuaternion& arg) const;

  //assignment operators
  inline _UnitQuaternion& operator *= (const _UnitQuaternion& arg);

  // accessors
  inline Type operator () (uint n) const;
  //inline Type operator () (CoordinateIndex n) const;

  // quaternion operations
  // get axis-angle rotations
  inline const _Vector3<Type>& getAARotationAxis() const;
  inline Type getAARotationAngle() const;

  // get transformation
  inline void getTransformation(_Matrix4<Type>& transformation) const;
  inline void getTransformation(_Matrix3<Type>& transformation) const;
  inline void getXRotationAxis(_Vector3<Type>& axis) const;
  inline void getYRotationAxis(_Vector3<Type>& axis) const;
  inline void getZRotationAxis(_Vector3<Type>& axis) const;

  //todo: inline void setTransformation(_Matrix4<Type>& transformation);
  inline void setTransformation(const _Matrix3<Type>& transformation);
  inline void setRotationAxes(const _Vector3<Type>& xAxis, const _Vector3<Type>& yAxis, const _Vector3<Type>& zAxis);
};

typedef _UnitQuaternion<float> UnitQuaternion;
typedef _UnitQuaternion<float> UnitQuaternionf;

#endif
