#ifndef __BASEGE_ELEMENTS_H__
#define __BASEGE_ELEMENTS_H__
//////////////////////////////////////////////////////////////////////////////
//
//    ELEMENTS.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Basic elements class for OpenGE backends.

    DOCUMENTATION
*/

namespace BaseGE
{
/*                                  CLASSES                                 */
  class Elements : public Base::Object
  {
  protected:
    OS_BASIC_TYPE type;
    OSuint nElements;
    void* elements;

    template<typename Type1, typename Type2>
    inline void convertScalarMap(Type2* map)
    {
      for(uint c = 0; c < nElements; ++c)
        map[c] = (Type2)((Type1*)elements)[c];
    }

    template<typename Type1, int n1, typename Type2, int n2>
    inline void convertVectorMap(Type2* map)
    {
      const int n = min(n1, n2);

      for(uint c = 0; c < nElements; ++c)
        for(int cn = 0; cn < n; ++cn)
          map[c*n2+cn] = (Type2)((Type1*)elements)[c*n1+cn];
    }

  public:
    inline Elements() : type((OS_BASIC_TYPE)0), nElements(0), elements(0)
    {}

    inline void init(OS_BASIC_TYPE type)
    {
      Elements::type = type;
    }

    inline void add(OSuint nElements, void* elements)
    {
      void* tmpElements = Elements::elements;
      int typeSize = osGetBasicTypeSize(type);

      Elements::nElements += nElements;
      Elements::elements = new OSuint8[typeSize*Elements::nElements];

      if(tmpElements == 0)
        memcpy(Elements::elements, elements, osGetBasicTypeSize(type)*nElements);
      else
      {
        memcpy(Elements::elements, tmpElements, Elements::nElements - nElements);
        memcpy((uint8*)Elements::elements + typeSize*(Elements::nElements-nElements), elements, nElements);
        delete[] tmpElements;
      }
    }

    inline OS_BASIC_TYPE getType() const { return type; }

    inline OSuint getLength() const { return nElements; }

    inline void* getData() { return elements; }

    void* mapData(OS_BASIC_TYPE type, bool forceCopy)
    {
      void* map = 0;

      if(type == Elements::type)
      {
        if(forceCopy == false)
          return elements;
        map = new uint16[nElements];
        memcpy(map, elements, nElements * osGetBasicTypeSize(type));
        return map;
      }

      switch(type)
      {
      case OS_UINT16:
        map = new uint16[nElements];

        switch(Elements::type)
        {
        case OS_UINT32: convertScalarMap<uint32, uint16>((uint16*)map); break;
        case OS_UINT8:  convertScalarMap<uint8, uint16>((uint16*)map); break;
        case OS_FLOAT:  convertScalarMap<float, uint16>((uint16*)map); break;
        case OS_DOUBLE: convertScalarMap<double, uint16>((uint16*)map); break;
        case OS_INT32:  convertScalarMap<int32, uint16>((uint16*)map); break;
        case OS_INT8:   convertScalarMap<int8, uint16>((uint16*)map); break;
        }
        break;

      case OS_VEC3_FLOAT:
        map = new float[nElements*3];

        switch(Elements::type)
        {
        case OS_VEC4_FLOAT:  convertVectorMap<float,  4, float, 3>((float*)map); break;
        case OS_VEC4_DOUBLE: convertVectorMap<double, 4, float, 3>((float*)map); break;
        }
        break;
      }

      return map;
    }

    inline void unmapData(void* map)
    {
      if(map != elements)
        delete[] map;
    }
  };
}

#endif
