#ifndef __OSIX_PARSERDBG_H__
#define __OSIX_PARSERDBG_H__
//////////////////////////////////////////////////////////////////////////////
//
//    PARSERDBG.H
//
//    Ver 0.1 (2007-06-11)
//
//    Copyright © 2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*
    DESCRIPTION:
      Debug OpenParser C++ interface.

    USAGE:
      parser.h must be included before this header
*/
/*                                 INCLUDES                                 */

/*                                 CONSTANTS                                */
#define PARSERDBG_VERSION 0.1

/*                                   TYPES                                  */


/*                                     API                                  */
class ParserDbg
{
public:
  OSI_DYNAMIC_METHOD void OSI_API_CALL debugOutputTokens();
  OSI_DYNAMIC_METHOD void OSI_API_CALL debugOutputStatements();
  OSI_DYNAMIC_METHOD void OSI_API_CALL debugOutputParseResult(OSobject parseResult);
};

#endif
