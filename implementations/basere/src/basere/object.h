#ifndef __BASERE_OBJECT_H__
#define __BASERE_OBJECT_H__
//////////////////////////////////////////////////////////////////////////////
//
//    OBJECT.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Base class for common BaseRE objects.

      An Object is any object is any object that can be created via
      the standard functions: createObject, delObject, beginObject and 
      endObject.
      These almost always correspond with a (id-returning) reBegin\reEnd 
      block. (Note: also see Issue 23 in the OpenRE specifications)
*/

namespace BaseRE
{
/*                                  CLASSES                                 */
  class Object
  {
  protected:
    
  public:
    virtual ~Object() {}
  };
}

#endif
