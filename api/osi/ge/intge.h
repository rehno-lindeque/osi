#ifndef __OSI_INTGE_H__
#define __OSI_INTGE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    INTGE.H
//
//    Ver 0.1 (2006-09-02)
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*
    DESCRIPTION:
      Internal OpenGE interface.

    DEPENDENCIES:
      ge.h must be included before this file
*/
/*                                   TYPES                                  */
enum GE_PRIORITY
{
  GE_LOW = 0,
  GE_MEDIUM_LOW,
  GE_MEDIUM,
  GE_MEDIUM_HIGH,
  GE_HIGH,
  GE_ABSOLUTE
};

enum GE_HARDWARE_INTERFACE
{
  GE_NATIVE,
  GE_GPU
  //GE_RPU
};

enum GE_SPATIAL_STRUCTURE
{
  GE_FLAT,
  GE_BIH,
  GE_EXTENDED_BIH,
  GE_BVH,
  GE_KDTREE
};

enum GE_STRUCTURE_LAYOUT
{
  GE_BIH_FLAT,
  GE_BIH_TREE
};

enum GE_PRIMITIVE_LAYOUT
{
  GE_VEC3_POINTS
  //GE_
};

/*                                    API                                   */

/*// Spatial structures
GEid geBeginGeometryPool();
void geEndGeometryPool();
void geGeometryInstance(GEid geometry, void* extraInfo);

void geRequestSpatialStructure(GE_SPATIAL_STRUCTURE spatialStructure, GE_PRIORITY priority);
GE_SPATIAL_STRUCTURE geGetSpatialStructure(GEid geometryPool);
/*void geBeginGeometryModify();
void geEndGeometryModify();*/
/*GEid geLockSpatialStructure(GEid geometryPool);
geUnlockSpatialStructure(GEid geometryPool);*
GEid geRetrieveSpatialStructure(GEid geometryPool);

/*void* geMapSpatialStructure(GEid spatialStructure);
void geUnmapSpatialStructure(GEid spatialStructure);*/


//void geModifySize();

//OSid geRequestStructure(GE_HARDWARE_INTERFACE hardwareInterface, GE_SPATIAL_STRUCTURE structure, OSid geometryPool);
OSid geGetStructure(OSobject geometryPool, GE_HARDWARE_INTERFACE hardwareInterface, GE_SPATIAL_STRUCTURE structure, GE_STRUCTURE_LAYOUT structureLayout, GE_PRIMITIVE_LAYOUT primitiveLayout);
const float* geGetBoundingBox(OSobject geometryPool);

//geMapElements???

#endif
