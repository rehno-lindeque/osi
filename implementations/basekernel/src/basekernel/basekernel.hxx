#ifndef __BASEKERNEL_BASEKERNEL_HXX__
#define __BASEKERNEL_BASEKERNEL_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    BASEKERNEL.HXX
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              IMPLEMENTATION                              */
namespace BaseKernel
{
  template<class ObjectType> ObjectType* Kernel::createObject()                  { ObjectType* obj = new ObjectType(); objects.push_front((Base::Object*)obj); return obj; }
  template<class ObjectType> ObjectType* Kernel::beginObject()                   { ObjectType* obj = createObject<ObjectType>(); activeObjects.push((Base::Object*)obj); return obj; }
  template<class ObjectType> ObjectType* Kernel::endObject()                     { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> ObjectType* Kernel::openObject(OSobject object)     { ObjectType* obj = (ObjectType*)(Base::Object*)object; activeObjects.push((Base::Object*)obj); return obj; }
  template<class ObjectType> ObjectType* Kernel::closeObject()                   { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> ObjectType* Kernel::getActiveObject()               { return (ObjectType*)activeObjects.top(); }
                             void        Kernel::delObject(Base::Object* object) { delete object; objects.remove(object); }
  template<class ObjectType> ObjectType* Kernel::cast_id(OSobject object)        { return (ObjectType*)(Base::Object*)object; }
  template<class ObjectType> OSobject    Kernel::cast_object(ObjectType* object)    { return (OSid)(Base::Object*)object; }
                             void        Kernel::delAllObjects()                 { for(std::list<Base::Object*>::iterator i = objects.begin(); i != objects.end(); ++i) delete *i; objects.clear(); }
}

#endif
