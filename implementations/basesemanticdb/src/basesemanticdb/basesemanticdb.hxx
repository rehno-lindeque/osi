#ifdef  __BASESEMANTICDB_BASESEMANTICDB_H__
#ifndef __BASESEMANTICDB_BASESEMANTICDB_HXX__
#define __BASESEMANTICDB_BASESEMANTICDB_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    BASESEMANTICDB.HXX
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              IMPLEMENTATION                              */
namespace BaseSemanticDB
{
  template<class ObjectType, typename... ParamTypes> INLINE ObjectType* SemanticDB::CreateObject(ParamTypes... params) 
  { ObjectType* obj = new ObjectType(params...); objects.push_front((Base::Object*)obj); return obj; }
  
  template<class ObjectType, typename... ParamTypes> INLINE ObjectType* SemanticDB::BeginObject(ParamTypes... params)
  { ObjectType* obj = CreateObject<ObjectType, ParamTypes...>(params...); activeObjects.push((Base::Object*)obj); return obj; }
  
  template<class ObjectType> INLINE ObjectType* SemanticDB::EndObject()                     { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> INLINE ObjectType* SemanticDB::OpenObject(OSobject object)     { ObjectType* obj = (ObjectType*)(Base::Object*)object; activeObjects.push((Base::Object*)obj); return obj; }
  template<class ObjectType> INLINE ObjectType* SemanticDB::CloseObject()                   { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> INLINE ObjectType* SemanticDB::GetActiveObject()               { return (ObjectType*)activeObjects.top(); }
                             INLINE void        SemanticDB::DeleteObject(Base::Object* object) { delete object; objects.remove(object); }
  template<class ObjectType> INLINE ObjectType* SemanticDB::cast_id(OSobject object)        { return (ObjectType*)(Base::Object*)object; }
  template<class ObjectType> INLINE OSobject    SemanticDB::cast_object(ObjectType* object) { return (OSobject)(Base::Object*)object; }
                             INLINE void        SemanticDB::DeleteAllObjects()              { for(std::list<Base::Object*>::iterator i = objects.begin(); i != objects.end(); ++i) delete *i; objects.clear(); }
}

#endif
#endif
