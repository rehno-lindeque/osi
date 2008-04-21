#ifndef __SAMPLES_CAMERA_H__
#define __SAMPLES_CAMERA_H__
//////////////////////////////////////////////////////////////////////////////
//
//    CAMERA.H
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      Quaternion-based camera class

    USAGE:
      All angles are in radians

    DEPENDENCIES:
      math.h must be used

    TODO:
      + Possible optimizations?
*/

/*                                CLASSES                                   */
template<class Type = float>
class _Camera
{
protected:
  /*typedef _Camera<Type> Camera;
  typedef _UnitQuaternion<Type> UnitQuaternion;
  typedef _Vector3<Type> Vector3;*/

  UnitQuaternion orientation;
  Vector3 position;

public:
  // constructors
  inline _Camera() {}
  inline _Camera(const _Vector3<Type> position, const _UnitQuaternion<Type>& orientation);
  inline _Camera(const _Vector3<Type> position);
  inline _Camera(const _UnitQuaternion<Type>& orientation);

  // accessors
  inline _Vector3<Type>& getPosition();
  inline const _Vector3<Type>& getPosition() const;
  inline _UnitQuaternion<Type>& getOrientation();
  inline const _UnitQuaternion<Type>& getOrientation() const;
  
  // transformations
  //todo: inline void setTarget(const Vector3<Type>& target);
  inline void setPitchYaw(Type pitch, Type yaw);
};

typedef _Camera<float> Camera;
typedef _Camera<float> Cameraf;

#include "camera.hxx"

#endif
