#ifndef __GLGE_API_H__
#define __GLGE_API_H__
//////////////////////////////////////////////////////////////////////////////
//
//    API.H
//
//    Author:      Rehno Lindeque
//
//    Description: GLGE classes used to implement api
//
//    Version: 0.1 (informal prototype)
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                                   INCLUDES                               */
// BaseGE
#include <basege/basege.h>

/*                               COMPILER MACROS                            */
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

/*                             FORWARD DECLERATIONS                         */
namespace GLGE
{
  class GLPoints;
  class GLLines;
  class GLTriangles;
  class GLQuads;
  class GLPolygons;
  class GLVertexData;
  class GLEdgeData;
  class GLPrimitiveData;
  class GLSurfaceData;
  class GLVertices;
  class GLEdges;
  class GLPrimitives;
  class GLSurfaces;
  class GLGeometry;
  class GLGeometryPool;
}

/*                                   INCLUDES                               */
#include "points.h"
#include "lines.h"
#include "triangles.h"
#include "quads.h"
#include "polygons.h"
#include "vertexdata.h"
#include "edgedata.h"
#include "primitivedata.h"
#include "surfacedata.h"
#include "vertices.h"
#include "edges.h"
#include "primitives.h"
#include "surfaces.h"
#include "geometry.h"
#include "geometrypool.h"

namespace GLGE
{
/*                                CONSTANTS                                 */
/*                                 GLOBALS                                  */
/*                              COMPILER MACROS                             */
#pragma warning(pop)
}

#endif
