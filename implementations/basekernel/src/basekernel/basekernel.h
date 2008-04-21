#ifndef __BASEKERNEL_BASEKERNEL_H__
#define __BASEKERNEL_BASEKERNEL_H__
//////////////////////////////////////////////////////////////////////////////
//
//    BASEKERNEL.H
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Sample base classes for OpenKernel backends.
*/
/*                              COMPILER MACROS                             */
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

#ifdef MSVC_BUILD
  #define STDEXT_NAMESPACE stdext
#else
  #define STDEXT_NAMESPACE __gnu_cxx
#endif

/*                                 INCLUDES                                 */
// OpenKernel
#include <osi/kernel/kernel.hpp>

// libstdc++
#include <vector>
#include <list>
#include <stack>

// Base
#include <base/common/types.h>
#include <base/common/functions.h>
#include <base/common/structures.h>
#include <base/common/object.h>

/*                            FORWARD DECLERATIONS                          */

/*                                 CLASSES                                  */
namespace BaseKernel
{
  class Kernel : public ::Kernel
  {
  public:
    std::list<Base::Object*> objects;
    std::stack<Base::Object*> activeObjects;

    template<class ObjectType> inline ObjectType* createObject();
    template<class ObjectType> inline ObjectType* beginObject();
    template<class ObjectType> inline ObjectType* endObject();
    template<class ObjectType> inline ObjectType* openObject(OSobject object);
    template<class ObjectType> inline ObjectType* closeObject();
    template<class ObjectType> inline ObjectType* getActiveObject();
                               inline void delObject(Base::Object* object);
    template<class ObjectType> inline static ObjectType* cast_id(OSobject object);
    template<class ObjectType> inline static OSobject cast_object(ObjectType* object);
                               inline void delAllObjects();
  };
}

/*                                 INCLUDES                                 */


/*                              IMPLEMENTATION                              */
#include "basekernel.hxx"

/*                              COMPILER MACROS                             */
#pragma warning(pop)

#endif
