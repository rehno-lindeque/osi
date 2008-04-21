#ifndef __BASEGUI_BASEGUI_H__
#define __BASEGUI_BASEGUI_H__
//////////////////////////////////////////////////////////////////////////////
//
//    BASEGUI.H
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Sample base classes for OpenGUI backends.

    DEPENDENCIES:
      gui.h or gui.hpp must be included before basegui.h
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
// OpenGUI headers
#include <osi/gui/gui.hpp>

// STL headers
#include <vector>
#include <list>
#include <stack>
#include <map>
/*#ifdef MSVC_BUILD
  #include <hash_map>	
#else
  #include <ext/hash_map>
#endif*/

// Base common headers
#include <base/common/types.h>
//#include <base/common/functions.h>
#include <base/common/structures.h>
#include <base/common/object.h>

// BaseGUI common headers
/*#include "common/types.h"
#include "common/functions.h"
#include "common/structures.h"*/

/*                            FORWARD DECLERATIONS                          */

/*                                 CLASSES                                  */
namespace BaseGUI
{
  class GUI : public ::GUI
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
#include "basegui.hxx"

/*                              COMPILER MACROS                             */
#pragma warning(pop)

#endif
