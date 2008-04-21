#ifdef  __BASEGUI_BASEGUI_H__
#ifndef __BASEGUI_BASEGUI_HXX__
#define __BASEGUI_BASEGUI_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    BASEGUI.HXX
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              IMPLEMENTATION                              */
namespace BaseGUI
{
  template<class ObjectType> ObjectType* GUI::createObject()                  { ObjectType* obj = new ObjectType(); objects.push_front((Base::Object*)obj); return obj; }
  template<class ObjectType> ObjectType* GUI::beginObject()                   { ObjectType* obj = createObject<ObjectType>(); activeObjects.push((Base::Object*)obj); return obj; }
  template<class ObjectType> ObjectType* GUI::endObject()                     { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> ObjectType* GUI::openObject(OSobject object)     { ObjectType* obj = (ObjectType*)(Base::Object*)object; activeObjects.push((Base::Object*)obj); return obj; }
  template<class ObjectType> ObjectType* GUI::closeObject()                   { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> ObjectType* GUI::getActiveObject()               { return (ObjectType*)activeObjects.top(); }
                             void        GUI::delObject(Base::Object* object) { delete object; objects.remove(object); }
  template<class ObjectType> ObjectType* GUI::cast_id(OSobject object)        { return (ObjectType*)(Base::Object*)object; }
  template<class ObjectType> OSobject    GUI::cast_object(ObjectType* object) { return (OSobject)(Base::Object*)object; }
                             void        GUI::delAllObjects()                 { for(std::list<Base::Object*>::iterator i = objects.begin(); i != objects.end(); ++i) delete *i; objects.clear(); }
}

#endif
#endif
