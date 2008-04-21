#ifndef __OSI_RE_H__
#define __OSI_RE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    RE.H
//
//    Ver 0.3 (2006-12-27)
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*
    DESCRIPTION:
      Main OpenRE interface.

      Reference Frames:
        Certain RE objects automatically create their own parent frames by
        default. Examples of these are:
          + Cameras
          (todo: maybe Scenes)

      reInstance may instantiate the following objects
        + Cameras
        + Scenes

    USAGE:
      windows.h must be included before this header in a windows build

    TODO:
      Port to linux, MacOS, cygwin tools etc.
      Perhaps split another simpler api for sharing video memory between
      engines. (I.e. reElements etc)
*/
/*                                 INCLUDES                                 */
//#include <gl/gl.h>
#include "../os.h"

/*                                 CONSTANTS                                */
#define RE_VERSION 0.3

/*                                   TYPES                                  */
// basic types
/*enum RE_PRIMITIVE_TYPE //these values can be logically OR'ed together for certain methods
{
  RE_TRIANGLES = 1,
  RE_QUADS = 2,
  RE_SPHERES = 4,
  RE_PLANES = 8
};*/

/*enum RE_BASIC_TYPE
{
  RE_FLOAT,
  RE_DOUBLE, //doule precision
  RE_INT32,
  RE_INT16,
  RE_INT8,
  RE_UINT32,
  RE_UINT16,
  RE_UINT8,
  RE_VEC2_FLOAT,
  RE_VEC3_FLOAT,
  RE_VEC4_FLOAT,
};*/

enum RE_DISPLAY_MODE
{
  RE_PERSISTENT,
  RE_TEMPORARY,
  RE_INCREMENTAL //or perhaps RE_THREADED ?
};

enum RE_RENDER_TARGET
{
  RE_FRAME_BUFFER,
  //RE_TEXTURE,
  //RE_IMAGE_FILE
};

enum RE_PARAMETER
{
  RE_GLOBAL_ILLUMINATION,
  RE_LOCAL_ILLUMINATION
  //RE_GAMMA_CORRECTION?
};

enum RE_GEOMETRY
{
  RE_MESH_GEOMETRY
};

enum RE_ATTRIBUTE
{
  RE_LOCATION,
  RE_INDEX,
  RE_COLOR,
  RE_MAP_COORDINATE,
  RE_NORMAL,

  RE_EXT_ATTRIBUTE = 0x80000000,
  RE_EXT_BENT_NORMAL_AREA // bent normal and disc area
};

enum RE_LIGHT
{
  RE_POINT_LIGHT
};

typedef OSuint RErc;

struct REParameterInfo
{
  OSid   parameter;
  OSuint maxQuality;
  OSuint defaultQuality;
  OSstring description;
};

struct RESettingInfo
{
  OSuint setting;
  OSstring description;
};

/*                                     API                                  */
/* scene */
OSobject reBeginScene();
void reEndScene();
//void reDelScene(REid scene);

void reGeometryPool(OSobject geometryPool);

/* frame */
/*REid reBeginFrame();
void reEndFrame();*/
//void reDelFrame(REid frame);

//todo void reRotate(...);
//todo void reScale(...);
//todo void reTransform(float* matrix);
//todo void reVisible(bool visibility);

// TODO: Extension - sphere
// REid reBeginSphere();
// void reRadius(OSfloat radius);



/*/* elements *
REid reElements(RE_BASIC_TYPE type, OSuint nElements, void* elements);
REid reBeginElements(RE_BASIC_TYPE type, OSuint nElements);
  void reElement(void* element);

  void reElementf(OSfloat);
  void reElement2f(OSfloat, OSfloat);
  void reElement3f(OSfloat, OSfloat, OSfloat);
  void reElement4f(OSfloat, OSfloat, OSfloat, OSfloat);

  void reElementd(REdouble);
  void reElement2d(REdouble, REdouble);
  void reElement3d(REdouble, REdouble, REdouble);
  void reElement4d(REdouble, REdouble, REdouble, REdouble);

  void reElementui8(OSuint8);
  void reElement2ui8(OSuint8, OSuint8);
  void reElement3ui8(OSuint8, OSuint8, OSuint8);
  void reElement4ui8(OSuint8, OSuint8, OSuint8, OSuint8);

  void reElementi8(REint8);
  void reElement2i8(REint8, REint8);
  void reElement3i8(REint8, REint8, REint8);
  void reElement4i8(REint8, REint8, REint8, REint8);

  void reElementui16(OSuint16);
  void reElement2ui16(OSuint16, OSuint16);
  void reElement3ui16(OSuint16, OSuint16, OSuint16);
  void reElement4ui16(OSuint16, OSuint16, OSuint16, OSuint16);

  void reElementi16(REint16);
  void reElement2i16(REint16, REint16);
  void reElement3i16(REint16, REint16, REint16);
  void reElement4i16(REint16, REint16, REint16, REint16);

  void reElementui32(OSuint32);
  void reElement2ui32(OSuint32, OSuint32);
  void reElement3ui32(OSuint32, OSuint32, OSuint32);
  void reElement4ui32(OSuint32, OSuint32, OSuint32, OSuint32);

  void reElementi32(REint32);
  void reElement2i32(REint32, REint32);
  void reElement3i32(REint32, REint32, REint32);
  void reElement4i32(REint32, REint32, REint32, REint32);

  void reElementui64(OSuint64);
  void reElement2ui64(OSuint64, OSuint64);
  void reElement3ui64(OSuint64, OSuint64, OSuint64);
  void reElement4ui64(OSuint64, OSuint64, OSuint64, OSuint64);

  void reElementi64(REint64);
  void reElement2i64(REint64, REint64);
  void reElement3i64(REint64, REint64, REint64);
  void reElement4i64(REint64, REint64, REint64, REint64);
void reEndElements();

OSuint reGetElementsLength(REid elements);
//OSuint reGetElementsSize(REid elements);

/* geometry *
REid reBeginGeometry(RE_GEOMETRY type);
void reEndGeometry();

/* mesh geometry *
REid reBeginPoints(OSuint nPoints);
void reEndPoints();
REid reBeginVertices(OSuint nVertices);
void reEndVertices();
REid reBeginEdges(OSuint nEdges);
void reEndEdges();
REid reBeginPrimitives(OSuint nPrimitives);
void reEndPrimitives();
REid reBeginMap();
void reEndMap();

//todo: void reAttribute(RE_ATTRIBUTE attribute, void* attribute);
void reAttributes(RE_ATTRIBUTE attribute, REid elements); //reAttributeElements?

/* primitives *
void reBeginTriangles();
void reEndTriangles();
void reBeginQuads();
void reEndQuads();
//todo: void reBeginPolygons();
//todo: void reEndPolygons();

/* primitive sub-components *
void rePoints(REid points, OSuint offset = 0, OSuint length = 0);
void reVertices(REid vertices, OSuint offset = 0, OSuint length = 0);
void reEdges(REid edges, OSuint offset = 0, OSuint length = 0);
void rePrimitives(REid primitives, OSuint offset = 0, OSuint length = 0);
void reMap(REid map);

  //todo: void reAttributes(REid elements);

  /*REid points = reBeginPoints(6);
        reVertices(vertices, offset=0, length=0);
        reColors(colors, offset=0, length=0)
      reEndPoints();
  REid primitives = reBeginPrimtives();
        reIndices(indices, offset=0, length=0);
        reNormals(normals, offset=0, length=0);
      reEndPrimitives();
      reBeginTriangles(2);
    rePoints(points);

    rePrimitives();
    reEndTriangles();*/



/* triangles */
/*void reBeginTriangle();
void reEndTriangle();
void reBeginTriangles(OSuint ntriangles);
void reEndTriangles();

/* quads *
void reBeginQuad();
void reEndQuad();
void reBeginQuads(OSuint nquads);
void reEndQuads();

/* polygons *
//todo void reBeginPolygon();
//todo void reEndPolygon();
//todo void reBeginPolygons(OSuint nquads);
//todo void reEndPolygons();
*/

/* geometry parameters */
//void reVertexV(OSfloat* vertex);
//void reVerticesV(OSfloat* vertices, OSuint vertexStride = 0, OSuint primitiveStride = 0);
//void reVerticesV(OSfloat* vertices, OSuint nVertices = 0, OSuint vertexStride = 0);
//void reIndexV(OSuint16 index);
//void reIndicesV(OSuint16* indices, OSuint indexStride = 0);
/*void reColorV(OSfloat* color);
void reColorP(OSfloat* color);
void reColorsV(OSfloat* color, OSuint vertexStride = 0, OSuint primitiveStride = 0);
void reColorsP(OSfloat* color, OSuint primitiveStride = 0);
void reNormalV(OSfloat* normal);
void reNormalP(OSfloat* normal);
void reNormalsV(OSfloat* normals, OSuint vertexStride = 0, OSuint primitiveStride = 0);
void reNormalsP(OSfloat* normals, OSuint primitiveStride = 0);*

/* Bake settings */
/* todo
void reBakeEnable(RE_AMBIENT_OCCLUSION | RE_RADIOSITY | RE_PHOTONMAP | RE_TEXTURES)
void reBakeTarget(RE_AMBIENT_OCCLUSION, RE_FRAMEBUFFER\FILE\etc)
void reBakeCoordinates(RE_AMBIENT_OCCLUSION, RE_UV0\RE_AUTO_GENERATE);
*/

/* camera */
OSobject reBeginCamera(OSbool referenceFrame = true);
void reEndCamera();

void reCameraPosition(OSfloat x, OSfloat y, OSfloat z);
void reCameraPosition(const OSfloat* position);
void reCameraDirection(const OSfloat* direction);

/* light */
OSobject reBeginLight(RE_LIGHT type);
void reEndLight();

void reLightPosition(OSfloat x, OSfloat y, OSfloat z);
void reLightPosition(const OSfloat* position);
void reLightDirection(const OSfloat* direction);

/* transformation */
//void rePosition(OSfloat x, OSfloat y, OSfloat z);
//void rePosition(const OSfloat* position);
//void reDirection(const OSfloat* direction);

/*void reTranslate(OSfloat x, OSfloat y, OSfloat z);
void reTranslateV(const OSfloat* translation);
/*void reSetTranslationV(float* translation);
void reRotateQ(OSfloat* quaternion);*
void reSetRotationQ(const OSfloat* quaternionRotation);*/

/* color */
void reColor(OSfloat r, OSfloat g, OSfloat b, OSfloat a = 1.0f);
void reColor(OSfloat* color);

/* render */
OSobject reBeginRender(RE_RENDER_TARGET renderTarget, OSobject scene);
void reEndRender();

//old: void reRenderTarget(RE_FRAMEBUFFER);

void reBackgroundColor(OSfloat* color);
void reCamera(OSobject camera);
void reLight(OSobject light);

/* todo:
void reBackgroundColor(OSfloat r, OSfloat g, OSfloat b);
void reBackgroundTexture(REid texture);
void reBackgroundCubemap(REid cubemap);
void reBackgroundSpheremap(REid texture (map?));
void reRenderEnable(RE_AMBIENT_OCCLUSION | RE_RADIOSITY | RE_PHOTONMAP)
void reRenderTarget(RE_AMBIENT_OCCLUSION, RE_FRAMEBUFFER\FILE\etc)*/

/* global parameters */
//todo: create a reBeginInit()\reEndInit() block
void reParameter(RE_PARAMETER type, OSid param); // to clear a parameter, set param to 0
void reQuality(RE_PARAMETER type, OSid param, OSfloat quality); //(Note: default quality = -1 \ -1.0)
void reParameterQuality(RE_PARAMETER type, OSid param, OSint quality);
void reParameterQuality(RE_PARAMETER type, OSid param, OSfloat quality);

/* queries */
const REParameterInfo*  reGetParameters(RE_PARAMETER type, OSint& num);
const RESettingInfo*    reGetParameterQualitySettings(RE_PARAMETER type, OSid param, OSint& num);

/* misc operations */
void reRender(OSobject render);
void reDebugRender(OSobject object, OSuint flag);
RErc reCopyRenderContext(RErc originalRC);
void reInit();
void reShutdown();
void reDisplayMode(RE_DISPLAY_MODE displayMode);
void reDisplay();
void reDelObject(OSobject object);
void reInstance(OSobject object);
//void reDuplicate(OSobject object);

/* windows */
#ifdef _WIN32
  RErc wreCreateRenderContext(HWND hwnd);
  //todo: RErc wreCopyOpenGLRenderContext(GLuint rc);
  //todo: RErc wreCopyDeviceContext(HDC dc);
#endif

#endif
