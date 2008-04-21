#ifndef __SAMPLES_MATH_H__
#define __SAMPLES_MATH_H__
//////////////////////////////////////////////////////////////////////////////
//
//    MATH.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Common maths functions for OpenRE backends.

    DEPENDENCIES:
      types.h must be included before this header

    TODO:
      + Create optimized implementations for specific data types
        e.g.
        #include "vector3f.hxx"
        #include "hvector3f.hxx"
        #include "hpoint3f.hxx"
*/
/*                              COMPILER MACROS                             */
#ifndef __SAMPLES_TYPES_H__
  #pragma error("types.h must be included before math.h")
#endif

/*                                INCLUDES                                  */
#include <math.h>

/*                                CONSTANTS                                 */
#define PI 3.14159265358979323846

const float IDENTITY_MATRIX4[] = { 1.0f, 0.0f, 0.0f, 0.0f,
                                   0.0f, 1.0f, 0.0f, 0.0f,
                                   0.0f, 0.0f, 1.0f, 0.0f,
                                   0.0f, 0.0f, 0.0f, 1.0f };

/*                            FORWARD DECLERATIONS                          */
template<class Type> class _Vector2;
template<class Type> class _Vector3;
template<class Type> class _HCoord3;
template<class Type> class _HVector3;
template<class Type> class _HPoint3;
template<class Type> class _Vector4;
template<class Type> class _Quaternion;
template<class Type> class _UnitQuaternion;
template<class Type> class _Matrix3;
template<class Type> class _Matrix4;
template<class Type> class _AxisAlignedBoundingBox;

/*                                CLASSES                                   */
#include "vector2.h"
#include "vector3.h"
#include "hcoord3.h"
#include "hvector3.h"
#include "hpoint3.h"
#include "vector4.h"
#include "quaternion.h"
#include "unitquaternion.h"
#include "matrix3.h"
#include "matrix4.h"
#include "axisalignedboundingbox.h"

/*                            IMPLEMENTATIONS                               */
#include "vector2.hxx"
#include "vector3.hxx"
#include "hcoord3.hxx"
#include "hvector3.hxx"
#include "hpoint3.hxx"
#include "vector4.hxx"
#include "quaternion.hxx"
#include "unitquaternion.hxx"
#include "matrix3.hxx"
#include "matrix4.hxx"
#include "axisalignedboundingbox.hxx"

#endif
