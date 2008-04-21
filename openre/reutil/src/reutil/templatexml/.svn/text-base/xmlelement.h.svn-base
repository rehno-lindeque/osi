#ifndef __TEMPLATEXML_XMLELEMENT_H__
#define __TEMPLATEXML_XMLELEMENT_H__
//////////////////////////////////////////////////////////////////////////////
//
//    XMLELEMENT.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      TemplateXML element data structure
    
    WARNING:
      + constructRootElement assumes that the root element is correct and
        does not check. (this shouldn't be a problem usualy, but it is
        possible (but tedious) to modify xmltemplate.h to draw this
        information from the template)
*/

namespace TemplateXML {

template<class Type> struct XmlElement
{ 
  int length;
  Type* data;
  inline operator Type* () { return data; }
};

template<class Type> struct XmlArray
{
  int length;
  Type* data;
  inline operator Type* () { return data; }
};

template<class Type> struct XmlElementAddition { };

// Construct Attributes
template<class Type> inline void constructAttributes(Type& element, XmlDatabaseElement* dbElement);

// Construct Elements
template<class Type> void constructElement(Type& element, XmlDatabaseElement* dbElement);

template<class Type> void constructRootElement(Type*& rootElement, XmlDatabaseElement* dbElement)
{  
  rootElement = new Type[1];
  memset(rootElement, 0, sizeof(Type));
  constructElement(*rootElement, dbElement);
}

template<class Type> void constructArrayElement(XmlArray<Type>& elementArray, XmlDatabaseElement* dbElement)
{
  elementArray.length = parseContents(elementArray.data, dbElement->contents);
}

// Construct Contents
template<class Type> void constructContents(Type& element, XmlDatabaseElement* dbElement);

/*template<class Type> void constructArrayContents(Type& element, XmlDatabaseElement* dbElement)
{
  element.length = parseContents(element.data->contents, dbElement->contents);
}*/

// Other
template<class Type> inline void parseEnum(Type& enumAttribute, const char* value);

template<class Type> inline void allocateElements(Type& element);

template<class Type> inline void countElements(Type& element, XmlDatabaseElement* dbElement);

template<class Type> void destroyElement(Type& element);

template<class Type> void destroyRootElement(Type*& rootElement)
{
  destroyElement(*rootElement);
  delete[] rootElement;
}

}

#endif
