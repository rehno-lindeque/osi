#ifndef __OSI_GE_H__
#define __OSI_GE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GE.H
//
//    Ver 0.1 (2006-09-02)
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*
    DESCRIPTION:
      Main OpenGE interface.
*/
/*                                 INCLUDES                                 */
#include "../os.h"

/*                                 CONSTANTS                                */
#define GE_VERSION 0.1

/*                                   TYPES                                  */
enum GE_GEOMETRY
{
  GE_MESH
};

enum GE_COMPONENT
{
  GE_POINT,
  GE_LINE,
  GE_TRIANGLE,
  GE_QUAD,
  GE_POLYGON
};

enum GE_ATTRIBUTE
{
  GE_INDEX    = 0x00000000,
  GE_POSITION = 0x00000001,
  GE_NORMAL   = 0x00000002
};

enum GE_GEOMETRY_STABILITY
{
  GE_STATIC,
  GE_DYNAMIC_STABLE,
  GE_DYNAMIC_UNSTABLE
};

/*                                     API                                  */

/* elements */
OSobject geBeginElements(OS_BASIC_TYPE type);
void geEndElements();

void geOpenElements(OSobject elements);
void geCloseElements();

void geAddElements(OSuint nElements, void* elements);

/* geometry pool */
OSobject geBeginGeometryPool();
void geEndGeometryPool();

void geOpenGeometryPool(OSobject geometryPool);
void geCloseGeometryPool();

/* geometry frame */
OSobject geBeginFrame();
void geEndFrame();

void geOpenFrame(OSobject frame);
void geCloseFrame();

void geClearFrame();
void geTransformFrame(OSfloat* matrix);
void geTranslateFrame(OSfloat x, OSfloat y, OSfloat z);
void geTranslateFrame(OSfloat* v);
void geRotateFrame(OSfloat angle, OSfloat x, OSfloat y, OSfloat z);
void geScaleFrame(OSfloat x, OSfloat y, OSfloat z);
void geScaleFrame(OSfloat* v);

/* geometry instance */
void geBeginGeometryInstanceData(OSobject geometry);
void geEndGeometryInstanceData();

OSid geAddGeometryInstance(OSobject geometry, GE_GEOMETRY_STABILITY stability);
void geRemoveGeometryInstance(OSid geometryInstance);

/* geometry */
OSobject geBeginGeometry(GE_GEOMETRY type);
void geEndGeometry();

void gePoints(OSobject points);
void geLines(OSobject lines);
void geTriangles(OSobject triangles);
void geQuads(OSobject quads);
void gePolygons(OSobject polygons);

/*/* geometry data *
void geTransformations(OSid transformationElements);*/

/* geometric components */
OSobject geBeginPoints();
void geEndPoints();
OSobject geBeginLines();
void geEndLines();
OSobject geBeginTriangles();
void geEndTriangles();
OSobject geBeginQuads();
void geEndQuads();
OSobject geBeginPolygons();
void geEndPolygons();

/* geometric sub-components */
void geVertexData(OSobject vertexData);
//void geVertices(OSobject vertices, OSuint16 offset = 0, OSuint16 length = 0); (todo: ??? Can't remember: Check design docs...)
void geVertices(OSobject vertices);
void geEdgeData(OSobject edgeData);
void geEdges(OSobject edges);
void gePrimitiveData(OSobject primitiveData);
void gePrimitives(OSobject primitives);
void geSurfaceData(OSobject surfaceData);
void geSurfaces(OSobject surfaces);

/* indexing components */
OSobject geBeginVertices(OSuint nVertices);
void geEndVertices();
OSobject geBeginEdges(OSuint nEdges);
void geEndEdges();
OSobject geBeginPrimitives(OSuint nPrimitives);
void geEndPrimitives();
OSobject geBeginSurfaces(OSuint nSurfaces);
void geEndSurfaces();

/* data components*/
OSobject geBeginVertexData(OSuint n);
void geEndVertexData();
OSobject geBeginEdgeData(OSuint n);
void geEndEdgeData();
OSobject geBeginPrimitiveData(OSuint n);
void geEndPrimitiveData();
OSobject geBeginSurfaceData(OSuint n);
void geEndSurfaceData();

/* attributes */
void geAttributes(OSenum attribute, OSobject elements);

/* indexing */
/*void geIndices(OSid indexElements);
void geComponentCounts(OSid countElements);
//void geMapCoordinates(OSid coordinateElements, GEuint coordinateSet);*/

/* miscelaneous */
void geDelObject(OSobject object);
void geShutdown();

#endif
