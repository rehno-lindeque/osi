#ifdef  __BASEPARSER_BASEPARSER_H__
#ifndef __BASEPARSER_BASEPARSER_HXX__
#define __BASEPARSER_BASEPARSER_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    BASEPARSER.HXX
//
//    Copyright © 2007-2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              IMPLEMENTATION                              */
namespace BaseParser
{
  template<class ObjectType, typename... ParamTypes> INLINE ObjectType* Parser::CreateObject(ParamTypes... params) 
  { ObjectType* obj = new ObjectType(params...); objects.push_front((Base::Object*)obj); return obj; }
  
  template<class ObjectType, typename... ParamTypes> INLINE ObjectType* Parser::BeginObject(ParamTypes... params)
  { ObjectType* obj = CreateObject<ObjectType, ParamTypes...>(params...); activeObjects.push((Base::Object*)obj); return obj; }
  
  template<class ObjectType> INLINE ObjectType* Parser::EndObject()                     { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> INLINE ObjectType* Parser::OpenObject(OSobject object)     { ObjectType* obj = (ObjectType*)(Base::Object*)object; activeObjects.push((Base::Object*)obj); return obj; }
  template<class ObjectType> INLINE ObjectType* Parser::CloseObject()                   { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> INLINE ObjectType* Parser::GetActiveObject()               { return (ObjectType*)activeObjects.top(); }
                             INLINE void        Parser::DeleteObject(Base::Object* object) { delete object; objects.remove(object); }
  template<class ObjectType> INLINE ObjectType* Parser::cast_id(OSobject object)        { return (ObjectType*)(Base::Object*)object; }
  template<class ObjectType> INLINE OSobject    Parser::cast_object(ObjectType* object) { return (OSobject)(Base::Object*)object; }
                             INLINE void        Parser::DeleteAllObjects()              { for(auto i = objects.begin(); i != objects.end(); ++i) delete *i; objects.clear(); }
}

#endif
#endif
