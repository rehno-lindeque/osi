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
  typedef uint16 ParseToken;

  // Note: We don't use an enum for this purpose because sadly enum's are restricted
  //       to signed integers types which causes trouble when doing comparisons with 
  //       the unsigned ParseToken type.
  const ParseToken TOKEN_IGNORE          = ~ParseToken(0) - 2;
  const ParseToken TOKEN_GOTO            = ~ParseToken(0) - 1;
  const ParseToken TOKEN_PIVOT           = ~ParseToken(0);
  const ParseToken TOKEN_FLAG_REDUCEPREV = (ParseToken(1) << (sizeof(ParseToken)*8 - 2));
  const ParseToken TOKEN_FLAG_SHIFT      = (ParseToken(1) << (sizeof(ParseToken)*8 - 1));        
  
}

#endif
