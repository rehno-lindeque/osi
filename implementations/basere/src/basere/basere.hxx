#ifndef __BASERE_BASERE_HXX__
#define __BASERE_BASERE_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    BASERE.HXX
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace BaseRE
{
/*                                FUNCTIONS                                 */
  template<class ObjectType> inline ObjectType* createObject()                { ObjectType* obj = new ObjectType(); objects.push_front((Object*)obj); return obj; }
  template<class ObjectType> inline ObjectType* beginObject()                 { ObjectType* obj = createObject<ObjectType>(); activeObjects.push((Object*)obj); return obj; }
  template<class ObjectType> inline ObjectType* endObject()                   { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> inline ObjectType* openObject(OSobject object)   { ObjectType* obj = (ObjectType*)(Object*)object; activeObjects.push((Object*)obj); return obj; }
  template<class ObjectType> inline ObjectType* closeObject()                 { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> inline ObjectType* getActiveObject()             { return (ObjectType*)activeObjects.top(); }
  inline void delObject(Object* object)                                       { delete object; objects.remove(object); }
  template<class ObjectType> inline ObjectType* cast_id(OSobject object)      { return (ObjectType*)(Object*)object; }
  template<class ObjectType> inline OSobject cast_object(ObjectType* object)  { return (OSid)(Object*)object; }
  inline void shutdown()                                                      { for(std::list<Object*>::iterator i = objects.begin(); i != objects.end(); ++i) delete *i; objects.clear(); }
}

#endif
