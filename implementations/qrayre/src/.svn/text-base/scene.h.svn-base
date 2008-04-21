#ifndef __QRAYRE_SCENE_H__
#define __QRAYRE_SCENE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    SCENE.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      QRayRE scene class
*/

/*                                 CLASSES                                  */
namespace QRayRE
{
  #define QRAY_MAX_LIGHTS 16

  class QRayScene : public BaseRE::Scene
  {
  public:
    AxisAlignedBoundingBox boundingBox;
    OSobject geometryPool;
    OSid spatialStructure;
    
    QRayLight* lights[QRAY_MAX_LIGHTS];
    uint nLights;

    inline QRayScene() : geometryPool(0), spatialStructure(0), nLights(0)
    {}

    inline void build()
    {
      if(geometryPool != 0)
      {
        spatialStructure = geGetStructure(geometryPool, GE_NATIVE, GE_BIH, GE_BIH_FLAT, GE_VEC3_POINTS);
        boundingBox = geGetBoundingBox(geometryPool);
      }
    }

    inline void addLight(QRayLight* light)
    {
      if(nLights >= QRAY_MAX_LIGHTS)
        return;
      lights[nLights] = light;
      ++nLights;
    }

    virtual ~QRayScene()
    {}
  };

  #undef QRAY_MAX_LIGHTS
};

#endif
