//////////////////////////////////////////////////////////////////////////////
//
//    BASERE.CPP
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*                                 INCLUDES                                 */
#ifdef _WIN32
#include <windows.h>
#endif

#include "basere.h"

using namespace BaseRE;
using namespace std;
/*                                 GLOBALS                                  */
stack<State>        BaseRE::state;
stack<Composite*>   BaseRE::activeComposite;
list<REObject*>     BaseRE::reObjects;
stack<REObject*>    BaseRE::activeObject;
Geometry*           BaseRE::activeGeometry = null;
Component*          BaseRE::activeComponent = null;
Scene*              BaseRE::activeScene = null;

const RE_OBJECT_TYPE Elements::TYPE     = ELEMENTS;
const RE_OBJECT_TYPE Frame::TYPE        = FRAME;
const RE_OBJECT_TYPE Scene::TYPE        = SCENE;

const RE_OBJECT_TYPE Mesh::TYPE         = MESH_GEOMETRY;

const RE_OBJECT_TYPE Points::TYPE       = POINTS_COMPONENT;
const RE_OBJECT_TYPE Vertices::TYPE     = VERTICES_COMPONENT;
const RE_OBJECT_TYPE Edges::TYPE        = EDGES_COMPONENT;
const RE_OBJECT_TYPE Primitives::TYPE   = PRIMITIVES_COMPONENT;
const RE_OBJECT_TYPE Map::TYPE          = MAP_COMPONENT;

const RE_OBJECT_TYPE Camera::TYPE       = CAMERA;
const RE_OBJECT_TYPE Render::TYPE       = RENDER;


/*                                  SOURCE                                  */

