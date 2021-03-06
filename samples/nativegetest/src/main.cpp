//////////////////////////////////////////////////////////////////////////////
//
//    MAIN.CPP
//
//    Copyright (c) 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      A NativeGE unit test
*/
/*                                  INCLUDES                                */

#include <windows.h>

/* Common*/
#include <common/types.h>

/* OpenGE */
#include <osi/ge/ge.h>
#include <osi/ge/intge.h>
#include <math.h>

/*                              COMPILER MACROS                             */
#pragma warning(push)
#pragma warning(disable: 4305)

/*                                  CLASSES                                 */
class NativeGETest
{
protected:
  OSobject positionElements;
  OSobject normalElements;
  OSobject indexElements;
  OSobject vertices;
  OSobject vertexData;
  OSobject geometry;
  OSobject geometryPool;
#ifdef USE_QUADS
  OSobject quads;
#else
  OSobject triangles;
#endif

public:
  void buildScene()
  {
    float height = 1.0f;

    float v[][3] = {
      // floor
      { 6.0f,  6.0f, 0.0f},
      {-6.0f,  6.0f, 0.0f},
      {-6.0f, -6.0f, 0.0f},
      { 6.0f, -6.0f, 0.0f},

      // cube
      { 1.0f, -1.0f, 0.0f + height}, // front vertices
      { 1.0f, -1.0f, 2.0f + height},
      {-1.0f, -1.0f, 2.0f + height},
      {-1.0f, -1.0f, 0.0f + height},

      { 1.0f, 1.0f, 2.0f + height}, // back vertices
      { 1.0f, 1.0f, 0.0f + height},
      {-1.0f, 1.0f, 0.0f + height},
      {-1.0f, 1.0f, 2.0f + height}};

    const float inv_sqrt3 = 1.0f/sqrtf(3.0f);

    float n[][3] = {
      // floor
      { 0.0f, 0.0f, 1.0f},
      { 0.0f, 0.0f, 1.0f},
      { 0.0f, 0.0f, 1.0f},
      { 0.0f, 0.0f, 1.0f},

      // cube
      { inv_sqrt3, -inv_sqrt3,-inv_sqrt3}, // front vertices
      { inv_sqrt3, -inv_sqrt3, inv_sqrt3},
      {-inv_sqrt3, -inv_sqrt3, inv_sqrt3},
      {-inv_sqrt3, -inv_sqrt3,-inv_sqrt3},

      { inv_sqrt3, inv_sqrt3,  inv_sqrt3}, // back vertices
      { inv_sqrt3, inv_sqrt3, -inv_sqrt3},
      {-inv_sqrt3, inv_sqrt3, -inv_sqrt3},
      {-inv_sqrt3, inv_sqrt3,  inv_sqrt3}};

#ifdef USE_QUADS
    uint16 i[] = {
      0,  1,  2,  3,  // floor

      4,  5,  6,  7,  // front face
      8,  5,  4,  9,  // left face
      8,  9, 10, 11,  // back face
      6, 11, 10,  7,  // right face
      8, 11,  6,  5,  // top face
      4,  7, 10,  9 }; // bottom face
#else
    uint16 i[] = {
      0,  1,  3,  1,  2,  3,   // floor

      4,  5,  7,  5,  6,  7,   // front face
      8,  5,  9,  5,  4,  9,   // left face
      8,  9, 11,  9, 10, 11,   // back face
      6, 11,  7, 11, 10,  7,   // right face
      8, 11,  5, 11,  6,  5,   // top face
      4,  7,  9,  7, 10,  9 }; // bottom face
#endif
    
    /* Create geometry */
    // vector elements
    positionElements = geBeginElements(OS_VEC3_FLOAT);
      geAddElements(12, v);
    geEndElements();
    
    // normal elements
    normalElements = geBeginElements(OS_VEC3_FLOAT);
      geAddElements(12, n);
    geEndElements();

    // index elements
    indexElements = geBeginElements(OS_UINT16);
      geAddElements(42, i);
    geEndElements();
    
    // vertex data
    vertexData = geBeginVertexData(12);
      geAttributes(GE_POSITION, positionElements);
      geAttributes(GE_NORMAL, normalElements);
    geEndVertexData();

    // vertices
    vertices = geBeginVertices(42);
      geAttributes(GE_INDEX, indexElements);
    geEndVertices();

#ifdef USE_QUADS
    // quads
    quads = geBeginQuads();
      geVertices(vertices);
      geVertexData(vertexData);
    geEndQuads();

    // geometry
    geometry = geBeginGeometry(GE_MESH);
      geQuads(quads);
    geEndGeometry();
#else
    // triangles
    triangles = geBeginTriangles();
      geVertices(vertices);
      geVertexData(vertexData);
    geEndTriangles();

    // geometry
    geometry = geBeginGeometry(GE_MESH);
      geTriangles(triangles);
    geEndGeometry();
#endif

    /* Instanciate geometry */
    geometryPool = geBeginGeometryPool();
      //todo: ? OSobject frame = geBeginFrame();
        OSid instance = geAddGeometryInstance(geometry, GE_STATIC);
      //todo: ? geEndFrame();
    geEndGeometryPool();

    /* Create structure */
    OSid bihStructure = geGetStructure(GE_NATIVE, GE_BIH, geometryPool);

    /* Delete objects */
    geDelObject(geometryPool);
    //todo: ? geDelObject(frame);
    geDelObject(geometry);
#ifdef USE_QUADS
    geDelObject(quads);
#else
    geDelObject(triangles);
#endif
    geDelObject(vertices);
    geDelObject(vertexData);
    geDelObject(positionElements);
    geDelObject(normalElements);
    geDelObject(indexElements);

    /*REid geometry = reBeginGeometry(RE_MESH_GEOMETRY);
      REid locations = reElements(RE_VEC3_FLOAT, 12, v);
      REid indices = reElements(RE_UINT16, 42, i);      
      REid normals = reElements(RE_VEC3_FLOAT, 12, n);

      REid points = reBeginPoints(12);
        reAttributes(RE_LOCATION, locations);
        reAttributes(RE_NORMAL,   normals);
      reEndPoints();

      REid vertices = geBeginVertices(42);
        reAttributes(RE_INDEX, indices);
      reEndVertices();

      reBeginTriangles();
        rePoints(points);
        reVertices(vertices);
      reEndTriangles();
    reEndGeometry();*/
  }

  virtual void init()
  {
    // Initialize OpenGE
    //geInit();    

    // Construct scene
    buildScene();
  }

  virtual bool main()
  {
    return false;
  }

  virtual void end()
  {
    geShutdown();
  }

  NativeGETest() {}
}application;

int main()
{
  application.init();
  while(application.main()) {}
  application.end();
  return 0;
}

/*                              COMPILER MACROS                             */
#pragma warning(pop)
