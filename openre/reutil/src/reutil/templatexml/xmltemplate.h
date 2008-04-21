//////////////////////////////////////////////////////////////////////////////
//
//    XMLTEMPLATE.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      TemplateXML template macros
*/

//#include <stddef.h>
/*#ifndef offsetof
#define offsetof(str,member) (int)(int8*)(&((str*)0)->member)
#endif*/

/*                                   MACROS                                */
// Unroll & convert macro definitions to string
#ifndef PREPROC_STRING
#define PREPROC_STRING_UNROLL(str) #str
#define PREPROC_STRING(str) PREPROC_STRING_UNROLL(str)
#endif

// Reset loop
#ifdef TEMPLATEXML_LOOP_DONE
  #undef TEMPLATEXML_LOOP_DONE
#endif

// Increment loop counter
#ifndef TEMPLATEXML_LOOP_COUNTER
  #define TEMPLATEXML_LOOP_COUNTER 0
#elif TEMPLATEXML_LOOP_COUNTER == 0
  #undef TEMPLATEXML_LOOP_COUNTER
  #define TEMPLATEXML_LOOP_COUNTER 1
#elif TEMPLATEXML_LOOP_COUNTER == 1
  #undef TEMPLATEXML_LOOP_COUNTER
  #define TEMPLATEXML_LOOP_COUNTER 2
#elif TEMPLATEXML_LOOP_COUNTER == 2
  #undef TEMPLATEXML_LOOP_COUNTER
  #define TEMPLATEXML_LOOP_COUNTER 3
#elif TEMPLATEXML_LOOP_COUNTER == 3
  #undef TEMPLATEXML_LOOP_COUNTER
  #define TEMPLATEXML_LOOP_COUNTER 4
#elif TEMPLATEXML_LOOP_COUNTER == 4
  #undef TEMPLATEXML_LOOP_COUNTER
  #define TEMPLATEXML_LOOP_COUNTER 5
#elif TEMPLATEXML_LOOP_COUNTER == 5
  #undef TEMPLATEXML_LOOP_COUNTER
  #define TEMPLATEXML_LOOP_COUNTER 6
#elif TEMPLATEXML_LOOP_COUNTER == 6
  #undef TEMPLATEXML_LOOP_COUNTER
  #define TEMPLATEXML_LOOP_COUNTER 7
#else
  #undef TEMPLATEXML_LOOP_COUNTER
  #define TEMPLATEXML_LOOP_COUNTER 999
#endif

// define empty namespace
#ifndef TEMPLATEXML_TEMPLATE_NAMESPACE
  #define TEMPLATEXML_TEMPLATE_NAMESPACE
#endif

// debugging output
#ifdef _DEBUG
  #pragma message( "TemplateXML Loop counter: " PREPROC_STRING(TEMPLATEXML_LOOP_COUNTER) )
#endif

// Clear previous macros
#ifdef TEMPLATEXML_BEGIN_TEMPLATE
  #undef TEMPLATEXML_BEGIN_TEMPLATE
#endif
#ifdef TEMPLATEXML_END_TEMPLATE
  #undef TEMPLATEXML_END_TEMPLATE
#endif

#ifdef TEMPLATEXML_BEGIN_ELEMENT
  #undef TEMPLATEXML_BEGIN_ELEMENT
#endif
#ifdef TEMPLATEXML_END_ELEMENT
  #undef TEMPLATEXML_END_ELEMENT
#endif
#ifdef TEMPLATEXML_ELEMENT
  #undef TEMPLATEXML_ELEMENT
#endif
#ifdef TEMPLATEXML_ARRAY_ELEMENT
  #undef TEMPLATEXML_ARRAY_ELEMENT
#endif
#ifdef TEMPLATEXML_ARRAY_CONTENTS
  #undef TEMPLATEXML_ARRAY_CONTENTS
#endif

#ifdef TEMPLATEXML_ID_ATTRIBUTE
  #undef TEMPLATEXML_ID_ATTRIBUTE
#endif
#ifdef TEMPLATEXML_REFERENCE_ATTRIBUTE
  #undef TEMPLATEXML_REFERENCE_ATTRIBUTE
#endif
#ifdef TEMPLATEXML_VOID_REFERENCE_ATTRIBUTE
  #undef TEMPLATEXML_VOID_REFERENCE_ATTRIBUTE
#endif
#ifdef TEMPLATEXML_ENUM_ATTRIBUTE
  #undef TEMPLATEXML_ENUM_ATTRIBUTE
#endif

#ifdef TEMPLATEXML_BEGIN_ENUM
  #undef TEMPLATEXML_BEGIN_ENUM
#endif
#ifdef TEMPLATEXML_END_ENUM
  #undef TEMPLATEXML_END_ENUM
#endif
#ifdef TEMPLATEXML_ENUM
  #undef TEMPLATEXML_ENUM
#endif  

#if (TEMPLATEXML_LOOP_COUNTER == 0)

  // Create the data structure
  // Template
  #define TEMPLATEXML_BEGIN_TEMPLATE \
    namespace TEMPLATEXML_TEMPLATE_NAMESPACE {

  #define TEMPLATEXML_END_TEMPLATE \
    }

  // Elements
  #define TEMPLATEXML_BEGIN_ELEMENT(Type) \
    struct Type : public TemplateXML::XmlElementAddition<Type> \
    { 

  #define TEMPLATEXML_END_ELEMENT \
    };

  #define TEMPLATEXML_ELEMENT(Type, elementName, elementString) \
    TemplateXML::XmlElement<Type> elementName;
  
  #define TEMPLATEXML_ARRAY_ELEMENT(Type, elementName, elementString) \
    TemplateXML::XmlElement< TemplateXML::XmlArray<Type> > elementName;

  // Contents
  #define TEMPLATEXML_ARRAY_CONTENTS(Type) \
    /*Type* contents;*/ \
    TemplateXML::XmlArray<Type> contents;

  // Attributes
  #define TEMPLATEXML_ID_ATTRIBUTE(attributeString)

  #define TEMPLATEXML_REFERENCE_ATTRIBUTE(Type, attributeName, attributeString) \
    Type* attributeName;

  #define TEMPLATEXML_VOID_REFERENCE_ATTRIBUTE(attributeName, attributeString) \
    void* attributeName;

  #define TEMPLATEXML_ENUM_ATTRIBUTE(Type, attributeName, attributeString) \
    Type attributeName;

  // Enums
  #define TEMPLATEXML_BEGIN_ENUM(Type) \
    enum Type {

  #define TEMPLATEXML_END_ENUM \
    UNKNOWN };

  #define TEMPLATEXML_ENUM(enumName, enumString) \
    enumName,

#elif (TEMPLATEXML_LOOP_COUNTER == 1)
  
  // Count elements
  // Template
  #undef TEMPLATEXML_BEGIN_TEMPLATE
  #define TEMPLATEXML_BEGIN_TEMPLATE \
    namespace TemplateXML {

  #undef TEMPLATEXML_END_TEMPLATE
  #define TEMPLATEXML_END_TEMPLATE \
    }

  // Elements
  #define TEMPLATEXML_BEGIN_ELEMENT(Type) \
    template<> inline void countElements(TEMPLATEXML_TEMPLATE_NAMESPACE::Type& element, XmlDatabaseElement* dbElement) \
    { \
      XmlDatabaseElement* subElement; \
      if(dbElement->elementCount != 0) \
      { \
        for(int c = 0; c < dbElement->elementCount; ++c) \
        { \
          subElement = &dbElement->subElements[c];

  #define TEMPLATEXML_END_ELEMENT \
    {} }}}

  #define TEMPLATEXML_ELEMENT(Type, elementName, elementString) \
    if(_stricmp(subElement->name, elementString) == 0) \
    { ++element.elementName.length; } else

  #define TEMPLATEXML_ARRAY_ELEMENT(Type, elementName, elementString) \
    if(_stricmp(subElement->name, elementString) == 0) \
    { ++element.elementName.length; } else

  // Contents
  #define TEMPLATEXML_ARRAY_CONTENTS(Type)

  // Attributes
  #define TEMPLATEXML_ID_ATTRIBUTE(attributeString)
  #define TEMPLATEXML_REFERENCE_ATTRIBUTE(Type, attributeName, attributeString)
  #define TEMPLATEXML_VOID_REFERENCE_ATTRIBUTE(attributeName, attributeString)
  #define TEMPLATEXML_ENUM_ATTRIBUTE(Type, attributeName, attributeString)

  // Enums
# define TEMPLATEXML_BEGIN_ENUM(Type)
# define TEMPLATEXML_END_ENUM
# define TEMPLATEXML_ENUM(enumName, enumString)

#elif (TEMPLATEXML_LOOP_COUNTER == 2)
  
  // Allocate space for elements and attributes (and also reset the length to zero)  
  // Template
# undef TEMPLATEXML_BEGIN_TEMPLATE
# define TEMPLATEXML_BEGIN_TEMPLATE \
    namespace TemplateXML {

# undef TEMPLATEXML_END_TEMPLATE
# define TEMPLATEXML_END_TEMPLATE \
    }
  
  // Elements
# define TEMPLATEXML_BEGIN_ELEMENT(Type) \
    template<> inline void allocateElements(TEMPLATEXML_TEMPLATE_NAMESPACE::Type& element) \
    {

# define TEMPLATEXML_END_ELEMENT \
    }

# define TEMPLATEXML_ELEMENT(Type, elementName, elementString) \
    if(element.elementName.length > 0) \
    { \
      element.elementName.data = new TEMPLATEXML_TEMPLATE_NAMESPACE::Type[element.elementName.length]; \
      memset(element.elementName.data, 0, sizeof(TEMPLATEXML_TEMPLATE_NAMESPACE::Type) * element.elementName.length); \
      element.elementName.length = 0; /*(this is used as a counter for the construction code)*/ \
    }

# define TEMPLATEXML_ARRAY_ELEMENT(Type, elementName, elementString) \
    if(element.elementName.length > 0) \
    { \
      element.elementName.data = new XmlArray<Type>[element.elementName.length]; \
      memset(element.elementName.data, 0, sizeof(XmlArray<Type>) * element.elementName.length); \
      element.elementName.length = 0; /*(this is used as a counter for the construction code)*/ \
    }

  // Contents
# define TEMPLATEXML_ARRAY_CONTENTS(Type) \
    element.contents.data = 0; \
    element.contents.length = 0;

  // Attributes
# define TEMPLATEXML_ID_ATTRIBUTE(attributeString)
    
# define TEMPLATEXML_REFERENCE_ATTRIBUTE(Type, attributeName, attributeString) \
    element.attributeName = 0;

# define TEMPLATEXML_VOID_REFERENCE_ATTRIBUTE(attributeName, attributeString) \
    element.attributeName = 0;

# define TEMPLATEXML_ENUM_ATTRIBUTE(Type, attributeName, attributeString) \
    element.attributeName = TEMPLATEXML_TEMPLATE_NAMESPACE::UNKNOWN;

  // Enums
# define TEMPLATEXML_BEGIN_ENUM(Type)
# define TEMPLATEXML_END_ENUM
# define TEMPLATEXML_ENUM(enumName, enumString)

#elif (TEMPLATEXML_LOOP_COUNTER == 3)
  
  // Create the parsing code for attributes & enums
  // Template
# undef TEMPLATEXML_BEGIN_TEMPLATE
# define TEMPLATEXML_BEGIN_TEMPLATE \
    namespace TemplateXML {

# undef TEMPLATEXML_END_TEMPLATE
# define TEMPLATEXML_END_TEMPLATE \
    }

  // Elements
# define TEMPLATEXML_BEGIN_ELEMENT(Type) \
    template<> inline void constructAttributes(TEMPLATEXML_TEMPLATE_NAMESPACE::Type& element, XmlDatabaseElement* dbElement) \
    { \
      const char elementType[] = #Type; \
      XmlDatabaseAttribute* attribute; \
      for(int c = 0; c < dbElement->attributeCount; ++c) \
      { \
        attribute = &dbElement->attributes[c];
  
# define TEMPLATEXML_END_ELEMENT \
    {} }}

# define TEMPLATEXML_ELEMENT(Type, elementName, elementString)
# define TEMPLATEXML_ARRAY_ELEMENT(Type, elementName, elementString)

  // Contents
# define TEMPLATEXML_ARRAY_CONTENTS(Type)

  // Attributes
# define TEMPLATEXML_ID_ATTRIBUTE(attributeString) \
    if(_stricmp(attribute->name, attributeString) == 0) \
    { \
      TemplateXML::parserAddId(elementType, attribute->value, &element); \
    } else

# define TEMPLATEXML_REFERENCE_ATTRIBUTE(Type, attributeName, attributeString) \
    if(_stricmp(attribute->name, attributeString) == 0) \
    { \
      element.attributeName = (TEMPLATEXML_TEMPLATE_NAMESPACE::Type*)parserLookupId(#Type, attribute->value); \
    } else

# define TEMPLATEXML_VOID_REFERENCE_ATTRIBUTE(attributeName, attributeString) \
    if(_stricmp(attribute->name, attributeString) == 0) \
    { \
      element.attributeName = parserLookupId(attribute->value); \
    } else

# define TEMPLATEXML_ENUM_ATTRIBUTE(Type, attributeName, attributeString) \
    if(_stricmp(attribute->name, attributeString) == 0) \
    { \
      parseEnum<TEMPLATEXML_TEMPLATE_NAMESPACE::Type>(element.attributeName, attribute->value); \
    } else

  // Enums
# define TEMPLATEXML_BEGIN_ENUM(Type) \
    template<> inline void parseEnum(TEMPLATEXML_TEMPLATE_NAMESPACE::Type& enumAttribute, const char* value) {

# define TEMPLATEXML_END_ENUM \
    { enumAttribute = TEMPLATEXML_TEMPLATE_NAMESPACE::UNKNOWN; } }

# define TEMPLATEXML_ENUM(enumName, enumString) \
    if(_stricmp(value, enumString) == 0) \
    { \
      enumAttribute = TEMPLATEXML_TEMPLATE_NAMESPACE::enumName; \
    } else

#elif (TEMPLATEXML_LOOP_COUNTER == 4)
  
  // Create the parsing code for element contents
  // Template
# define TEMPLATEXML_BEGIN_TEMPLATE \
    namespace TemplateXML {

# define TEMPLATEXML_END_TEMPLATE \
    }

  // Elements
# define TEMPLATEXML_BEGIN_ELEMENT(Type) \
    template<> inline void constructContents(TEMPLATEXML_TEMPLATE_NAMESPACE::Type& element, XmlDatabaseElement* dbElement) \
    { 
      
# define TEMPLATEXML_END_ELEMENT \
    }

# define TEMPLATEXML_ELEMENT(Type, elementName, elementString)
    
# define TEMPLATEXML_ARRAY_ELEMENT(Type, elementName, elementString)

  // Contents
# define TEMPLATEXML_ARRAY_CONTENTS(Type) \
    /*constructArrayContents(element, dbElement);*/ \
    constructArrayElement(element.contents, dbElement);

  // Attributes
# define TEMPLATEXML_ID_ATTRIBUTE(attributeString)  
# define TEMPLATEXML_REFERENCE_ATTRIBUTE(Type, attributeName, attributeString)
# define TEMPLATEXML_VOID_REFERENCE_ATTRIBUTE(attributeName, attributeString)
# define TEMPLATEXML_ENUM_ATTRIBUTE(Type, attributeName, attributeString)

  // Enums
# define TEMPLATEXML_BEGIN_ENUM(Type)
# define TEMPLATEXML_END_ENUM
# define TEMPLATEXML_ENUM(enumName, enumString)

#elif (TEMPLATEXML_LOOP_COUNTER == 5)
  
  // Create the parsing code
  // Template
# define TEMPLATEXML_BEGIN_TEMPLATE \
    namespace TemplateXML {

# define TEMPLATEXML_END_TEMPLATE \
    }

  // Elements
# define TEMPLATEXML_BEGIN_ELEMENT(Type) \
    template<> void constructElement(TEMPLATEXML_TEMPLATE_NAMESPACE::Type& element, XmlDatabaseElement* dbElement) \
    { \
      XmlDatabaseElement* subElement; \
      countElements(element, dbElement); \
      allocateElements(element); \
      constructAttributes(element, dbElement); \
      if(dbElement->elementCount == 0) \
        constructContents(element, dbElement); \
      else \
      { \
        for(int c = 0; c < dbElement->elementCount; ++c) \
        { \
          subElement = &dbElement->subElements[c];
  
# define TEMPLATEXML_END_ELEMENT \
    {} }}}

# define TEMPLATEXML_ELEMENT(Type, elementName, elementString) \
    if(_stricmp(subElement->name, elementString) == 0) \
    { \
      constructElement(element.elementName.data[element.elementName.length], subElement); \
      ++element.elementName.length; \
    } else

# define TEMPLATEXML_ARRAY_ELEMENT(Type, elementName, elementString) \
    if(_stricmp(subElement->name, elementString) == 0) \
    { \
      constructArrayElement(element.elementName.data[element.elementName.length], subElement); \
      ++element.elementName.length; \
    } else

  // Contents
# define TEMPLATEXML_ARRAY_CONTENTS(Type)

  // Attributes
# define TEMPLATEXML_ID_ATTRIBUTE(attributeString)  
# define TEMPLATEXML_REFERENCE_ATTRIBUTE(Type, attributeName, attributeString)
# define TEMPLATEXML_VOID_REFERENCE_ATTRIBUTE(attributeName, attributeString)
# define TEMPLATEXML_ENUM_ATTRIBUTE(Type, attributeName, attributeString)

  // Enums
# define TEMPLATEXML_BEGIN_ENUM(Type)
# define TEMPLATEXML_END_ENUM
# define TEMPLATEXML_ENUM(enumName, enumString)

#elif (TEMPLATEXML_LOOP_COUNTER == 6)
  
  // Create the destruction code
  // Template
# define TEMPLATEXML_BEGIN_TEMPLATE \
    namespace TemplateXML {

# define TEMPLATEXML_END_TEMPLATE \
    }
  
  // Elements
# define TEMPLATEXML_BEGIN_ELEMENT(Type) \
    template<> inline void destroyElement(TEMPLATEXML_TEMPLATE_NAMESPACE::Type& element) \
    { \
      int c = 0;
  
# define TEMPLATEXML_END_ELEMENT \
    }

#  define TEMPLATEXML_ELEMENT(Type, elementName, elementString) \
    for(c = 0; c < element.elementName.length; ++c) \
      destroyElement<TEMPLATEXML_TEMPLATE_NAMESPACE::Type>(element.elementName.data[c]); \
    delete[] element.elementName.data;

# define TEMPLATEXML_ARRAY_ELEMENT(Type, elementName, elementString) \
    for(c = 0; c < element.elementName.length; ++c) \
      delete[] element.elementName.data[c].data; \
    delete[] element.elementName.data;
  
  // Contents
# define TEMPLATEXML_ARRAY_CONTENTS(Type) \
    /*delete[] element.contents;*/ \
    delete[] element.contents.data; \
    element.contents.data = 0;

  // Attributes
# define TEMPLATEXML_ID_ATTRIBUTE(attributeString)
# define TEMPLATEXML_REFERENCE_ATTRIBUTE(Type, attributeName, attributeString)
# define TEMPLATEXML_VOID_REFERENCE_ATTRIBUTE(attributeName, attributeString)
# define TEMPLATEXML_ENUM_ATTRIBUTE(Type, attributeName, attributeString)

  // Enums
# define TEMPLATEXML_BEGIN_ENUM(Type)
# define TEMPLATEXML_END_ENUM
# define TEMPLATEXML_ENUM(enumName, enumString)

#else
  // Reset macros & definitions
  // Template  
# define TEMPLATEXML_BEGIN_TEMPLATE
# define TEMPLATEXML_END_TEMPLATE

  // Elements
# define TEMPLATEXML_BEGIN_ELEMENT(Type)
# define TEMPLATEXML_END_ELEMENT
# define TEMPLATEXML_ELEMENT(Type, elementName, elementString)
# define TEMPLATEXML_ARRAY_ELEMENT(Type, elementName, elementString)

  // Contents
# define TEMPLATEXML_ARRAY_CONTENTS(Type)

  // Attributes
# define TEMPLATEXML_ID_ATTRIBUTE(attributeString)  
# define TEMPLATEXML_REFERENCE_ATTRIBUTE(Type, attributeName, attributeString)
# define TEMPLATEXML_VOID_REFERENCE_ATTRIBUTE(attributeName, attributeString)
# define TEMPLATEXML_ENUM_ATTRIBUTE(Type, attributeName, attributeString)

  // Enums
# define TEMPLATEXML_BEGIN_ENUM(Type)
# define TEMPLATEXML_END_ENUM
# define TEMPLATEXML_ENUM(enumName, enumString)
  
  // Set end-of-loop flag
# define TEMPLATEXML_LOOP_DONE
# undef TEMPLATEXML_LOOP_COUNTER

  // clear template namespace
# ifdef TEMPLATEXML_TEMPLATE_NAMESPACE
#   undef TEMPLATEXML_TEMPLATE_NAMESPACE
# endif
#endif
