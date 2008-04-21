#ifndef __QRAYRE_INTERSECTION_RAYTRI_GUIGUEDEVILLERS_H__
#define __QRAYRE_INTERSECTION_RAYTRI_GUIGUEDEVILLERS_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GUIGUEDEVILLERS.H
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Ray-Triangle intersection based on the GuigueDevillers algorithm.

    PROPERTIES:
      + Minimal storage (triangle vectors)
      + Efficient
      + UV coordinate and distance calculation
      + Backface culling
      + Robust (more so than MollerTrumbore in any case)
      + Improvement on MollerTrumbore

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
      + http://www.acm.org/jgt/papers/GuigueDevillers03/
        author = "Philippe Guigue and Olivier Devillers",
        title = "Fast and Robust Triangle-Triangle Overlap Test using Orientation Predicates"
        journal = "Journal of Graphics Tools: JGT"
        volume = "8"
        number = "1"
        pages = "25-42"
        year = "2003"
        CODEN = "JGTOFD"
        ISSN = "1086-7651"
        bibdate = "Tue Dec 16 13:47:48 2003"
        abstract = "This paper presents an algorithm for determining
                    whether two triangles in three dimensions intersect.
                    The general scheme is identical to the one proposed by
                    Moller. The main difference is that our algorithm
                    relies exclusively on the sign of 4x4 determinants and
                    does not need any intermediate explicit constructions
                    which are source of numerical errors. Besides the fact
                    that the resulting code is more reliable than existing
                    methods, it is also more efficient. The source code is
                    available online."

      + http://jgt.akpeters.com/papers/GuigueDevillers03/addendum.html
        author = "Philippe Guigue and Olivier Devillers",
        title = "Fast Ray-Triangle Intersection Test Using Orientation Determinants
                 (Addemdum to Fast and Robust Triangle-Triangle Overlap Test using Orientation Predicates)"
        abstract = "We describe in the following how the value of orientation
                    determinants can be used to obtain an efficient solution for
                    the ray/triangle intersection problem.
                    This problem can be stated as follow: given a triangle abc,
                    and a ray r(t) = o + tdir defined by its origin point o, and
                    a direction vector dir, decide whether the ray and the
                    triangle intersect, and if so, compute the barycentric
                    coordinates (u,v) of the intersection point,
                    r(t) = (1 - u - v)a + ub + vc.
                    The structure of the algorithm is identical to the algorithm
                    presented by Möller and Trumbore (Fast, Minimum Storage
                    Ray-Triangle Intersection), i.e. it performs in the same
                    order the same rejection tests on the barycentric coordinates
                    of the intersection point between the ray and the supporting
                    plane of the triangle. The algorithms differ only in the way
                    these barycentric coordinates are calculated."

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
      + SIMD optimizations
      + Rewrite to take vertex winding as a template argument
      + Possibly apply optimizations from the Moller's later analysis
*/

namespace QRayRE { namespace GuigueDevillers {

/*                               DEFINITIONS                                */
  #ifdef RAYTRI_EPSILON
  #define GUIGUEDEVILLERS_EPSILON RAYTRI_EPSILON
  #else
  #define GUIGUEDEVILLERS_EPSILON 0.000001f
  #endif

  //template<class Type, Type EPSILON, bool BACKFACE_CULL>
  template<class Type, bool BACKFACE_CULL>
  inline bool intersectTriangle(Type& distance,
                                _Vector2<Type>& uv,
                                const _Vector3<Type>& rayOrigin,
                                const _Vector3<Type>& rayDirection,
                                const _Vector3<Type>& v1,
                                const _Vector3<Type>& v2,
                                const _Vector3<Type>& v3);

/*                              IMPLEMENTATION                              */
  template<class Type, bool BACKFACE_CULL>
  inline bool intersectTriangle(Type& distance, _Vector2<Type>& uv, const _Vector3<Type>& rayOrigin, const _Vector3<Type>& rayDirection, const _Vector3<Type>& v1, const _Vector3<Type>& v2, const _Vector3<Type>& v3)
  {
    _Vector2<Type> uvInt;
	  Type det, invDet;

    // Calculate edges
    const _Vector3<Type> e1 = v2 - v1;
    const _Vector3<Type> e2 = v3 - v1;

	  // Calculate (unnormalized) normal
    const _Vector3<Type> normal = _Vector3<Type>::cross(e1, e2);

    // Calculate determinant
    /*note: orientation of the ray with respect to the triangle's normal,
	   also used to calculate output parameters*/

    //old: det = -_Vector3<Type>::dot(rayDirection, normal);
    det = _Vector3<Type>::dot(rayDirection, normal);

	  if(BACKFACE_CULL)
    {
      // Test whether the ray lies in the plane of the triangle (epsilon is close to zero)
      if(det < GUIGUEDEVILLERS_EPSILON)
        return false;

      // Calculate the distance vector from the ray origin to v1
      const _Vector3<Type> t1 = v1 - rayOrigin; // SUB(vect0,a,orig);

      // Calculate the vector used to calculate u and v parameters
      //old: const _Vector3<Type> n = _Vector3<Type>::cross(t1, rayDirection);
      const _Vector3<Type> n = -_Vector3<Type>::cross(rayDirection, t1);

      // Calculate the distance vector from the ray origin to v2
      const _Vector3<Type> t2 = v2 - rayOrigin;

      // Calculate the unormalized v coordinate (intersection)
      uvInt(1) = -_Vector3<Type>::dot(t2, n);

      // Test v coordinate intersection
      if(uvInt(1) < (Type)0.0 || uvInt(1) > det)
        return false;

      // Calculate the distance vector from the ray origin to v3
      const _Vector3<Type> t3 = v3 - rayOrigin;

      // Calculate the unormalized u coordinate (intersection)
      uvInt(0) = _Vector3<Type>::dot(t3, n);

      // Test u coordinate intersection
      if (uvInt(0) < (Type)0.0 || uvInt(0) + uvInt(1) > det)
        return false;

      // Calculate unormalized distance (t coordinate)
      distance = _Vector3<Type>::dot(t1,normal);

      // Calculate inverse determinant
	    invDet = ((Type)1.0) / det;

      // Normalize u, v and distance (t) coordinates
      uv = uvInt * invDet;
      distance *= invDet;
    }
    else
    {
      // Test whether the ray lies in the plane of the triangle (epsilon is close to zero)
      if (det > -GUIGUEDEVILLERS_EPSILON && det < GUIGUEDEVILLERS_EPSILON)
        return false;

      // Calculate the distance vector from the ray origin to v1
      const _Vector3<Type> t1 = v1 - rayOrigin;

      // Calculate the vector used to calculate u and v parameters
      //old: const _Vector3<Type> n = _Vector3<Type>::cross(rayDirection, t1);
      const _Vector3<Type> n = _Vector3<Type>::cross(t1, rayDirection);

      // Calculate inverse determinant
   	  invDet = (Type)1.0 / det;

      // Calculate the distance vector from the ray origin to v2
      const _Vector3<Type> t2 = v2 - rayOrigin;

      // Calculate (normalized) v coordinate
      uvInt(1) = -_Vector3<Type>::dot(t2, n) * invDet;

      // Test v coordinate intersection
      if(uvInt(1) < (Type)0.0 || uvInt(1) > 1.0)
        return false;

      // Calculate the distance vector from the ray origin to v3
      const _Vector3<Type> t3 = v3 - rayOrigin;

      // Calculate (normalized) u coordinate
      uvInt(0) = _Vector3<Type>::dot(t3, n) * invDet;

      // Test u coordinate intersection
      if(uvInt(0) < (Type)0.0 || uvInt(0) + uvInt(1) > (Type)1.0)
        return false;

      // Calculate distance (t coordinate)
      const Type distanceInt = -_Vector3<Type>::dot(t1, normal) * invDet;

      /*if(distanceInt < (Type)0.0) // Todo: Is this really necessary? (check)
        return false;*/

      distance = distanceInt;
      uv = uvInt;
    }
    return true;
  }

}}

#undef GUIGUEDEVILLERS_EPSILON

#endif
