#ifndef __BASEGE_BASEGE_H__
#define __BASEGE_BASEGE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    BASEGE.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Sample base classes for OpenGE backends.
*/
/*                              COMPILER MACROS                             */
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

#ifdef _MSC_VER
  #define STDEXT_NAMESPACE stdext
#else
  #define STDEXT_NAMESPACE __gnu_cxx
#endif

/*                                 INCLUDES                                 */
// OpenGE
#include <osi/ge/ge.h>
#include <osi/ge/intge.h>

// stdlibc++
#include <vector>
#include <list>
#include <stack>
#include <map>
#ifdef _MSC_VER
  #include <hash_map>
#else
  #include <ext/hash_map>
#endif

// libc
#include <math.h>

// Base
#include <base/common/types.h>
#include <base/common/functions.h>
#include <base/common/structures.h>
#include <base/common/object.h>
#include <base/math/math.h>

using Base::Object;

/*                            FORWARD DECLERATIONS                          */
namespace BaseGE
{
  //class Object;
  /*class Points;
  class Lines;
  class Triangles;
  class Quads;
  class Polygons;*/
  class Components;
  class VertexData;
  class EdgeData;
  class PrimitiveData;
  class SurfaceData;
  class Vertices;
  class Edges;
  class Primitives;
  class Surfaces;
  class Geometry;
  class GeometryPool;

  template<class ObjectType> ObjectType* createObject();
  template<class ObjectType> ObjectType* beginObject();
  template<class ObjectType> ObjectType* endObject();
  template<class ObjectType> ObjectType* openObject(OSobject object);
  template<class ObjectType> ObjectType* closeObject();
  template<class ObjectType> ObjectType* getActiveObject();
  void delObject(Base::Object* object);
  template<class ObjectType> ObjectType* cast_id(OSobject object);
  template<class ObjectType> OSobject cast_object(ObjectType* object);
  inline void shutdown();
}

/*                                 INCLUDES                                 */
// BaseGE headers
//#include "object.h"
#include "attributeobject.h"
#include "elements.h"
#include "elementspool.h"
/*#include "points.h"
#include "lines.h"
#include "triangles.h"
#include "quads.h"
#include "polygons.h"*/
#include "vertexdata.h"
#include "edgedata.h"
#include "primitivedata.h"
#include "surfacedata.h"
#include "vertices.h"
#include "edges.h"
#include "primitives.h"
#include "surfaces.h"
#include "components.h"
#include "geometry.h"
#include "spatialstructure.h"
#include "geometrypool.h"
#include "frame.h"
#include "memorypool.h"

/*                                 GLOBALS                                  */
namespace BaseGE
{
  extern std::list<Object*> objects;
  extern std::stack<Object*> activeObjects;
  extern ElementsPool elementsPool;
}

/*                              IMPLEMENTATION                              */
#include "basege.hxx"

/*                              COMPILER MACROS                             */
#pragma warning(pop)

#endif
