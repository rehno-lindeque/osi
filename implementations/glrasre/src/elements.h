#ifndef __GLRASRE_ELEMENTS_H__
#define __GLRASRE_ELEMENTS_H__
//////////////////////////////////////////////////////////////////////////////
//
//    ELEMENTS.H
//
//    Author:      Rehno Lindeque
//
//    Description: GlRasRE classes used to implement api
//
//    Version: 0.1 (informal prototype)
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              COMPILER MACROS                             */
#ifndef __BASERE_STREAM_H__
  #error "stream.h must be included before elements.h"
#endif

using namespace BaseRE;
namespace GlRasRE
{
/*                                CLASSES                                   */
  class GlRasElements : public Elements
  {
  public:
    GlStream* stream;
    uint length;
    inline GlRasElements() : stream(null), length(0) {}

    virtual ~GlRasElements()
    {
      delete stream;
      stream = null;
    }

    void create(RE_BASIC_TYPE type, uint nElements)
    {
      if(stream != null)
        return; //error

      // create opengl stream for storage
      stream = new GlStream;
      StreamFormat streamFormat;
      switch(type)
      {
      case RE_FLOAT:      streamFormat = FLOAT16_STREAM; break;
      case RE_UINT16:     streamFormat = UINT16_STREAM; break;
      case RE_UINT32:     streamFormat = UINT32_STREAM; break;
      //todo: case RE_VEC2_FLOAT: streamFormat = VEC2_FLOAT16; break;
      //todo: case RE_VEC3_FLOAT: streamFormat = VEC3_FLOAT16; break;
      case RE_VEC3_FLOAT: streamFormat = VEC3_FLOAT16_STREAM; break;
      case RE_VEC4_FLOAT: streamFormat = VEC4_FLOAT16_STREAM; break;
      default: return; //error
      }

      stream->create(nElements, streamFormat, null);
      length = nElements;
    }

    inline void setElements(void* elements)
    {
      if(stream->isMapped())
        return; //error
      stream->setData(elements, length);
    }

    inline void addElement(void* element)
    {
      if(!stream->isMapped())
        stream->map(WRITE_ONLY);
      stream->appendCopy(element, 1);
    }
    /*void addElements(void* element, uint n)
    {
      if(!stream->isMapped())
        stream->map(WRITE_ONLY);
      stream->appendCopy(element, n);
    }*/

    inline void build()
    {
      if(stream->isMapped())
        stream->unmap();
    }

    inline GlStream* getStream()
    {
      return stream;
    }

    inline uint getLength() { return length; }
  };
};

#endif
