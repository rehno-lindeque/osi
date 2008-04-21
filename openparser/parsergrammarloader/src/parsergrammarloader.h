#ifndef __PARSERGRAMMARLOADER_H__
#define __PARSERGRAMMARLOADER_H__
//////////////////////////////////////////////////////////////////////////////
//
//    PARSERGRAMMARLOADER.H
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
#include <osix/parser/parser.h>

/*                             FORWARD DECLERATIONS                         */
#ifdef __cplusplus
extern "C" {
#endif

void parserConstructGrammar(const OSchar* buffer, OSuint bufferSize, OSid* ids, OSuint idsBufferSize);

#ifdef __cplusplus
}
#endif

#endif
