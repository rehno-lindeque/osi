#ifndef __BASEGE_ATTRIBUTEOBJECT_H__
#define __BASEGE_ATTRIBUTEOBJECT_H__
//////////////////////////////////////////////////////////////////////////////
//
//    ATTRIBUTEOBJECT.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Object that contains attributes.

    IMPLEMENTATION:
      Note
*/

namespace BaseGE
{
/*                                  CLASSES                                 */
  class AttributeObject : public Base::Object
  {
  protected:
    STDEXT_NAMESPACE::hash_map<OSenum, OSid> attributes;
    //todo: optimization void* indices;

  public:
    inline void setAttribute(OSenum attribute, OSid id) { attributes.insert(std::pair<OSenum, OSid>(attribute, id)); }
    inline OSid getAttribute(OSenum attribute) { return attributes[attribute]; }
  };
}

#endif
