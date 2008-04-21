//////////////////////////////////////////////////////////////////////////////
//
//    MAIN.CPP
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      A QRayRE test
*/
/*                                  INCLUDES                                */

#include <windows.h>

/* Common*/
#include <common/types.h>
#include <common/math/math.h>
#include <common/camera/camera.h>

/* OpenGE */
#include <osi/ge/ge.h>

/* OpenRE */
#include <osi/re/re.h>
#ifdef _DEBUG
  #include <osi/re/redbg.h>
#endif
#include <rewrapper/openreapplication.h>

/* OpenGUI */
#include <osi/gui/gui.hpp>

/* Other */
//#include <math.h>

using namespace REWrapper;
//#define USE_QUADS

/*                              COMPILER MACROS                             */
#pragma warning(push)
#pragma warning(disable: 4305)

/*                                  CLASSES                                 */
class QRayRETest : public OpenREApp
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

  OSobject scene;
  OSobject camera;
  OSobject light;
  OSobject render;

  GUI* gui;

public:
  void initializeGeometry()
  {
    float height = 0.1f;

    float v[][3] = {
      // floor
      { 6.0f, 0.0f,  6.0f},
      {-6.0f, 0.0f,  6.0f},
      {-6.0f, 0.0f, -6.0f},
      { 6.0f, 0.0f, -6.0f},

      // cube
      { 1.0f, 2.0f + height, 1.0f}, // front vertices
      { 1.0f, 0.0f + height, 1.0f},
      {-1.0f, 0.0f + height, 1.0f},
      {-1.0f, 2.0f + height, 1.0f},

      { 1.0f, 0.0f + height, -1.0f}, // back vertices
      { 1.0f, 2.0f + height, -1.0f},
      {-1.0f, 2.0f + height, -1.0f},
      {-1.0f, 0.0f + height, -1.0f}};

    const float inv_sqrt3 = 1.0f/sqrtf(3.0f);

    float n[][3] = {
      // floor
      { 0.0f, 1.0f, 0.0f},
      { 0.0f, 1.0f, 0.0f},
      { 0.0f, 1.0f, 0.0f},
      { 0.0f, 1.0f, 0.0f},

      // cube
      { inv_sqrt3, -inv_sqrt3, -inv_sqrt3}, // front vertices
      { inv_sqrt3,  inv_sqrt3, -inv_sqrt3},
      {-inv_sqrt3,  inv_sqrt3, -inv_sqrt3},
      {-inv_sqrt3, -inv_sqrt3, -inv_sqrt3},

      { inv_sqrt3,  inv_sqrt3,  inv_sqrt3}, // back vertices
      { inv_sqrt3, -inv_sqrt3,  inv_sqrt3},
      {-inv_sqrt3, -inv_sqrt3,  inv_sqrt3},
      {-inv_sqrt3,  inv_sqrt3,  inv_sqrt3}};

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
    
    //// Create geometry
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

    //// Instantiate geometry
    geometryPool = geBeginGeometryPool();
      //todo: ? OSobject frame = geBeginFrame();
        OSid instance = geAddGeometryInstance(geometry, GE_STATIC);
      //todo: ? geEndFrame();
    geEndGeometryPool();

    //// Create structure
    //OSid bihStructure = geGetStructure(GE_NATIVE, GE_BIH, geometryPool);
  }

  virtual void init()
  {
    //// Initialize engines
    //geInit();
    reInit();
    reDisplayMode(RE_PERSISTENT);
    gui = guiInit();

    //// Initialize data
    // Initialize geometry
    initializeGeometry();
    
    // Construct camera
    Cameraf cameraObject(Vector3f(2.5f, 5.5f, -5.5f));
    cameraObject.setPitchYaw((float)(-15.0/180.0*PI), (float)(10.0/180.0*PI));
    
    camera = reBeginCamera();
      reCameraPosition(cameraObject.getPosition());
      reCameraDirection(cameraObject.getOrientation());
      //reLens();
    reEndCamera();

    // Construct lights
    light = reBeginLight(RE_POINT_LIGHT);
      reLightPosition(5.0f, 10.0f, 5.0f);
    reEndLight();

    // Construct scene
    scene = reBeginScene();
      reGeometryPool(geometryPool);
      reLight(light);
    reEndScene();

    // Construct render
    render = reBeginRender(RE_FRAME_BUFFER, scene);
      reCamera(camera);
    reEndRender();

    //// Set debug breakpoints
#ifdef _DEBUG
    //redbgBreakRasterPosition(413, 194);
#endif

    ////// Render scene
    //reRender(render);
  }

  virtual void main()
  {
    // Render scene
    reRender(render);

    // Display render
    reDisplay();
    gui->display();
  }

  virtual void end()
  {
    //// Delete objects
    // Render objects
    reDelObject(camera);
    reDelObject(render);
    reDelObject(scene);

    // Geometry objects
    geDelObject(geometryPool);
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

    //// Shutdown engines
    reShutdown();
    geShutdown();
  }

  QRayRETest() : OpenREApp("QRayRE Test application. Copyright (c) 2006, Rehno Lindeque. All rights reserved.") {}
}application;

/*int main()
{
  application.init();
  while(application.main()) {}
  application.end();
  return 0;
}*/

/*                              COMPILER MACROS                             */
#pragma warning(pop)
