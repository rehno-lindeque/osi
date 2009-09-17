#ifndef __OSIX_SEMANTICDB_HPP__
#define __OSIX_SEMANTICDB_HPP__
//////////////////////////////////////////////////////////////////////////////
//
//    SEMANTICDB.HPP
//
//    Ver 0.1 (2009-08-21)
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Main OpenParser C++ interface.
*/
/*                                 INCLUDES                                 */
#include "../../osi/os.hpp"

/*                                 CONSTANTS                                */
#define SEMANTICDB_VERSION "0.1"

namespace OSIX
{
  /*                                   TYPES                                  */


  /*                                     API                                  */
  class SemanticDB
  {
  protected:
    inline SemanticDB() {}
    inline SemanticDB(const SemanticDB&) {}
    
  public:
    typedef OSuint32 SemanticId;
    struct Relation
    {
      SemanticId domain;
      SemanticId codomain;
    };    
  };

  SemanticDB* OSI_API_CALL semanticDBInit();
}

#endif
