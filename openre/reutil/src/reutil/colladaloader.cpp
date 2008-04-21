//////////////////////////////////////////////////////////////////////////////
//
//    COLLADALOADER.CPP
//
//    Copyright © 2006 Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                                 INCLUDES                                 */
// REUtil
#include "colladaloader.h"

using namespace REUtil;

// TemplateXML
#include "templatexml/parse/parse.h"
#include "templatexml/xmlelement.h"

namespace ColladaTemplate{ struct Source; }

namespace TemplateXML { template<> struct XmlElementAddition<ColladaTemplate::Source> { REid elementsId; }; }

#include "templatexml/templates/colladatemplate.h"
#include "templatexml/xmlreader.h"

using namespace TemplateXML;

// Standard Template Library
#include <vector>
using namespace std;

/*                              GLOBAL SOURCE                               */
REid createElementsFromColladaSource(ColladaTemplate::Source* source)
{
  using namespace ColladaTemplate;

  Source_TechniqueCommon* technique;
  Accessor* accessor;
  FloatArray* floatArray;

  // return the elements if it has already been created
  if(source->elementsId != null)
    return source->elementsId; 

  // check parameters
  technique = source->technique;
  if(technique == null)
    return 0;
  
  accessor = technique->accessor;
  if(accessor == null)
    return 0;
  
  floatArray = accessor->source;
  if(floatArray == null)
    return 0;

  // create elements
  source->elementsId = reElements(RE_VEC3_FLOAT, floatArray->contents.length / 3, (float*)floatArray->contents);
  return source->elementsId;
}

void loadTriangles(ColladaTemplate::Triangles* triangles)
{
  using namespace ColladaTemplate;

  Input* input;
  Source* positionSource;
  Source* normalSource;
  FloatArray* floatArray;
  XmlArray<int>* p;

  Vertices* vertices;

  input = triangles->input;
  if(input == null)
    return;

  switch(input->semantic)
  {
  case VERTEX:
    vertices = (Vertices*)input->source;
    break;
  }

  if(vertices == null)
    return;

  // get positions and normals
  for(int cInput = 0; cInput < vertices->input.length; ++cInput)
  {
    input = &vertices->input[cInput];
    if(input == null)
      continue;

    switch(input->semantic)
    {
    case POSITION:
      positionSource = (Source*)input->source;
      break;
    case NORMAL:
      normalSource = (Source*)input->source;
      break;
    }
  }

  // create location elements
  if(positionSource == null)
    return;
  REid locationElements = createElementsFromColladaSource(positionSource);
  if(locationElements == 0)
    return;
  
  // create normal elements
  /* todo:
  REid normalElements = 0;
  if(normalSource != null)
    normalElements = createElementsFromColladaSource(normalSource);*/

  // create points
  REid points;
  {
    // calculate number of points
    floatArray = positionSource->floatArray;
    uint nPoints = floatArray->contents.length / 3;
    
    points = reBeginPoints(nPoints);
      reAttributes(RE_LOCATION, locationElements);
      /* todo:
      if(normalSource != null)
        reAttributes(RE_NORMAL, normalElements);*/
    reEndPoints();
  }

  // create index elements
  REid indexElements;
  uint nIndices = 0;
  {
    // copy indices
    uint16* data = null;
    {
      // calculate number of indices
      for(int cP = 0; cP < triangles->p.length; ++cP)
      {
        p = &triangles->p[cP];
        if(p == null)
          continue;
        
        nIndices += p->length;
      }

      // copy indices and convert to uint16
      data = new uint16[nIndices];
      uint16* dataPos = data;
      for(int cP = 0; cP < triangles->p.length; ++cP)
      {
        p = &triangles->p[cP];
        if(p == null)
          continue;
        for(int cData = 0; cData < p->length; ++cData)
        {
          *dataPos = (uint16)((*p)[cData]);
          ++dataPos;
        }
      }
    }

    if(data == null)
      return;

    indexElements = reElements(RE_UINT16, nIndices, (uint16*)data);

    // deallocate indices
    delete[] data;
  }

  {
    REid vertices = reBeginVertices(nIndices);
      reAttributes(RE_INDEX, indexElements);
    reEndVertices();

    reBeginTriangles();
      rePoints(points);
      reVertices(vertices);
    reEndTriangles();
  }
}

void loadPolygons(ColladaTemplate::Polygons* polygons)
{
  using namespace ColladaTemplate;

  Input* input;
  Source* positionSource;
  Source* normalSource;
  FloatArray* floatArray;
  XmlArray<int>* p;

  Vertices* vertices;

  input = polygons->input;
  if(input == null)
    return;

  switch(input->semantic)
  {
  case VERTEX:
    vertices = (Vertices*)input->source;
    break;
  }

  if(vertices == null)
    return;

  // get positions and normals
  for(int cInput = 0; cInput < vertices->input.length; ++cInput)
  {
    input = &vertices->input[cInput];
    if(input == null)
      continue;

    switch(input->semantic)
    {
    case POSITION:
      positionSource = (Source*)input->source;
      break;
    case NORMAL:
      normalSource = (Source*)input->source;
      break;
    }
  }

  // create location elements
  if(positionSource == null)
    return;
  REid locationElements = createElementsFromColladaSource(positionSource);
  if(locationElements == 0)
    return;
  
  // create normal elements
  /* todo:
  REid normalElements = 0;
  if(normalSource != null)
    normalElements = createElementsFromColladaSource(normalSource);*/

  // create points
  REid points;
  {
    // calculate number of points
    floatArray = positionSource->floatArray;
    uint nPoints = floatArray->contents.length / 3;
    
    points = reBeginPoints(nPoints);
      reAttributes(RE_LOCATION, locationElements);
      /* todo:
      if(normalSource != null)
        reAttributes(RE_NORMAL, normalElements);*/
    reEndPoints();
  }

  // create index elements
  REid indexElements;
  uint nIndices = 0;
  {
    // copy indices
    uint16* data = null;
    {
      // calculate number of indices
      for(int cP = 0; cP < polygons->p.length; ++cP)
      {
        p = &polygons->p[cP];

        if(p == null)
          continue;  

        // skip non-quads (todo: load arbitrary polygons)
        if(p->length != 4)
          continue;        

        nIndices += p->length;
      }

      // copy indices and convert to uint16
      data = new uint16[nIndices];
      uint16* dataPos = data;
      for(int cP = 0; cP < polygons->p.length; ++cP)
      {
        p = &polygons->p[cP];
        if(p == null)
          continue;
        
        // skip non-quads (todo: load arbitrary polygons)
        if(p->length != 4)
          continue; 

        for(int cData = 0; cData < p->length; ++cData)
        {
          *dataPos = (uint16)((*p)[cData]);
          ++dataPos;
        }
      }
    }

    if(data == null)
      return;

    indexElements = reElements(RE_UINT16, nIndices, (uint16*)data);

    // deallocate indices
    delete[] data;
  }

  {
    REid vertices = reBeginVertices(nIndices);
      reAttributes(RE_INDEX, indexElements);
    reEndVertices();

    reBeginQuads();
      rePoints(points);
      reVertices(vertices);
    reEndQuads();
  }
}

REid loadMeshGeometry(ColladaTemplate::Mesh* mesh)
{
  using namespace ColladaTemplate;

  Triangles* triangles = mesh->triangles;
  Polygons* polygons = mesh->polygons;
  if(triangles == null && polygons == null)
    return 0;
  
  REid geometry = reBeginGeometry(RE_MESH_GEOMETRY);

  // add triangles
  // todo: loop at triangles?
  if(triangles != null)
    loadTriangles(triangles);
  
  // add polygons
  // todo: loop at polygons?
  /* todo: temporarily removed
  if(polygons != null)
    loadPolygons(polygons);
  */

  reEndGeometry();
  return geometry;
}

REid loadScene(vector<REid>& meshes, vector<REid>& cameras, ColladaTemplate::VisualScene* visualScene)
{
  using namespace ColladaTemplate;

  Node* node;
  InstanceGeometry* instanceGeometry;
  Geometry* geometry;
  Mesh* mesh;
    
  REid scene = reBeginScene();
     
  for(int cNode = 0; cNode < visualScene->node.length; ++cNode)
  {
    node = &visualScene->node[cNode];
    instanceGeometry = node->instanceGeometry;
    if(instanceGeometry == null)
      continue;

    geometry = instanceGeometry->ref;
    if(geometry == null)
      continue;
    
    mesh = geometry->mesh;
    if(mesh == null)
      continue;

    REid geometry = loadMeshGeometry(mesh);
    if(geometry != 0)
      meshes.push_back(geometry);
  }
  
  // add cameras
  {
    cameras.push_back(reBeginCamera());
      reTranslate(0.0f, 0.0f, -10.0f);
    reEndCamera();
  }

  reEndScene();
  return scene;
}

/*                               CLASS SOURCE                               */

ColladaLoader::ColladaLoader() : nScenes(0), nMeshes(0), scenes(null), meshes(null), loadOption(LOAD_EVERYTHING)
{}

ColladaLoader::~ColladaLoader()
{
  destroyData();
}

ColladaLoader* ColladaLoader::create()
{
  return new ColladaLoader();
}

void ColladaLoader::load(const char* fileName, LoadOption loadOption)
{
  ColladaLoader::loadOption = loadOption;
  load(fileName);
}

void ColladaLoader::load(const char* fileName)
{
  using namespace ColladaTemplate;

  Collada* collada;
  Scene* scene;
  InstanceVisualScene* instanceVisualScene;
  LibraryVisualScenes* libraryVisualScenes;
  VisualScene* visualScene;

  vector<REid> scenes;
  vector<REid> meshes;
  vector<REid> cameras;
  
  XmlReader<Collada> fileReader;

  fileReader.loadFile(fileName, collada);
  if(collada == null)
    return; //error

  switch(loadOption)
  {
    case LOAD_EVERYTHING:
      //todo
      //break;
    case LOAD_SCENES:
      libraryVisualScenes = collada->libraryVisualScenes;
      if(libraryVisualScenes == null)
        break;

      for(int cVisualScene = 0; cVisualScene < collada->libraryVisualScenes.length; ++cVisualScene)
      {
        visualScene = &libraryVisualScenes->visualScene[cVisualScene];
        if(visualScene == null)
          break;

        REid scene = loadScene(meshes, cameras, visualScene);
        if(scene != 0)
          scenes.push_back(scene);
      }
      break;
    case LOAD_MAIN_SCENE:
      scene = collada->scene;
      if(scene == null)
        break;
      
      instanceVisualScene = scene->instanceVisualScene;
      if(instanceVisualScene == null)
        break;
      
      visualScene = instanceVisualScene->ref;
      if(visualScene == null)
        break;

      REid scene = loadScene(meshes, cameras, visualScene);
      if(scene != 0)
        scenes.push_back(scene);
      break;
  }
  
  // destroy file data
  fileReader.destroyData(collada);

  // copy OpenRE id's
  if(scenes.size() > 0)
  {
    int c = 0;
    nScenes = (uint)scenes.size();
    ColladaLoader::scenes = new REid[nScenes];
    for(vector<REid>::iterator iScene = scenes.begin(); iScene != scenes.end(); ++iScene, ++c)
      ColladaLoader::scenes[c] = *iScene;
  }

  if(meshes.size() > 0)
  {
    int c = 0;
    nMeshes = (uint)meshes.size();
    ColladaLoader::meshes = new REid[nMeshes];
    for(vector<REid>::iterator iMesh = meshes.begin(); iMesh != meshes.end(); ++iMesh, ++c)
      ColladaLoader::meshes[c] = *iMesh;
  }

  if(cameras.size() > 0)
  {
    int c = 0;
    nCameras = (uint)cameras.size();
    ColladaLoader::cameras = new REid[nCameras];
    for(vector<REid>::iterator iCamera = cameras.begin(); iCamera != cameras.end(); ++iCamera, ++c)
      ColladaLoader::cameras[c] = *iCamera;
  }
}


void ColladaLoader::destroyObjects()
{
  // destroy OpenRE Objects
  // destroy scenes
  for(uint c = 0; c < nScenes; ++c)
    reDelObject(scenes[c]);

  //todo: is this necessary?

  // destroy geometry
  for(uint c = 0; c < nMeshes; ++c)
    reDelObject(meshes[c]);

  // destroy cameras
  for(uint c = 0; c < nCameras; ++c)
    reDelObject(cameras[c]);

  // destroy all file data
  destroyData();
}

void ColladaLoader::destroyData()
{
  delete[] scenes;
  scenes = null;
  delete[] meshes;
  meshes = null;
  delete[] cameras;
  cameras = null;
}
