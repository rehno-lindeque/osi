#ifndef __QPARSER_TOKEN_H__
#define __QPARSER_TOKEN_H__
//////////////////////////////////////////////////////////////////////////////
//
//    TOKEN.H
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Common token definitions
*/
namespace QParser
{
/*                                   TYPES                                  */
  typedef OSIX::ParseToken ParseToken;
  
/*                                 CONSTANTS                                */

  // Note: We don't use an enum for this purpose because sadly enum's are restricted
  //       to signed integers types which causes trouble when doing comparisons with 
  //       the unsigned ParseToken type.
  const ParseToken TOKEN_FLAG_REDUCEPREV = (ParseToken(1) << (sizeof(ParseToken)*8 - 2));
  const ParseToken TOKEN_FLAG_SHIFT      = (ParseToken(1) << (sizeof(ParseToken)*8 - 1));
  
  const ParseToken TOKEN_SPECIAL_EOF     = ~ParseToken(0);
  const ParseToken TOKEN_SPECIAL_IGNORE  = ParseToken(~(TOKEN_FLAG_REDUCEPREV | TOKEN_FLAG_SHIFT));
  
  const ParseToken TOKEN_TERMINAL_IDENTIFIER = TOKEN_FLAG_SHIFT | 0;
  const ParseToken TOKEN_TERMINAL_LITERAL    = TOKEN_FLAG_SHIFT | 1;
  const ParseToken TOKEN_TERMINAL_FIRST      = TOKEN_FLAG_SHIFT | 2;
  
  const ParseToken TOKEN_NONTERMINAL_FIRST   = 0;
  
  const ParseToken TOKEN_ACTION_PIVOT    = ~ParseToken(0) - 1;
  const ParseToken TOKEN_ACTION_RETURN   = ~ParseToken(0) - 2;
  const ParseToken TOKEN_ACTION_GOTO     = ~ParseToken(0) - 3;
  const ParseToken TOKEN_ACTION_ACCEPT   = ~ParseToken(0) - 4;
  const ParseToken TOKEN_RESERVED_TOKENS = ~ParseToken(0) - 4;
}

#endif
