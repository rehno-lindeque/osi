#ifndef __GLRASRE_SCENE_H__
#define __GLRASRE_SCENE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    SCENE.H
//
//    Author:      Rehno Lindeque
//
//    Description: GlRasRE classes used to implement api
//
//    Version: 0.1 (informal prototype)
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __GLRASRE_GEOMETRY_H__
  #error "geometry.h must be included before scene.h"
#endif

using namespace BaseRE;
namespace GlRasRE
{
  class GlRasScene : public Scene
  {
  public:
    //GlRasGeometry geometry;
    GlRasScene()
    {
      //geometry.create(512, 512);
      //geometry.map(WRITE_ONLY);
    }

    inline void build()
    {
      //geometry.build();
    }

    virtual ~GlRasScene()
    {
      //geometry.destroy();
    }

    /*old: virtual void addQuads(const Quads& quads)
    {
      geometry.addQuads(quads);
    }*/
  };
};

#endif
