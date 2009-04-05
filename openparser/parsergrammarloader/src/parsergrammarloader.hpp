#ifndef __PARSERGRAMMARLOADER_HPP__
#define __PARSERGRAMMARLOADER_HPP__
//////////////////////////////////////////////////////////////////////////////
//
//    PARSERGRAMMARLOADER.HPP
//
//    Copyright © 2008, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      This library assists grammar construction by parsing an input string
      representing the OpenParser grammar.

    USEAGE:
      + The start symbol is always the last production currently
      + No forward declarations are needed. It should be done automatically
        by the openparser api.

    TODO:
      + Error reporting...
      + Escape character in lexical tokens
*/	
/*                                 INCLUDES                                 */
// OpenParser
#include <osix/parser/parser.hpp>

namespace Parser { 
namespace GrammarLoader { 

/*                             FORWARD DECLERATIONS                         */
void constructGrammar(OSIX::Parser &parser, const OSchar* buffer, OSuint bufferSize, OSid* ids, OSuint idsBufferSize);

}}

#endif
