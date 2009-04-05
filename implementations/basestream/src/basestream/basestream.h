#ifndef __BASESTREAM_BASESTREAM_H__
#define __BASESTREAM_BASESTREAM_H__
//////////////////////////////////////////////////////////////////////////////
//
//    BASESTREAM.H
//
//    Copyright © 2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Sample base classes for OpenStream backends.

    DEPENDENCIES:
      stream.h or stream.hpp must be included before basestream.h
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
// OpenParser headers
#include <osix/stream/stream.hpp>

// STL headers
#include <vector>
#include <list>
#include <stack>
#include <map>

// Base common headers
#include <base/common/types.h>
#include <base/common/functions.h>
#include <base/common/structures.h>
#include <base/common/object.h>

/*                            FORWARD DECLERATIONS                          */

/*                                 CLASSES                                  */
namespace BaseStream
{
  class Stream : public OSIX::Stream
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
    template<class ObjectType> static inline ObjectType* cast_id(OSobject object);
    template<class ObjectType> static inline OSobject cast_object(ObjectType* object);
                               inline void delAllObjects();
  };
}

/*                                 INCLUDES                                 */


/*                              IMPLEMENTATION                              */
#include "basestream.inl"

/*                              COMPILER MACROS                             */
#ifdef _MSC_VER
# pragma warning(pop)
#endif

#endif
