//////////////////////////////////////////////////////////////////////////////
//
//    INTAPI.CPP
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      Internal API implementation.
*/
/*                              COMPILER MACROS                             */
#pragma warning(push)

/*                                 INCLUDES                                 */
#include "api.h"

using namespace GLGE;
using namespace BaseGE;
/*                                  GLOBALS                                 */

/*                               IMPLEMENTATION                             */
OSid geGetStructure(OSobject geometryPool, GE_HARDWARE_INTERFACE hardwareInterface, GE_SPATIAL_STRUCTURE structure, GE_STRUCTURE_LAYOUT structureLayout, GE_PRIMITIVE_LAYOUT primitiveLayout)
{
  GLGeometryPool* geometryPoolObject = cast_id<GLGeometryPool>(geometryPool);
  return geometryPoolObject->getStructure(hardwareInterface, structure, structureLayout, primitiveLayout);
}

const float* geGetBoundingBox(OSobject geometryPool)
{
  GLGeometryPool* geometryPoolObject = cast_id<GLGeometryPool>(geometryPool);
  return geometryPoolObject->getBoundingBox();
}

/*                              COMPILER MACROS                             */
#pragma warning(pop)
