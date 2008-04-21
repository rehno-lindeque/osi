#ifndef __BASEGE_ELEMENTSPOOL_H__
#define __BASEGE_ELEMENTSPOOL_H__
//////////////////////////////////////////////////////////////////////////////
//
//    ELEMENTSPOOL.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Basic elements pool for OpenGE backends.

    DEPENDENCIES:
      STL header <vector> must be included before this file.
*/

namespace BaseGE
{
/*                                  CLASSES                                 */
  class ElementsPool
  {
  protected:
    std::vector<Elements*> elementsObjects;

  public:
    void addElements(Elements* elements)
    {
      elementsObjects.push_back(elements);
    }

    inline ~ElementsPool()
    {
      /*todo free elements*/
    }
  };
}

#endif
