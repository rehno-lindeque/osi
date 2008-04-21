#ifndef __QRAYRE_CAMERA_H__
#define __QRAYRE_CAMERA_H__
//////////////////////////////////////////////////////////////////////////////
//
//    CAMERA.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      QRayRE camera class
*/

/*                                 CLASSES                                  */
namespace QRayRE
{
  class QRayCamera : public BaseRE::Camera
  {
  public:
    Vector3f position;
    UnitQuaternionf direction;

    inline QRayCamera() : position(0.0f, 0.0f, 0.0f), direction(0.0f, 0.0f, 0.0f, 1.0f)
    {}

    inline void getCameraAxis(Vector3f& dx, Vector3f& dy, Vector3f& dz)
    {
      // get the camera space axes
      direction.getXRotationAxis(dx);
      direction.getYRotationAxis(dy);
      direction.getZRotationAxis(dz);
    }

    virtual ~QRayCamera()
    {}
  };
};

#endif
