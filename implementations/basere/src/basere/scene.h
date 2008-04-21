#ifndef __BASERE_SCENE_H__
#define __BASERE_SCENE_H__
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
      Base class for BaseRE scenes.
*/

namespace BaseRE
{
/*                                  CLASSES                                 */
  class Scene : public Object
  {
  };

  /* old:
  class Scene : public Composite
  {
  public:
    list<Geometry*> geometry;

    static const RE_OBJECT_TYPE TYPE;
    inline Scene() : Composite(TYPE, null) {}
  };*/
}

#endif
