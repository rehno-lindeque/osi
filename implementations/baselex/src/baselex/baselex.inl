#ifdef  __BASELEX_BASELEX_H__
#ifndef __BASELEX_BASELEX_HXX__
#define __BASELEX_BASELEX_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    BASELEX.HXX
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              IMPLEMENTATION                              */
namespace BaseLex
{
  template<class ObjectType, typename... ParamTypes> INLINE ObjectType* Lex::CreateObject(ParamTypes... params) 
  { ObjectType* obj = new ObjectType(params...); objects.push_front((Base::Object*)obj); return obj; }
  
  template<class ObjectType, typename... ParamTypes> INLINE ObjectType* Lex::BeginObject(ParamTypes... params)
  { ObjectType* obj = CreateObject<ObjectType, ParamTypes...>(params...); activeObjects.push((Base::Object*)obj); return obj; }
  
  template<class ObjectType> INLINE ObjectType* Lex::EndObject()                     { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> INLINE ObjectType* Lex::OpenObject(OSobject object)     { ObjectType* obj = (ObjectType*)(Base::Object*)object; activeObjects.push((Base::Object*)obj); return obj; }
  template<class ObjectType> INLINE ObjectType* Lex::CloseObject()                   { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> INLINE ObjectType* Lex::GetActiveObject()               { return (ObjectType*)activeObjects.top(); }
                             INLINE void        Lex::DeleteObject(Base::Object* object) { delete object; objects.remove(object); }
  template<class ObjectType> INLINE ObjectType* Lex::cast_id(OSobject object)        { return (ObjectType*)(Base::Object*)object; }
  template<class ObjectType> INLINE OSobject    Lex::cast_object(ObjectType* object) { return (OSobject)(Base::Object*)object; }
                             //INLINE void        Lex::DeleteAllObjects()              { for(auto i = objects.begin(); i != objects.end(); ++i) delete *i; objects.clear(); }
}

#endif
#endif
