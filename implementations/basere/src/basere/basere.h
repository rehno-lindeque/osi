#ifndef __BASERE_BASERE_H__
#define __BASERE_BASERE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    BASERE.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Sample base classes for OpenRE backends.
*/
/*                              COMPILER MACROS                             */
#pragma warning(push)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)

/*                                 INCLUDES                                 */
// OpenRE
#include <osi/re/re.h>

// libstdc++
#include <vector>
#include <list>
#include <stack>

// Base
#include <base/common/types.h>
#include <base/common/functions.h>
#include <base/common/structures.h>
#include <base/common/object.h>
#include <base/math/math.h>

/*                          FORWARD DECLERATIONS                            */
namespace BaseRE
{
  class Object;
  class Scene;
  class Camera;
  class Light;
  class Render;

  template<class ObjectType> ObjectType* createObject();
  template<class ObjectType> ObjectType* beginObject();
  template<class ObjectType> ObjectType* endObject();
  template<class ObjectType> ObjectType* openObject(OSobject object);
  template<class ObjectType> ObjectType* closeObject();
  template<class ObjectType> ObjectType* getActiveObject();
  void delObject(Object* object);
  template<class ObjectType> ObjectType* cast_id(OSobject object);
  template<class ObjectType> OSobject cast_object(ObjectType* object);
  inline void shutdown();
}
/*                                 INCLUDES                                 */
// BaseRE headers
#include "object.h"
#include "scene.h"
#include "camera.h"
#include "light.h"
#include "render.h"

/*                                 GLOBALS                                  */
namespace BaseRE
{
  extern std::list<Object*> objects;
  extern std::stack<Object*> activeObjects;
  extern const float* indentityMatrix4;
}

/*                              IMPLEMENTATION                              */
#include "basere.hxx"

/*                              COMPILER MACROS                             */
#pragma warning(pop)

#endif
