#ifndef __OSIX_LEX_HPP__
#define __OSIX_LEX_HPP__
//////////////////////////////////////////////////////////////////////////////
//
//    LEX.HPP
//
//    Ver 0.1 (2009-09-19)
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
#include <osix/semanticdb/semanticdb.hpp>

/*                                 CONSTANTS                                */
#define LEX_VERSION "0.1"

namespace OSIX
{
  /*                                   TYPES                                  */
  enum LEX_BOUNDED_LINETYPE
  {
    SINGLE_LINE,
    MULTI_LINE
  };
  
  /*                                     API                                  */
  class Lex
  {
  protected:
    inline Lex() {}
    inline Lex(const Lex&) {}

  public:
    /* alphabet */
    OSI_DYNAMIC_METHOD void OSI_API_CALL BeginAlphabet(const OSchar* alphabetName);
    OSI_DYNAMIC_METHOD void OSI_API_CALL EndAlphabet();
  
    /* raw strings */
    OSI_DYNAMIC_METHOD void OSI_API_CALL BeginRaw();
    OSI_DYNAMIC_METHOD void OSI_API_CALL EndRaw();

    /* nil tokens */
    OSI_DYNAMIC_METHOD void OSI_API_CALL BeginNil();
    OSI_DYNAMIC_METHOD void OSI_API_CALL EndNil();

    /* lexical tokens */
    OSI_DYNAMIC_METHOD void OSI_API_CALL BeginLex();
    OSI_DYNAMIC_METHOD void OSI_API_CALL EndLex();

    OSI_DYNAMIC_METHOD void OSI_API_CALL BeginLexSymbols();
    OSI_DYNAMIC_METHOD void OSI_API_CALL EndLexSymbols();

    OSI_DYNAMIC_METHOD void OSI_API_CALL BeginLexWords();
    OSI_DYNAMIC_METHOD void OSI_API_CALL EndLexWords();

    /* lexical & nill tokens */
    OSI_DYNAMIC_METHOD OSid OSI_API_CALL StringToken(const OSchar* tokenName, const OSchar* value);
    OSI_DYNAMIC_METHOD OSid OSI_API_CALL CharToken(const OSchar* tokenName, char value);
    OSI_INLINE_METHOD  OSid OSI_API_CALL KeywordToken(const OSchar* keyword) { return StringToken(keyword, keyword); }
    OSI_DYNAMIC_METHOD OSid OSI_API_CALL BoundedToken(const OSchar* tokenName, const OSchar* leftBoundingValue, const OSchar* rightBoundingValue, enum LEX_BOUNDED_LINETYPE lineType = MULTI_LINE);

    /* token store */
    OSI_DYNAMIC_METHOD SemanticDB& GetSemanticDB();
    OSI_DYNAMIC_METHOD const SemanticDB& GetSemanticDB() const;
    
  public:
  };

  Lex* OSI_API_CALL LexInit(SemanticDB& tokenDB);
}

#endif
