//////////////////////////////////////////////////////////////////////////////
//
//    API.CPP
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
*/
/*                              COMPILER MACROS                             */
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

/*                                 INCLUDES                                 */
// GLGE
#include "api.h"

using namespace GLGE;
using namespace BaseGE;
/*                                  GLOBALS                                 */

/*                               IMPLEMENTATION                             */

/* elements */
OSobject geBeginElements(OS_BASIC_TYPE type)
{
  Elements* elementsObject = beginObject<Elements>();
  elementsObject->init(type);
  return cast_object(elementsObject);
}

void geEndElements()
{
  Elements* elementsObject = endObject<Elements>();
}

void geOpenElements(OSobject elements)
{
  Elements* elementsObject = openObject<Elements>(elements);
}

void geCloseElements()
{
  Elements* elementsObject = closeObject<Elements>();
}

void geAddElements(OSuint nElements, void* elements)
{
  Elements* elementsObject = getActiveObject<Elements>();
  elementsObject->add(nElements, elements);
}

/* geometry pool */
OSobject geBeginGeometryPool()
{
  GLGeometryPool* geometryPoolObject = beginObject<GLGeometryPool>();
  return cast_object(geometryPoolObject);
}

void geEndGeometryPool()
{
  GLGeometryPool* geometryPoolObject = endObject<GLGeometryPool>();
  //todo: calculate bounding box? geometryPoolObject->calculateBoundingBox();
}

void geOpenGeometryPool(OSobject geometryPool)
{
  GLGeometryPool* geometryPoolObject = openObject<GLGeometryPool>(geometryPool);
}

void geCloseGeometryPool()
{
  GLGeometryPool* geometryPoolObject = closeObject<GLGeometryPool>();
  //todo: recalculate bounding box?
}

void geGeometry(OSobject geometry)
{
  //todo?
}

/* geometry frame */
OSobject geBeginFrame()
{
  Frame* frameObject = beginObject<Frame>();
  return cast_object(frameObject);
}

void geEndFrame()
{
  Frame* frameObject = endObject<Frame>();
}

void geOpenFrame(OSobject frame)
{
  Frame* frameObject = openObject<Frame>(frame);
}

void geCloseFrame()
{
  Frame* frameObject = closeObject<Frame>();
}

void geClearFrame()
{
  Frame* frameObject = getActiveObject<Frame>();
  //todo
}

void geTransformFrame(OSfloat* matrix)
{
  Frame* frameObject = getActiveObject<Frame>();
  //todo
}

void geTranslateFrame(OSfloat x, OSfloat y, OSfloat z)
{
  Frame* frameObject = getActiveObject<Frame>();
  //todo
}

void geTranslateFrame(OSfloat* v)
{
  Frame* frameObject = getActiveObject<Frame>();
  //todo
}

void geRotateFrame(OSfloat angle, OSfloat x, OSfloat y, OSfloat z)
{
  Frame* frameObject = getActiveObject<Frame>();
  //todo
}

void geScaleFrame(OSfloat x, OSfloat y, OSfloat z)
{
  Frame* frameObject = getActiveObject<Frame>();
  //todo
}

void geScaleFrame(OSfloat* v)
{
  Frame* frameObject = getActiveObject<Frame>();
  //todo
}

/* geometry instance */
void geBeginGeometryInstanceData(OSobject geometry)
{
  //todo
}

void geEndGeometryInstanceData()
{
  //todo
}

OSid geAddGeometryInstance(OSobject geometry, GE_GEOMETRY_STABILITY stability)
{
  GLGeometryPool* geometryPoolObject = getActiveObject<GLGeometryPool>();
  GLGeometry* geometryObject = cast_id<GLGeometry>(geometry);
  OSid instance = geometryPoolObject->addGeometryInstance(geometryObject, stability);
  return instance;
}

void geRemoveGeometryInstance(OSid geometryInstance)
{
  GLGeometryPool* geometryPoolObject = getActiveObject<GLGeometryPool>();
  OSuint index = (OSuint)geometryInstance;
  geometryPoolObject->removeGeometryInstance(index);
}

/* geometry */
OSobject geBeginGeometry(GE_GEOMETRY type)
{
  GLGeometry* geometryObject = beginObject<GLGeometry>();
  return cast_object(geometryObject);
}

void geEndGeometry()
{
  GLGeometry* geometryObject = endObject<GLGeometry>();
  geometryObject->build();
}

void gePoints(OSobject points)
{
  GLGeometry* geometryObject = getActiveObject<GLGeometry>();
  geometryObject->addPoints(cast_id<GLPoints>(points));
}

void geLines(OSobject lines)
{
  GLGeometry* geometryObject = getActiveObject<GLGeometry>();
  geometryObject->addLines(cast_id<GLLines>(lines));
}

void geTriangles(OSobject triangles)
{
  GLGeometry* geometryObject = getActiveObject<GLGeometry>();
  geometryObject->addTriangles(cast_id<GLTriangles>(triangles));
}

void geQuads(OSobject quads)
{
  GLGeometry* geometryObject = getActiveObject<GLGeometry>();
  geometryObject->addQuads(cast_id<GLQuads>(quads));
}

void gePolygons(OSobject polygons)
{
  GLGeometry* geometryObject = getActiveObject<GLGeometry>();
  geometryObject->addPolygons(cast_id<GLPolygons>(polygons));
}

/* geometric components */
OSobject geBeginPoints()
{
  GLPoints* pointsObject = beginObject<GLPoints>();
  return cast_object(pointsObject);
}

void geEndPoints()
{
  GLPoints* pointsObject = endObject<GLPoints>();
}

OSobject geBeginLines()
{
  GLLines* linesObject = beginObject<GLLines>();
  return cast_object(linesObject);
}

void geEndLines()
{
  GLLines* linesObject = endObject<GLLines>();
}

OSobject geBeginTriangles()
{
  GLTriangles* trianglesObject = beginObject<GLTriangles>();
  return cast_object(trianglesObject);
}

void geEndTriangles()
{
  GLTriangles* trianglesObject = endObject<GLTriangles>();
}

OSobject geBeginQuads()
{
  GLQuads* quadsObject = beginObject<GLQuads>();
  return cast_object(quadsObject);
}

void geEndQuads()
{
  GLQuads* quadsObject = beginObject<GLQuads>();
}

OSobject geBeginPolygons()
{
  GLPolygons* polygonsObject = beginObject<GLPolygons>();
  return cast_object(polygonsObject);
}

void geEndPolygons()
{
  GLPolygons* polygonsObject = endObject<GLPolygons>();
}

/* sub-components */
void geVertexData(OSobject vertexData)
{
  Components* componentsObject = getActiveObject<Components>();
  componentsObject->vertexData = cast_id<GLVertexData>(vertexData);
}

void geVertices(OSobject vertices)
{
  Components* componentsObject = getActiveObject<Components>();
  componentsObject->vertices = cast_id<GLVertices>(vertices);
}

void geEdgeData(OSobject edgeData)
{
  Components* componentsObject = getActiveObject<Components>();
  componentsObject->edgeData = cast_id<GLEdgeData>(edgeData);
}

void geEdges(OSobject edges)
{
  Components* componentsObject = getActiveObject<Components>();
  componentsObject->edges = cast_id<GLEdges>(edges);
}

void gePrimitiveData(OSobject primitiveData)
{
  Components* componentsObject = getActiveObject<Components>();
  componentsObject->primitiveData = cast_id<GLPrimitiveData>(primitiveData);
}

void gePrimitives(OSobject primitives)
{
  Components* componentsObject = getActiveObject<Components>();
  componentsObject->primitives = cast_id<GLPrimitives>(primitives);
}

void geSurfaceData(OSobject surfaceData)
{
  Components* componentsObject = getActiveObject<Components>();
  componentsObject->surfaceData = cast_id<GLSurfaceData>(surfaceData);
}

void geSurfaces(OSobject surfaces)
{
  Components* componentsObject = getActiveObject<Components>();
  componentsObject->surfaces = cast_id<GLSurfaces>(surfaces);
}

/* indexing components */
OSobject geBeginVertices(OSuint nVertices)
{
  GLVertices* verticesObject = beginObject<GLVertices>();
  return cast_object(verticesObject);
}

void geEndVertices()
{
  GLVertices* verticesObject = endObject<GLVertices>();
}

OSobject geBeginEdges(OSuint nEdges)
{
  GLEdges* edgesObject = beginObject<GLEdges>();
  return cast_object(edgesObject);
}

void geEndEdges()
{
  GLEdges* edgesObject = endObject<GLEdges>();
}

OSobject geBeginPrimitives(OSuint nPrimitives)
{
  GLPrimitives* primitivesObject = beginObject<GLPrimitives>();
  return cast_object(primitivesObject);
}

void geEndPrimitives()
{
  GLPrimitives* primitivesObject = endObject<GLPrimitives>();
}

OSobject geBeginSurfaces(OSuint nSurfaces)
{
  GLSurfaces* surfacesObject = beginObject<GLSurfaces>();
  return cast_object(surfacesObject);
}

void geEndSurfaces()
{
  GLSurfaces* surfacesObject = endObject<GLSurfaces>();
}

/* data components*/
OSobject geBeginVertexData(OSuint n)
{
  GLVertexData* vertexData = beginObject<GLVertexData>();
  return cast_object(vertexData);
}

void geEndVertexData()
{
  GLVertexData* vertexData = endObject<GLVertexData>();
}

OSobject geBeginEdgeData(OSuint n)
{
  GLEdgeData* edgeData = beginObject<GLEdgeData>();
  return cast_object(edgeData);
}

void geEndEdgeData()
{
  GLEdgeData* edgeData = endObject<GLEdgeData>();
}

OSobject geBeginPrimitiveData(OSuint n)
{
  GLPrimitiveData* primitiveData = beginObject<GLPrimitiveData>();
  return cast_object(primitiveData);
}

void geEndPrimitiveData()
{
  GLPrimitiveData* primitiveData = endObject<GLPrimitiveData>();
}

OSobject geBeginSurfaceData(OSuint n)
{
  GLSurfaceData* surfaceData = beginObject<GLSurfaceData>();
  return cast_object(surfaceData);
}

void geEndSurfaceData()
{
  GLSurfaceData* surfaceData = endObject<GLSurfaceData>();
}

/* attributes */
void geAttributes(OSenum attribute, OSobject elements)
{
  AttributeObject* object = (AttributeObject*)activeObjects.top();
  object->setAttribute(attribute, elements);
}

/* miscelaneous */
void geDelObject(OSobject object)
{
  delObject((Object*)object);
}

void geShutdown()
{
  shutdown();
}


/*                              COMPILER MACROS                             */
#pragma warning(pop)
