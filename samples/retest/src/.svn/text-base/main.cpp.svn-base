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
      A OpenRE test application. This is intended as a common test harness
      for development of the OpenRE backend engines.
*/
/*                                  INCLUDES                                */

#include <windows.h>

/* Common*/
#include <common/types.h>
#include <common/time/time.h>
#include <common/log/logfile.h>
#include <math.h>

/* OpenRE */
//#include <OpenRE/re.h>
#include <osi/re/re.h>
#include <rewrapper/openreapplication.h>

// #define USE_GLRAYRE
#define USE_GLRASRE
#if defined(USE_GLRAYRE)
  //#include <OpenRE/../GlRayRE/glrayre.h>
  #include <osi/re/ext/glrayre.h>
#pragma lib
#elif defined(USE_GLRASRE)
  //#include <OpenRE/../GlRasRE/glrasre.h>
  #include <osi/re/ext/glrasre.h>
#endif

using namespace REWrapper;

/*                                  CLASSES                                 */
class RETest : public OpenREApp
{
protected:
  LogFile log;
  Time timer;
  uint64 startTime, endTime;
  //void* renderBuffer;

public:
  void buildScene()
  {

#pragma warning(push)
#pragma warning(disable: 4305)
    /*float q[][3] = {
      {-1.0f, 1.0f, 0.0f },
      { 1.0f, 1.0f, 0.0f },
      { 1.0f,-1.0f, 0.0f },
      {-1.0f,-1.0f, 0.0f } };*/
    float height = 1.0f;

    //old:
    /*float v[][3] = {
      {6.000000,5.999999,0.000000}, //floor
      {-5.999998,6.000002,0.000000},
      {-6.000001,-5.999999,0.000000},
      {6.000000,-6.000001,0.000000},

      {1.000000,1.000000,0.000000+height}, //bottom
      {1.000000,-1.000000,0.000000+height},
      {-1.000000,-1.000000,0.000000+height},
      {-1.000000,1.000000,0.000000+height},

      {1.000000,0.999999,2.000000+height}, //top
      {-1.000000,1.000000,2.000000+height},
      {-1.000000,-1.000000,2.000000+height},
      {0.999999,-1.000001,2.000000+height},

      {1.000000,1.000000,0.000000+height}, //right
      {1.000000,0.999999,2.000000+height},
      {0.999999,-1.000001,2.000000+height},
      {1.000000,-1.000000,0.000000+height},

      {1.000000,-1.000000,0.000000+height}, //front
      {0.999999,-1.000001,2.000000+height},
      {-1.000000,-1.000000,2.000000+height},
      {-1.000000,-1.000000,0.000000+height},

      {-1.000000,-1.000000,0.000000+height}, //left
      {-1.000000,-1.000000,2.000000+height},
      {-1.000000,1.000000,2.000000+height},
      {-1.000000,1.000000,0.000000+height},

      {1.000000,0.999999,2.000000+height}, //back
      {1.000000,1.000000,0.000000+height},
      {-1.000000,1.000000,0.000000+height},
      {-1.000000,1.000000,2.000000+height}};*/

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
#pragma warning(pop)
    /*uint16 i[] = {
      0,  1,  2,  3,  // floor

      4,  5,  6,  7,  // front face
      8,  5,  4,  9,  // left face
      8,  9, 10, 11,  // back face
      6, 11, 10,  7,  // right face
      8, 11,  6,  5,  // top face
      4,  7, 10,  9 }; // bottom face*/

    uint16 i[] = {
      0,  1,  3,  1,  2,  3,   // floor

      4,  5,  7,  5,  6,  7,   // front face
      8,  5,  9,  5,  4,  9,   // left face
      8,  9, 11,  9, 10, 11,   // back face
      6, 11,  7, 11, 10,  7,   // right face
      8, 11,  5, 11,  6,  5,   // top face
      4,  7,  9,  7, 10,  9 }; // bottom face

    REid geometry = reBeginGeometry(RE_MESH_GEOMETRY);
      REid locations = reElements(RE_VEC3_FLOAT, 12, v);
      REid indices = reElements(RE_UINT16, 42, i);      
      REid normals = reElements(RE_VEC3_FLOAT, 12, n);

      REid points = reBeginPoints(12);
        reAttributes(RE_LOCATION, locations);
        reAttributes(RE_NORMAL,   normals);
      reEndPoints();

      REid vertices = reBeginVertices(42);
        reAttributes(RE_INDEX, indices);
      reEndVertices();

      reBeginTriangles();
        rePoints(points);
        reVertices(vertices);
      reEndTriangles();
    reEndGeometry();

    /* VERY OLD:
    reBeginQuads(1); //draw quad
      reVerticesV(*q);
    reEndQuads();*/

    /* OLD:
    reBeginQuads(1); //draw floor
      reVerticesV(*v, 1*4);
    reEndQuads();

    reBeginQuads(6); //draw cube
      reVerticesV(*(v+4), 6*4);
    reEndQuads();*/
  }

  virtual void init()
  {
    log << "OpenRE Test log\n";
    log << "---------------\n";
    log << "Scene setup time: ";

    startTime = timer.getCounter();

    /*glClear(GL_DEPTH_BUFFER_BIT);
    glFlush();

    reInit();
    REuint view = reGenView();
    reBeginView(view);
      reBegin(RE_SPHERES);
        rePosition3f(0.0f, 0.0f, 5.0f);
      reEnd();
    reEndView();
    reRender(view);
    reDelView(view);
    #ifdef USE_RAYRE
      renderBuffer = rayreGetRenderBuffer();
    #endif*/

    // Initialize OpenRE
    reInit();
    reDisplayMode(RE_PERSISTENT);

    // Construct scene
    REid scene = reBeginScene();
      REid camera = reBeginCamera();
        //reTranslate(-1.0f, -10.0f, 0.0f);
        reTranslate(0.0f, 0.0f, -10.0f);
        //reLens();
      reEndCamera();
      buildScene();
    reEndScene();

    // Construct render
    REid render = reBeginRender(RE_FRAME_BUFFER, scene);
      //depricated: reCamera(camera);
      //idea: reInstance(scene);
      //idea: reInstance(camera);
      reCamera(camera);
    reEndRender();

    // Render scene
    reRender(render);

    reDelObject(camera);
    reDelObject(render);
    reDelObject(scene);
    reDisplay();
  }

  virtual void main()
  {
    /*glClear(GL_DEPTH_BUFFER_BIT);

    //display render buffer
    glRasterPos2i(-400,-300);
    glDrawPixels(800, 600, GL_RGBA, GL_UNSIGNED_BYTE, renderBuffer);
    glFlush();*/
    reDisplay();
  }

  virtual void end()
  {
    reShutdown();
  }

  RETest() : OpenREApp("OpenRE Test application. Copyright (c) 2006, Rehno Lindeque. All rights reserved."), log("test1.log") {}
}application;
