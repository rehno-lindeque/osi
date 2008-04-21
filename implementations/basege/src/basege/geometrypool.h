#ifndef __BASEGE_GEOMETRYPOOL_H__
#define __BASEGE_GEOMETRYPOOL_H__
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
      GeometryPool base.

    TODO:
      + Review performance of instance containers. Do we need fast insertion\traversal time or fast removal?
*/

namespace BaseGE
{
/*                                  CLASSES                                 */
  class GeometryPool : public Base::Object
  {
  protected:
    class GeometryInstance
    {
    protected:
      GE_GEOMETRY_STABILITY stability;
      Geometry* geometry;

    public:
      inline GeometryInstance(Geometry* geometry, GE_GEOMETRY_STABILITY stability) : geometry(geometry), stability(stability)
      {}

      inline OSuint countPrimitives()
      {
        return geometry->countPrimitives();
      }

      inline AxisAlignedBoundingBox calculateBoundingBox()
      {
        //todo: optimize
        //todo: apply transformations to calculate instance bounding box if it is dynamic.
        return geometry->getBoundingBox();
      }

      inline Geometry* getGeometry() { return geometry; }
    };

    //typedef std::pair<Geometry*, GE_GEOMETRY_STABILITY> GeometryInstance;
    typedef std::multimap<Geometry*, GeometryInstance*> GeometryInstanceMap;

    GeometryInstanceMap staticInstances;
    GeometryInstanceMap dynamicStableInstances;
    GeometryInstanceMap dynamicUnstableInstances;
    std::map<OSuint32, OSid> spatialStructures;

    enum InstanceIndex
    {
      STATIC_INDEX           = 0x00000000,
      DYNAMIC_STABLE_INDEX   = 0x80000000,
      DYNAMIC_UNSTABLE_INDEX = 0x40000000,
    };

    inline OSuint countPrimitives()
    {
      // todo: perhaps rename to countComponents
      OSuint nPrimitives = 0;

      for(GeometryInstanceMap::iterator i = staticInstances.begin(); i != staticInstances.end(); ++i)
        nPrimitives += (*i).second->countPrimitives();

      for(GeometryInstanceMap::iterator i = dynamicStableInstances.begin(); i != dynamicStableInstances.end(); ++i)
        nPrimitives += (*i).second->countPrimitives();

      for(GeometryInstanceMap::iterator i = dynamicUnstableInstances.begin(); i != dynamicUnstableInstances.end(); ++i)
        nPrimitives += (*i).second->countPrimitives();

      return nPrimitives;
    }

  public:
    inline ~GeometryPool()
    {
      for(GeometryInstanceMap::iterator i = staticInstances.begin(); i != staticInstances.end(); ++i)
        delete (*i).second;

      for(GeometryInstanceMap::iterator i = dynamicStableInstances.begin(); i != dynamicStableInstances.end(); ++i)
        delete (*i).second;

      for(GeometryInstanceMap::iterator i = dynamicUnstableInstances.begin(); i != dynamicUnstableInstances.end(); ++i)
        delete (*i).second;

      staticInstances.clear();
      dynamicStableInstances.clear();
      dynamicUnstableInstances.clear();
    }

    inline OSid addGeometryInstance(Geometry* geometry, GE_GEOMETRY_STABILITY stability)
    {
      GeometryInstance* instance = new GeometryInstance(geometry, stability);
      switch(stability)
      {
      case GE_STATIC:
        staticInstances.insert(std::pair<Geometry*, GeometryInstance*>(geometry, instance));
        break;

      case GE_DYNAMIC_STABLE:
        dynamicStableInstances.insert(std::pair<Geometry*, GeometryInstance*>(geometry, instance));
        break;

      case GE_DYNAMIC_UNSTABLE:
        dynamicUnstableInstances.insert(std::pair<Geometry*, GeometryInstance*>(geometry, instance));
        break;
      }
      return (OSid)instance;
    }

    inline void removeGeometryInstance(OSid geometryInstance)
    {
      /*std::vector<Geometry*>::iterator i = geometryInstances.at(index);
      geometryInstances.erase(i);*/
      for(std::multimap<Geometry*, GeometryInstance*>::iterator i = staticInstances.begin(); i != staticInstances.end(); ++i)
      {
        if((OSid)(*i).second == geometryInstance)
          delete (*i).second;
        staticInstances.erase(i);
      }
    }

    inline OSid findStructure(GE_HARDWARE_INTERFACE hardwareInterface, GE_SPATIAL_STRUCTURE structure)
    {
      OSid spatialStructure = 0;

      std::map<OSuint32, OSid>::iterator i = spatialStructures.find((hardwareInterface << 16) + structure);
      if(i == spatialStructures.end())
        return 0;
      return (*i).second;
    }
  };
}

#endif
