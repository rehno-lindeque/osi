#ifndef __BASESEMANTICDB_BASESEMANTICDB_H__
#define __BASESEMANTICDB_BASESEMANTICDB_H__
//////////////////////////////////////////////////////////////////////////////
//
//    BASESEMANTICDB.H
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Sample base classes for OpenSemanticDB backends.

    DEPENDENCIES:
     semanticdb.h or semanticdb.hpp must be included before basesemanticdb.h
*/
/*                              COMPILER MACROS                             */
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4311)
# pragma warning(disable:4312)
#endif

#if defined(__GNUC__)
# define STDEXT_NAMESPACE __gnu_cxx
#elif defined(_MSC_VER)
# define STDEXT_NAMESPACE stdext
#else
# define STDEXT_NAMESPACE stdext
#endif

/*                                 INCLUDES                                 */
// OpenSemanticDB headers
#include <osix/semanticdb/semanticdb.hpp>

// STL headers
#include <list>
#include <stack>

// Base common headers
#include <base/common/types.h>
#include <base/common/functions.h>
#include <base/common/structures.h>
#include <base/common/object.h>

/*                            FORWARD DECLERATIONS                          */

/*                                 CLASSES                                  */
namespace BaseSemanticDB
{
  class SemanticDB : public OSIX::SemanticDB
  {
  public:
    std::list<Base::Object*> objects;
    std::stack<Base::Object*> activeObjects;

    template<class ObjectType, typename... ParamTypes> INLINE ObjectType* CreateObject(ParamTypes... params);
    template<class ObjectType, typename... ParamTypes> INLINE ObjectType* BeginObject(ParamTypes... params);  
    template<class ObjectType> INLINE ObjectType* EndObject();
    template<class ObjectType> INLINE ObjectType* OpenObject(OSobject object);
    template<class ObjectType> INLINE ObjectType* CloseObject();
    template<class ObjectType> INLINE ObjectType* GetActiveObject();
                               INLINE void DeleteObject(Base::Object* object);
    template<class ObjectType> static INLINE ObjectType* cast_id(OSobject object);
    template<class ObjectType> static INLINE OSobject cast_object(ObjectType* object);
                               INLINE void DeleteAllObjects();
  };
}

/*                                 INCLUDES                                 */


/*                              IMPLEMENTATION                              */
#include "basesemanticdb.hxx"

/*                              COMPILER MACROS                             */

#endif
