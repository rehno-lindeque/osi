#ifndef __GLGE_GEOMETRYPOOL_H__
#define __GLGE_GEOMETRYPOOL_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GEOMETRYPOOL.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      GLGE GeometryPool

    TODO:
      + Clean up primitive \ component convention
*/

using BaseGE::cast_id;

namespace GLGE
{
/*                                  CLASSES                                 */
  class GLGeometryPool : public BaseGE::GeometryPool
  {
  protected:
    AxisAlignedBoundingBox boundingBox;

    inline OSid BIH_rebuild();
    inline OSid BIH_build();

  public:
    inline GLGeometryPool() {}
    
    inline ~GLGeometryPool() 
    {
      //todo: destroy structures
    }

    inline OSid getStructure(GE_HARDWARE_INTERFACE hardwareInterface, GE_SPATIAL_STRUCTURE structure, GE_STRUCTURE_LAYOUT structureLayout, GE_PRIMITIVE_LAYOUT primitiveLayout)
    {
      OSid spatialStructure = findStructure(hardwareInterface, structure);
      if(spatialStructure == 0)
      { 
        // create the structure
        switch(structure)
        {
        case GE_BIH:
          spatialStructure = BIH_build();
          break;
        }
      }
      else
      {
        // rebuild the structure
        switch(structure)
        {
        case GE_BIH:
          spatialStructure = BIH_rebuild();
          break;
        }
      }

      return spatialStructure;
    }

    inline const float* getBoundingBox() const { return (const float*)boundingBox; }
  };
}

/*                              IMPLEMENTATION                              */
#include "geometrypool.hxx"

#endif
