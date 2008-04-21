#ifndef __QRAYRE_INTERSECTION_RAYTRI_SEGURAFEITO_H__
#define __QRAYRE_INTERSECTION_RAYTRI_SEGURAFEITO_H__
//////////////////////////////////////////////////////////////////////////////
//
//    SEGURAFEITO.H
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      Ray-Triangle intersection based on the SeguraFeito algorithm.
      
    REFERENCES:
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
*/

namespace QRayRE { namespace SeguraFeito {

/*                               DEFINITIONS                                */
  template<class Type, Type EPSILON>
  inline bool intersectTriangle(const _Vector3<Type>& rayOrigin, 
                                const _Vector3<Type>& rayDirection, 
                                const _Vector3<Type>& v1,
                                const _Vector3<Type>& v2,
                                const _Vector3<Type>& v3);

  template<class Type>
  inline bool intersectTriangle(Type& distance,
                                const _Vector3<Type>& rayOrigin, 
                                const _Vector3<Type>& rayDirection, 
                                const _Vector3<Type>& v1,
                                const _Vector3<Type>& v2,
                                const _Vector3<Type>& v3);

/*                              IMPLEMENTATION                              */
  template<class Type>
  inline bool intersectTriangle(const _Vector3<Type>& rayOrigin, const _Vector3<Type>& rayDirection, const _Vector3<Type>& v1, const _Vector3<Type>& v2, const _Vector3<Type>& v3)
  {
    // Initialize inputs
    const _Vector3<Type>& p = rayOrigin;
    const _Vector3<Type> qp = rayDirection; // (we're only insterested in the sign, so we can use rayDirection directly
                                            //  rather than calculating q value somewhere behind the triangle's plane)
    Type det;
    
    //// Check that triangle is intersected
    // Check det([p v1 v2 q])
    const _Vector3<Type> v1p = v1-p;
    const _Vector3<Type> v2p = v2-p;

    det = _Vector3<Type>::dot(v1p, _Vector3<Type>::cross(v2p, qp));
    if(det < (Type)0.0)
      return false;

    // Check det([p v3 v1 q])
    const _Vector3<Type> v3p_cross_qp = _Vector3<Type>::cross(v3-p, qp);

    det = _Vector3<Type>::dot(v1p, v3p_cross_qp);
    if(det > (Type)0.0)
      return false;

    // Check det([p v2 v3 q])
    det = _Vector3<Type>::dot(v2p, v3p_cross_qp);
    return det < (Type)0.0;
  }

  template<class Type>
  inline bool intersectTriangle(Type& distance, const _Vector3<Type>& rayOrigin, const _Vector3<Type>& rayDirection, const _Vector3<Type>& v1, const _Vector3<Type>& v2, const _Vector3<Type>& v3)
  {
    // Initialize inputs
    const _Vector3<Type>& p = rayOrigin;
    const _Vector3<Type> qp = rayDirection; // (we're only insterested in the sign, so we can use rayDirection directly
                                            //  rather than calculating q value somewhere behind the triangle's plane)
    Type det;
    
    //// Check that triangle is intersected
    // Check det([p v1 v2 q])
    const _Vector3<Type> v1p = v1-p;
    const _Vector3<Type> v2p = v2-p;

    det = _Vector3<Type>::dot(v1p, _Vector3<Type>::cross(v2p, qp));
    if(det < (Type)0.0)
      return false;

    // Check det([p v3 v1 q])
    const _Vector3<Type> v3p_cross_qp = _Vector3<Type>::cross(v3-p, qp);

    det = _Vector3<Type>::dot(v1p, v3p_cross_qp);
    if(det > (Type)0.0)
      return false;

    // Check det([p v2 v3 q])
    det = _Vector3<Type>::dot(v2p, v3p_cross_qp);
    if(det < (Type)0.0)
      return false;

    //// Find intersect distance
    // Calculate (unnormalized) normal
    const _Vector3<Type> normal = _Vector3<Type>::cross(v2-v1, v3-v1);
    
    // Calculate determinant
    det = _Vector3<Type>::dot(rayDirection, normal);
    
    // Calculate distance
    distance = _Vector3<Type>::dot(v1p, normal) / det;
    return true;
  }

}}

#endif
