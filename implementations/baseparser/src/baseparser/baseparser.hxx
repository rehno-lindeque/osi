#ifdef  __BASEPARSER_BASEPARSER_H__
#ifndef __BASEPARSER_BASEPARSER_HXX__
#define __BASEPARSER_BASEPARSER_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    BASEPARSER.HXX
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              IMPLEMENTATION                              */
namespace BaseParser
{
  template<class ObjectType> ObjectType* Parser::createObject()                  { ObjectType* obj = new ObjectType(); objects.push_front((Base::Object*)obj); return obj; }
  template<class ObjectType> ObjectType* Parser::beginObject()                   { ObjectType* obj = createObject<ObjectType>(); activeObjects.push((Base::Object*)obj); return obj; }
  template<class ObjectType> ObjectType* Parser::endObject()                     { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> ObjectType* Parser::openObject(OSobject object)     { ObjectType* obj = (ObjectType*)(Base::Object*)object; activeObjects.push((Base::Object*)obj); return obj; }
  template<class ObjectType> ObjectType* Parser::closeObject()                   { ObjectType* obj = (ObjectType*)activeObjects.top(); activeObjects.pop(); return obj; }
  template<class ObjectType> ObjectType* Parser::getActiveObject()               { return (ObjectType*)activeObjects.top(); }
                             void        Parser::delObject(Base::Object* object) { delete object; objects.remove(object); }
  template<class ObjectType> ObjectType* Parser::cast_id(OSobject object)        { return (ObjectType*)(Base::Object*)object; }
  template<class ObjectType> OSobject    Parser::cast_object(ObjectType* object) { return (OSobject)(Base::Object*)object; }
                             void        Parser::delAllObjects()                 { for(std::list<Base::Object*>::iterator i = objects.begin(); i != objects.end(); ++i) delete *i; objects.clear(); }
}

#endif
#endif
