//////////////////////////////////////////////////////////////////////////////
//
//    BASEGE.CPP
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*                                 INCLUDES                                 */
// BaseGE
#include "basege.h"

using namespace BaseGE;

/*                                 GLOBALS                                  */
std::list<Object*>  BaseGE::objects;
std::stack<Object*> BaseGE::activeObjects;
ElementsPool        BaseGE::elementsPool;
