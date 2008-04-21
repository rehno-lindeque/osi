#ifndef __BASERE_BASERE_H__
#define __BASERE_BASERE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    BASERE.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      Sample base classes for OpenRE backends.
    
    IMPLEMENTATION:
      + The REObject Class:
        An REObject is any object is any object that can be created via
        the standard functions: createObject, delObject, beginObject and endObject
        These almost always correspond with a (id-returning) reBegin\reEnd
        (Note: also see Issue 23 in the OpenRE specifications)
        block.

      + The Composite Class:
        A Composite is any type that may contain geometry as well as other
        Composites and certain other objects such as Cameras.
        Composites contain three types of objects:
          Local data (This is geometry that is directly contained and deleted with the composite)
          Child objects (These are child objects that are created inside and deleted with the Composite object)
          Child instances  (These are references to outside objects)
      
      + The standard functions
        delObject automatically deletes all sub-objects if the object is a
        composite.
*/
/*                              COMPILER MACROS                             */
#pragma warning(push)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)

/*                                 INCLUDES                                 */
// STL headers
#include <vector>
#include <list>
#include <stack>
#ifdef MSVC_BUILD
	#include <hash_map>
	using namespace stdext;	
#else
	#include <ext/hash_map>
	using namespace __gnu_cxx;
#endif
using namespace std;

// OpenRE header
#include <osi/re/re.h> 

// BaseRE headers
#include "common/types.h"
#include "math/math.h"

namespace BaseRE
{
/*                          FORWARD DECLERATIONS                            */
class REObject;
class Scene;
class Geometry;
class Component;
class Composite;

class Points;
class Vertices;
class Edges;
class Primitives;
class Map;

/*                                   TYPES                                  */
enum RE_OBJECT_TYPE
{
  ELEMENTS = 0x00000001,
  CAMERA,
  RENDER,

  GEOMETRY_TYPE = 0xd0000000,
  MESH_GEOMETRY,
  
  COMPONENT_TYPE = 0xe0000000,
  POINTS_COMPONENT,
  VERTICES_COMPONENT,
  EDGES_COMPONENT,
  PRIMITIVES_COMPONENT,
  MAP_COMPONENT,

  COMPOSITE_TYPE = 0xf0000000,
  FRAME,
  SCENE
};

enum State
{
  GLOBAL,
  SCENE_COMPOSITE,
  FRAME_COMPOSITE,
  TRIANGLE_OBJECT,
  TRIANGLES_OBJECT,
  QUAD_OBJECT,
  QUADS_OBJECT
};

/*                                 GLOBALS                                  */
extern stack<State>      state;           //todo remove (& use rtti instead)
extern stack<Composite*> activeComposite; //todo remove??? (or not: it might be needed)
extern list<REObject*>   reObjects;
extern stack<REObject*>  activeObject;
extern Geometry*         activeGeometry;
extern Component*        activeComponent;
extern Scene*            activeScene;
extern const float*      indentityMatrix4;

/*                                 CLASSES                                  */
class REObject
{
public:
  REuint type;
  REObject(REuint type) : type(type) {}
  //REObject() : type(0) {}
  virtual ~REObject() {}
};

class Geometry : public REObject
{
public:
  inline Geometry(REuint type) : REObject(type) {}
};

class Mesh : public Geometry
{
public:
  Points* points;
  Vertices* vertices;
  Edges* edges;
  Primitives* primitives;
  Map* map;

  static const RE_OBJECT_TYPE TYPE;
  inline Mesh() : Geometry(TYPE), points(null), vertices(null), edges(null), primitives(null), map(null) {}
};

/*class Triangles
{
};*/

/*old: class Quads
{
public:
  void* vertices,* indices;
  REuint nQuads, nVertices;
  REuint vertexStride, indexStride;
  
  inline void clear() { vertices = null; indices = null; nQuads = 0; nVertices = 0; vertexStride = 0; indexStride = 0; }
  
  //DEPRICATED: REuint primitiveStride;  
  inline uint16 getIndex(uint primitiveIndex, uint vertexIndex) const
  { 
    uint primitiveStride = 3*indexStride;
    return *(uint16*)((int8*)indices + primitiveIndex*primitiveStride + vertexIndex*indexStride);
  }
  inline const float* getVertex(uint primitiveIndex, uint vertexIndex) const
  { 
    if(indices)
      return (float*)((int8*)vertices + getIndex(primitiveIndex, vertexIndex)*vertexStride);
    else
    {
      //uint primitiveStride = 3*vertexStride;
      uint primitiveStride = 4*vertexStride;
      return (float*)((int8*)vertices + primitiveIndex*primitiveStride + vertexIndex*vertexStride);
    }
  }
};*/

class Elements : public REObject
{
public:
  static const RE_OBJECT_TYPE TYPE;
  inline Elements() : REObject(TYPE) {}
};

class Component : public REObject
{
public:
  hash_map<REenum, REid> attributes; //hash_map<RE_ATTRIBUTE, REid>

  inline Component(REuint type) : REObject(type) {}

  inline void addAttributes(RE_ATTRIBUTE attribute, REid elements)
  {
    attributes.insert(pair<RE_ATTRIBUTE, REid>(attribute, elements));
  }

  inline REid getAttributes(RE_ATTRIBUTE attribute)
  {
    //pair<RE_ATTRIBUTE, REid>& attrib = attributes[attribute];
    //return attrib.second;

    //hash_map<RE_ATTRIBUTE, REid>::iterator i = attributes.find(attribute);
    //if(i == attributes.end())
    //  return 0;
    //return i->second;
    
    return attributes[attribute];

  }
};

class Points : public Component
{
public:
  static const RE_OBJECT_TYPE TYPE;
  inline Points() : Component(TYPE) {}
};

class Vertices : public Component
{
public:
  static const RE_OBJECT_TYPE TYPE;
  inline Vertices() : Component(TYPE) {}
};

class Edges : public Component
{
public:
  static const RE_OBJECT_TYPE TYPE;
  inline Edges() : Component(TYPE) {}
};

class Primitives : public Component
{
public:
  static const RE_OBJECT_TYPE TYPE;
  inline Primitives() : Component(TYPE) {}
};

class Map : public Component
{
public:
  static const RE_OBJECT_TYPE TYPE;
  inline Map() : Component(TYPE) {}
};

class Composite : public REObject
{
public:
  Composite* parent;

  // Composite contents:
  // Geometry localGeometry; (Not here!)
  vector<REObject*> childObjects;
  vector<REObject*> childInstances;

  /*Depricated:
  inline void removeChildObject(Composite* child)
  {
    // Note: this removes all instances of the child from the composite
    for(vector<Composite*>::iterator i = children.begin(); i != children.end(); )
    {
      if(*i == child)
        i = children.erase(i);
      else
        ++i;
    }
  }*/
  
  inline void addChildObject(Composite* child)
  {
    childObjects.push_back(child);
  }
  
  inline Composite(REuint type, Composite* parent) : REObject(type), parent(parent) { /*NO LONGER NEEDED: if(parent) parent->addChild(this);*/ }
  virtual ~Composite()
  { //NO LONGER NEEDED: if(parent) parent->removeChild(this); (See issue 25)
    for(vector<REObject*>::iterator i = childObjects.begin(); i != childObjects.end(); ++i)
      reDelObject((REid)*i);
  }
  
  //old: virtual void addQuads(const Quads& quads) = 0;
};

class Frame : public Composite
{
protected:
  Matrix4 transformation;
  //Vector3 translation; ???
  //Vector3\Quaternion rotation; ???

public:
  static const RE_OBJECT_TYPE TYPE;
  

  //Frame() : Composite(TYPE, activeComposite.empty()?null:activeComposite.top()) {} (Wrong because there will always be a global composite
  inline Frame() : Composite(TYPE, activeComposite.top()), transformation(IDENTITY_MATRIX4) {}

  inline void translate(float x, float y, float z) { Matrix4 temp; transformation *= temp.setTranslation(x, y, z); }
  inline void rotate(float angle, float x, float y, float z) { Matrix4 temp; transformation *= temp.setAxisRotation(Vector3(x, y, z), angle); }
  inline const Matrix4& getTransformation() const { return transformation; }
};

class Scene : public Composite
{
public:
  list<Geometry*> geometry;

  static const RE_OBJECT_TYPE TYPE;
  inline Scene() : Composite(TYPE, null) {}
};

class Camera : public REObject
{
public:
  static const RE_OBJECT_TYPE TYPE;
  REid frame;
  bool referenceFrame;

  inline Camera() : REObject(TYPE), frame(null) {}
  void getTransform(Matrix4& transformation) const
  {
    //get parent frames
    stack<Frame*> frames;
    frames.push((Frame*)frame);
    Composite* composite = ((Frame*)frame)->parent;
    while(composite != null)
    {
      if(composite->type == FRAME)
        frames.push((Frame*)composite);
      composite = composite->parent;
    }

    //calculate transformation matrix
    Matrix4 trans(IDENTITY_MATRIX4);
    transformation = IDENTITY_MATRIX4;

    //temp: rotate camera around object:
    transformation *= trans.setRotationZ((float)(-25.0  * PI/180.0));
    transformation *= trans.setRotationX((float)(-75.0  * PI/180.0));
        
    //temp: rotate camera
    transformation *= trans.setTranslation(0.0f, 0.0f, -20.0f);
    transformation *= trans.setRotationY((float)(180.0 * PI/180.0));
    

   /* TODO
    while(!frames.empty())
    {
      transformation *= frames.top()->getTransformation();
      frames.pop();
    }*/
    transformation *= trans.setPerspectiveProjectionFOV((float)(45.0 * PI/180.0), 800.0f/600.0f, 0.1f, 100.0f);
  }
};

class Render : public REObject
{
public:
  static const RE_OBJECT_TYPE TYPE;
  const Camera* camera;
  const Scene*  scene;
  /*struct
  {
    RE_RENDER_PARAM type
  } *paramTypes;*/

  inline Render() : REObject(TYPE), camera(null), scene(null) {}
  
  /*??? void setParameter(RE_RENDER_PARAM type, REid param)
  {
    re
  }*/
};

/*                                FUNCTIONS                                 */
inline void delObject(REObject* obj)
{
  for(list<REObject*>::iterator i = reObjects.begin(); i != reObjects.end(); ++i)
    if(obj == *i) { delete obj; reObjects.erase(i); return; }
}

template<class REType> inline REType* createObject() { REType* obj = new REType(); reObjects.push_front((REObject*)obj); return obj; }

template<class REType> inline REType* beginObject() { REType* obj = createObject<REType>(); activeObject.push((REObject*)obj); return obj; }
template<class REType> inline void endObject() { activeObject.pop(); }

template<class REType> inline REType* beginComponent() { REType* obj = createObject<REType>(); activeObject.push((REObject*)obj); activeComponent = (Component*)obj; return obj; }
template<class REType> inline void endComponent() { activeObject.pop(); activeComponent = null; }

template<class REType> inline REType* beginGeometry() { REType* obj = createObject<REType>(); activeObject.push((REObject*)obj); activeGeometry = (Geometry*)obj; return obj; }
template<class REType> inline void endGeometry() { activeObject.pop(); activeGeometry = null; }
}

/*                              COMPILER MACROS                             */
#pragma warning(pop)

#endif
