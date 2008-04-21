#ifndef __GLRASRE_FRAME_H__
#define __GLRASRE_FRAME_H__
//////////////////////////////////////////////////////////////////////////////
//
//    FRAME.H
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

/*                              COMPILER MACROS                             */
#ifndef __GLRASRE_SCENE_H__
  #error "scene.h must be included before frame.h"
#endif

using namespace BaseRE;
namespace GlRasRE
{
/*                                CLASSES                                   */
  class GlRasFrame : public Frame
  {
  public:
    /*uint streamOffset;
    uint streamLength;*/
    GlRasFrame() /*: streamLength(0)/*, geometry(null)*/
    {
      /*if(activeScene)
        streamOffset = ((GlRasScene*)activeScene)->geometry.getMapOffset();
      else
      {
        streamOffset = 0;
        //todo: create your own geometry buffer, or use the parent composite's geometry buffer
      }*/
    }

    void build()
    {
      //todo
    }

    /*old: virtual void addQuads(const Quads& quads)
    {
      if(activeScene)
        ((GlRasScene*)activeScene)->geometry.addQuads(quads);
      else
      {
        //todo (see constructor)
      }
    }*/
  };
};

#endif
