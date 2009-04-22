#ifndef __QPARSER_PARSERESULT_H__
#define __QPARSER_PARSERESULT_H__
//////////////////////////////////////////////////////////////////////////////
//
//    PARSERESULT.H
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      QParser parse result structure. Describes the outputs (and inputs) of 
      the parser.
*/

/*                                  CLASSES                                 */
namespace QParser
{
  struct ParseMatch : public OSIX::ParseMatch
  {
    FORCE_INLINE ParseMatch(uint16 offset, uint16 length, ParseToken token) { ParseMatch::offset = offset; ParseMatch::length = length; ParseMatch::token = token; }
    INLINE ParseMatch() {}
  };
  
  class ParseResult : public Base::Object
  {
  public:
    // Input data
    template<typename Type> struct Stream
    {
      uint  length;       // Number of elements in the stream
      uint  elementSize;  // Size of a single element in the stream
      Type* data;         // Data contained in the stream
    };
    Stream<const char> inputStream;

    // Parse matches
    Stream<ParseMatch> parseStream;
    uint globalMatchesLength;

    // Lexical token matches
    Stream<ParseMatch> lexStream;

    ParseResult() 
    { 
      memset(&inputStream, 0, sizeof(inputStream));
      memset(&parseStream, 0, sizeof(parseStream));
      memset(&lexStream, 0, sizeof(lexStream));
    }
    virtual ~ParseResult() { delete[] parseStream.data; delete[] lexStream.data; }
  };
}

#endif
