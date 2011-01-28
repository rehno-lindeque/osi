#ifndef __BASELEX_BASELEX_H__
#define __BASELEX_BASELEX_H__
//////////////////////////////////////////////////////////////////////////////
//
//    BASELEX.H
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Sample base classes for OpenLex backends.

    DEPENDENCIES:
     lex.h or lex.hpp must be included before baselex.h
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
// OpenLex headers
#include <osix/lex/lex.hpp>

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
namespace BaseLex
{
  class Lex : public OSIX::Lex
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
#include "baselex.inl"

/*                              COMPILER MACROS                             */

#endif
