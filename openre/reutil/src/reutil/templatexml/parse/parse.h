#ifndef __TEMPLATEXML_PARSE_H__
#define __TEMPLATEXML_PARSE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    PARSE.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      General parse routines and types for TemplateXML
*/
namespace TemplateXML {

struct XmlDatabaseAttribute
{ char* name; char* value; };

struct XmlDatabaseElement
{ char* name; 
  int elementCount; union { XmlDatabaseElement* subElements; char* contents; };
  int attributeCount; XmlDatabaseAttribute* attributes;
};

void parserAddId(const char* type, const char* id, void* element);
void* parserLookupId(const char* type, const char* id);
void* parserLookupId(const char* id);
void parserClearIdTable();

//int parseContents(double*& values, const char* contents);
int parseContents(float*& values, const char* contents);
int parseContents(int*& values, const char* contents);

}     

#endif
