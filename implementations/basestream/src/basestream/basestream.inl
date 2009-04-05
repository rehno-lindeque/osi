#ifdef  __BASESTREAM_BASESTREAM_H__
#ifndef __BASESTREAM_BASESTREAM_HXX__
#define __BASESTREAM_BASESTREAM_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    BASESTREAM.HXX
//
//    Copyright � 2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              IMPLEMENTATION                              */
namespace BaseStream
{
  template<class ObjectType> ObjectType* Stream::createObject()                  { ObjectType* obj = new ObjectType(); objects.push_front((Base::Object*)obj); return obj; }
  template<class ObjectType> ObjectType* Stream::beginObject()                   { ObjectType* obj = createObject<ObjectType>(); activeObjects.push((Base::Object*)obj); return obj; }
  template<class ObjectType> ObjectType* Stream::endObject()                     { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> ObjectType* Stream::openObject(OSobject object)     { ObjectType* obj = (ObjectType*)(Base::Object*)object; activeObjects.push((Base::Object*)obj); return obj; }
  template<class ObjectType> ObjectType* Stream::closeObject()                   { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> ObjectType* Stream::getActiveObject()               { return (ObjectType*)activeObjects.top(); }
                             void        Stream::delObject(Base::Object* object) { delete object; objects.remove(object); }
  template<class ObjectType> ObjectType* Stream::cast_id(OSobject object)        { return (ObjectType*)(Base::Object*)object; }
  template<class ObjectType> OSobject    Stream::cast_object(ObjectType* object) { return (OSobject)(Base::Object*)object; }
                             void        Stream::delAllObjects()                 { for(std::list<Base::Object*>::iterator i = objects.begin(); i != objects.end(); ++i) delete *i; objects.clear(); }
}

#endif
#endif
