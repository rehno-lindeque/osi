#ifndef __BASERE_RENDER_H__
#define __BASERE_RENDER_H__
//////////////////////////////////////////////////////////////////////////////
//
//    RENDER.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Base class for BaseRE render.
*/

namespace BaseRE
{
/*                                  CLASSES                                 */
  class Render : public Object
  {
  public:
    const Camera* camera;
    const Scene*  scene;

    /*struct
    {
      RE_RENDER_PARAM type
    } *paramTypes;*/

    //inline Render() : Object(TYPE), camera(null), scene(null) {}
    inline Render() : camera(null), scene(null) {}
    
    /*??? void setParameter(RE_RENDER_PARAM type, REid param)
    {
      re
    }*/
  };
}

#endif
