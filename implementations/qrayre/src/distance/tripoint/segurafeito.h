#ifndef __QRAYRE_DISTANCE_TRIPOINT_SEGURAFEITO_H__
#define __QRAYRE_DISTANCE_TRIPOINT_SEGURAFEITO_H__
//////////////////////////////////////////////////////////////////////////////
//
//    DISTANCE.H
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Find the closest distance between a point and a triangle.

    REFERENCES:
      + Algorithm by Rehno Lindeque, 06 Jan 2007 - 13 Jan 2007, but it is by
        and large based on Segura's ray-triangle intersection algorithm.
        See bellow.

      + http://wscg.zcu.cz/wscg2001/Papers_2001/R75.pdf
        author = "Rafael J. Segura1, Francisco R. Feito"
        title = "Algorithms To Test Ray-Triangle Intersection. Comparative Study"
        conference = "International Conferences in Central Europe on Computer Graphics, Visualization and Computer Vision (recently Winter School of Computer Graphics) (WSCG)"
        year = "2001"

      + Segura, R.J., Feito, F.R.: An algorithm for determining intersection segment-polygon
        in 3D, Computer & Graphics, Vol.22, No.5, pp.587-592, 1998.

    PROPERTIES:
      + Minimal storage (triangle vectors)
      + Efficient
      + Relatively robust (?)
      + Optional distance calculation
      + Backface culling (compulsory)
      - No UV coordinate calculation

    TODO:
      + Non-backface culling algorithm
      + Negative distance clipping ?
      + SIMD optimizations
      + It might be worthwhile to try writing a distance algorithm using Moller's ray-triangle intersection
        algorithm.
*/

namespace QRayRE { namespace SeguraFeito {

/*                               DEFINITIONS                                */
  template<class Type>
  inline Type triangePointDistance(const _Vector3<Type>& point,
                                   const _Vector3<Type>& v1,
                                   const _Vector3<Type>& v2,
                                   const _Vector3<Type>& v3);

/*                              IMPLEMENTATION                              */
  template<class Type>
  inline Type edgePointDistance(const _Vector3<Type>& p, const _Vector3<Type>& v1p, const _Vector3<Type>& v2p, const _Vector3<Type>& v1, const _Vector3<Type>& v2, const _Vector3<Type>& v2v1)
  {
    // Project distance vector (v1 - p) onto edge vector (v2 - v1)
    Type projDist = _Vector3<Type>::dot(v1p, v2v1);

    // Determine whether the closest distance is to vertex v1
    if(projDist < (Type)0.0)
      return v1p.getLength(); // distance to v1

    // Calculate the full projected distance
    Type v2v1Sqr = _Vector3<Type>::dot(v2v1, v2v1); // square length of edge v1 -> v2
    projDist /= v2v1Sqr;

    // Determine whether the closest distance is to vertex v2
    if(projDist*projDist > v2v1Sqr)
      return v2p.getLength(); // distance to v2

    // Calculate the closest distance to the line (using pythagoras's formula)

    /*const _Vector3<Type> distVec = v1p + projLength * v2v1;
    return distVec.getLength();*/

    return sqrt(v1p.getSquareLength() - projDist*projDist);
  }

  template<class Type>
  inline Type triangePointDistance(const _Vector3<Type>& point, const _Vector3<Type>& v1, const _Vector3<Type>& v2, const _Vector3<Type>& v3)
  {
    //// Initialize inputs
    Type det;

    // Origin point
    const _Vector3<Type>& p = point;

    // Destination point & (Un-normalized) plane normal
    const _Vector3<Type> v2v1 = v2-v1;
    const _Vector3<Type> v3v1 = v3-v1;
    const _Vector3<Type> normal = _Vector3<Type>::cross(v2v1, v3v1);
    const _Vector3<Type>& qp = normal; // Note: Technically this should be -normal, but we simply swap the determinant signs to compensate

    //// Check whether triangle is intersected by its closest on the plane from the point (ray intersect using the normal as the ray direction from the orgin point)
    // Check det([p v1 v2 q])
    const _Vector3<Type> v1p = v1-p;
    const _Vector3<Type> v2p = v2-p;

    det = _Vector3<Type>::dot(v1p, _Vector3<Type>::cross(v2p, qp));
    if(det > (Type)0.0)
    {
      // Calculate closest distance to edge v1 -> v2
      return edgePointDistance<Type>(p, v1p, v2p, v1, v2, v2v1);
    }

    // Check det([p v3 v1 q])
    const _Vector3<Type> v3p = v3-p;
    const _Vector3<Type> v3p_cross_qp = _Vector3<Type>::cross(v3p, qp);

    det = _Vector3<Type>::dot(v1p, v3p_cross_qp);
    if(det < (Type)0.0)
    {
      // Calculate closest distance to edge v1 -> v3
      return edgePointDistance<Type>(p, v1p, v3p, v1, v3, v3v1);
    }

    // Check det([p v2 v3 q])
    det = _Vector3<Type>::dot(v2p, v3p_cross_qp);
    if(det > (Type)0.0)
    {
      // Calculate closest distance to edge v2 -> v3
      return edgePointDistance<Type>(p, v2p, v3p, v2, v3, v3 - v2);
    }

    //// Calculate closest distance to plane
    return _Vector3<Type>::dot(v1p, normal) / normal.getLength();
  }

}}

#endif
