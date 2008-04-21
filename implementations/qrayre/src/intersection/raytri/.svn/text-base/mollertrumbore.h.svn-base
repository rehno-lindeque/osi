#ifndef __QRAYRE_INTERSECTION_RAYTRI_MOLLERTRUMBORE_H__
#define __QRAYRE_INTERSECTION_RAYTRI_MOLLERTRUMBORE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    MOLLERTRUMBORE.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      Ray-Triangle intersection based on the MollerTrumbore algorithm.

    PROPERTIES:
      + Minimal storage (triangle vectors)
      + Efficient
      + UV coordinate and distance calculation
      + Backface culling (slightly slower than Baudell)
      - Not very robust
      - GuigueDevillers is possibly an improvement of MollerTrumbore

    CHANGES:
      + The backface culling was changed to user counter-clockwise vertex
        orientation for determining the front face. (similar to default 
        OpenGL behaviour)

    USAGE:
      + Use EPSILON = 0.000001 as default. (Unfortunately default template
        arguments for functions are not allowed).
      + Setting BACKFACE_CULL to true forces the algorithm to discard 
        back-facing triangles
      
    REFERENCES:
      + http://www.graphics.cornell.edu/pubs/1997/MT97.html

      + http://jgt.akpeters.com/papers/MollerTrumbore97/      
        author = "Tomas Möller and Ben Trumbore"
        title = "Fast, Minimum Storage Ray-Triangle Intersection"
        journal = "journal of graphics tools"
        volume = "2"
        number = "1"
        pages = "21-28"
        year = "1997"

      + http://www.cs.lth.se/home/Tomas_Akenine_Moller/raytri/
        title = "Practical Analysis of Optimized Ray-Triangle Intersection"
        author = "Tomas Möller"

    TODO:
      + It could be usefull to make a version where the edges (e1 and e2) 
        are passed as parameters in place of vectors v2 and v3 as an
        optimization (see pg3 of the paper)
      + SIMD optimizations
      + Precalculation optimizations
      + Possibly apply optimizations from the Moller's later analysis
*/

namespace QRayRE { namespace MollerTrumbore {

/*                               DEFINITIONS                                */
  template<class Type, Type EPSILON, bool BACKFACE_CULL>
  inline bool intersectTriangle(Type& distance, 
                                _Vector2<Type>& uv,
                                const _Vector3<Type>& rayOrigin, 
                                const _Vector3<Type>& rayDirection, 
                                const _Vector3<Type>& v1,
                                const _Vector3<Type>& v2,
                                const _Vector3<Type>& v3);

/*                              IMPLEMENTATION                              */
  template<class Type, Type EPSILON, bool BACKFACE_CULL>
  inline bool intersectTriangle(Type& distance, _Vector2<Type>& uv, const _Vector3<Type>& rayOrigin, const _Vector3<Type>& rayDirection, const _Vector3<Type>& v1, const _Vector3<Type>& v2, const _Vector3<Type>& v3)
  {
    _Vector2<Type> uvInt;
    _Vector3<Type> t, p, q;
    Type det, invDet;

    // Calculate edges
    const _Vector3<Type> e1 = v2 - v1;
    const _Vector3<Type> e2 = v3 - v1;
    
    // Calculate determinant
    p = _Vector3<Type>::cross(rayDirection, e1);
    det = _Vector3<Type>::dot(e2, p);

    if(BACKFACE_CULL)
    {
      /* Note: This branch allows us to put off the division operation until the intersection is confirmed */

      // Test whether the ray lies in the plane of the triangle (epsilon is close to zero)
      if(det < EPSILON)
        return false;

      // Calculate the distance vector from v1 to the ray origin
      t = rayOrigin - v1;
      
      // Calculate V coordinate (intersection)
      uvInt(1) = _Vector3<Type>::dot(t, p);
     
      // Test v coordinate intersection
      if(uvInt(1) < (Type)0.0 || uvInt(1) > det)
        return false;

      // Calculate u coordinate (intersection)
      q = _Vector3<Type>::cross(t, e2);
      uvInt(0) = _Vector3<Type>::dot(rayDirection, q);
           
      // Test u coordinate intersection
      if(uvInt(0) < (Type)0.0 || uvInt(0) + uvInt(1) > det)
        return false;

      // Calculate intersection distance and scale coordinates
      distance = _Vector3<Type>::dot(e1, q);
      invDet = ((Type)1.0) / det;
      distance *= invDet;
      uv = uvInt * invDet;
    }
    else
    {
      // Test whether the ray lies in the plane of the triangle (epsilon is close to zero)
      if(det > -EPSILON && det < EPSILON)
        return false;

      // Calculate inverse determinant
      invDet = ((Type)1.0) / det;

      // Calculate the distance from v1 to the ray origin
      t = rayOrigin - v1;

      // Calculate v coordinate
      uvInt(1) = _Vector3<Type>::dot(t, p) * invDet;

      // Test v coordinate intersection
      if(uvInt(1) < (Type)0.0 || uvInt(1) > (Type)1.0)
        return false;

      // Calculate u coordinate
      q = _Vector3<Type>::cross(t, e2);
      uvInt(0) = _Vector3<Type>::dot(rayDirection, q) * invDet;
      
      // Test v coordinate intersection
      if(uvInt(0) < (Type)0.0 || uvInt(0) + uvInt(1) > (Type)1.0)
        return false;

      // Calculate intersection distance
      distance = _Vector3<Type>::dot(e1, q) * invDet;
      uv = uvInt;
    }

    return true;
  }

}}

#endif
