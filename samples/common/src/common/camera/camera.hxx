#ifdef __SAMPLES_CAMERA_H__
#ifndef __SAMPLES_CAMERA_HXX__
#define __SAMPLES_CAMERA_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    CAMERA.HXX
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
_Camera<Type>::_Camera(const _Vector3<Type> position, const _UnitQuaternion<Type>& orientation) : position(position), orientation(orientation)
{}

template<class Type>
_Camera<Type>::_Camera(const _Vector3<Type> position) : position(position)
{}

template<class Type>
_Camera<Type>::_Camera(const _UnitQuaternion<Type>& orientation) : orientation(orientation)
{}

template<class Type>
_Vector3<Type>& _Camera<Type>::getPosition()
{
  return position;
}

template<class Type>
const _Vector3<Type>& _Camera<Type>::getPosition() const
{
  return position;
}

template<class Type>
_UnitQuaternion<Type>& _Camera<Type>::getOrientation()
{
  return orientation;
}

template<class Type>
const _UnitQuaternion<Type>& _Camera<Type>::getOrientation() const
{
  return orientation;
}

/*template<class Type>
void _Camera<Type>::setTarget(const Vector3<Type>& target)
{
  //todo
}*/

template<class Type>
void _Camera<Type>::setPitchYaw(Type pitch, Type yaw)
{
  orientation  = _UnitQuaternion<Type>(_Vector3<Type>((Type)1.0, (Type)0.0, (Type)0.0), pitch);
  orientation *= _UnitQuaternion<Type>(_Vector3<Type>((Type)0.0, (Type)1.0, (Type)0.0), yaw);


  /* todo: improve this algorithm (by simply expanding the Quaternion axis angle), like this:
  Set yaw pitch roll

  Type halfPitch = ((Type)0.5)*pitch  
  Type halfYaw = ((Type)0.5)*yaw;
  Type halfRoll = ((Type)0.5)*roll;  
  Type cosHalfPitch = cos(halfPitch);
  Type sinHalfPitch =sin(halfPitch);
  Type cosHalfYaw = cos(halfYaw);
  Type sinHalfYaw = sin(halfYaw);
  Type cosHalfRoll = cos(halfRoll);
  Type sinHalfRoll = sin(halfRoll);
  orientation.set(cosHalfRoll * sinHalfPitch * cosHalfYaw + sinHalfRoll * cosHalfPitch * sinHalfYaw,
                  cosHalfRoll * cosHalfPitch * sinHalfYaw - sinHalfRoll * sinHalfPitch * cosHalfYaw,
                  sinHalfRoll * cosHalfPitch * cosHalfYaw - cosHalfRoll * sinHalfPitch * sinHalfYaw,
                  cosHalfRoll * cosHalfPitch * cosHalfYaw + sinHalfRoll * sinHalfPitch * sinHalfYaw);*/
}



#endif
#endif
