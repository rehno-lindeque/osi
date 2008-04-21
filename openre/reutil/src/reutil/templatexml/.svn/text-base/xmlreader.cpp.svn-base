//////////////////////////////////////////////////////////////////////////////
//
//    XMLREADER.CPP
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
#include "parse/parse.h"
#include "xmlreader.h"
using namespace TemplateXML;

#include <stdio.h>

//#include <fstream>
#include <vector>
using namespace std;

// types
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef int int32;
typedef short int16;
typedef char int8;

// File
#pragma warning(push)
#pragma warning(disable:4996)
char* loadFileData(const char* fileName)
{
  int fileLength;
  char* data = 0;

  // open file
  FILE* file = fopen(fileName, "rb");
  if(file == 0)
    throw(0);

  // get file length
  if(fseek(file,0,SEEK_END) != 0)
    return 0;
  fileLength = ftell(file);
  if(fseek(file, 0, SEEK_SET) != 0)
    return 0;

  // create storage buffer (with size increased by three for easy parsing)
  data = new char[fileLength+3];

  // read file into buffer
  fileLength = (int)fread(data, 1, fileLength, file);
  if(ferror(file) != 0)
  {
    delete[] data;
    throw(0);
  }
  
  // set final null characters
  data[fileLength+0] = '\0';
  data[fileLength+1] = '\0';
  data[fileLength+2] = '\0';

  // close file
  if(fclose(file) != 0)
  {
    delete[] data;
    throw(0);
  }

  return data;
}
#pragma warning(pop)

// Memory pools:
#define MEMPOOL_SIZE 256*256
vector<void*> memoryPools;
void* currentPool = 0;
void* poolPosition = 0;

// Xml Database
inline XmlDatabaseAttribute* allocateXmlDatabaseAttributes(int count)
{
  void* data;
  if((MEMPOOL_SIZE - ((char*)poolPosition - (char*)currentPool)) < (int)sizeof(XmlDatabaseAttribute)*count)
  {
    poolPosition = currentPool = new char[MEMPOOL_SIZE];
    memoryPools.push_back(currentPool);
  }
  data = poolPosition;
  poolPosition = (char*)poolPosition + sizeof(XmlDatabaseAttribute)*count;
  return (XmlDatabaseAttribute*)data;
}

inline XmlDatabaseElement* allocateXmlDatabaseElements(int count)
{
  void* data;
  if((MEMPOOL_SIZE - ((char*)poolPosition - (char*)currentPool)) < (int)sizeof(XmlDatabaseElement)*count)
  {
    poolPosition = currentPool = new char[MEMPOOL_SIZE];
    memoryPools.push_back(currentPool);
  }
  data = poolPosition;
  poolPosition = (char*)poolPosition + sizeof(XmlDatabaseElement)*count;
  return (XmlDatabaseElement*)data;
}

// Parse
inline bool isEndTag(const char* position)
{
  return *(uint16*)position == *(const uint16*)"</";
}

inline bool isEndOfComment(const char* position)
{
  return (*(uint32*)position & 0xffffff00) == *(const uint32*)"-->\0";
};

inline bool isStartOfComment(const char* position)
{
  return *(uint32*)position == *(const uint32*)"<!--";
}

void findStartOrEndTag(char*& position)
{
  while(position != '\0')
  {
    if(*position == '<')
    {
      // skip comment
      if(isStartOfComment(position))
      {
        position += 4;

        while(position != '\0' && !isEndOfComment(position))
          ++position;

        continue;
      }
      
      // start of tag found
      return;
    }

    if(*(uint16*)position == *(uint16*)"</")
      return; //end of tag found
 
    ++position;
  }
  throw(0);
}

void findStartTag(char*& position)
{
  while(position != '\0')
  {
    if(*position == '<')
    {
      // skip comment
      if(isStartOfComment(position))
      {
        position += 4;

        while(position != '\0' && !isEndOfComment(position))
          ++position;

        continue;
      }
      
      // start of tag found
      return;
    }

    ++position;
  }
  throw(0);
}

void parseAttribute(XmlDatabaseAttribute* attribute, char*& position)
{
  //note: position must already be at attribute (i.e. first name character)

  attribute->name = position;
  
  while(*position != '\0')
  {
    // find end of attribute name
    switch(*position)
    {
    case ' ': case '=':
      // delimit attribute name
      *position = '\0';
      
      // find attribute value
      do
      {
        ++position;
        if(*position == '"')
        {
          ++position;
          attribute->value = position;
          
          // find end of attribute
          while(*position != '\0')
          {
            if(*position == '"')
            {
              *position = '\0';
              ++position;
              return;
            }
            ++position;
          }
          throw(0);
        }
      } while(*position != '\0');
      throw(0);
    }
    ++position;
  }
  throw(0);
}

void parseTag(XmlDatabaseElement* element, char*& position)
{
  //note: position must already be at element (i.e. '<' character)

  vector<XmlDatabaseElement> subElements;
  vector<XmlDatabaseAttribute> attributes;
  char* contents = 0;

  ++position;
  element->name = position;
  element->subElements = 0;
  
  //find end of element name
  while(*position != '\0')
  {
    switch(*position)
    {
    case ' ': case '\t': case '\r': case '\n':
      // delimit element name
      *position = '\0';

      ++position;

      // parse attributes
      while(*position != '\0')
      {
        // parse end of tag
        if(*position == '>')
        {
          ++position;
          break; // note: continue parsing sub elements
        }
        
        // parse end of short tag
        if(*(uint16*)position == *(const uint16*)"/>")
        {
          position += 2;
          
          // copy attributes
          {
            int c = 0;
            element->attributeCount = (int)attributes.size();
            element->attributes = allocateXmlDatabaseAttributes(element->attributeCount);
            for(vector<XmlDatabaseAttribute>::iterator i = attributes.begin(); i != attributes.end(); ++i, ++c)
              memcpy(&element->attributes[c], &(XmlDatabaseAttribute&)*i, sizeof(XmlDatabaseAttribute));
          }
          
          // copy subelements
          element->elementCount = 0;
          element->contents = 0;

          return;
        }

        // parse individual attribute
        switch(*position)
        {
        case ' ': case '\t': case '\r': case '\n':
          ++position;
          continue;
        }
        XmlDatabaseAttribute attribute;
        parseAttribute(&attribute, position);
        attributes.push_back(attribute);
      }
      if(*position == '\0')
        throw(0);
      break;
    case '/':
      //parse end of short tag
      if(*(uint16*)position == *(const uint16*)"/>")
      {
        position += 2;

        // copy attributes
        {
          int c = 0;
          element->attributeCount = (int)attributes.size();
          element->attributes = allocateXmlDatabaseAttributes(element->attributeCount);
          for(vector<XmlDatabaseAttribute>::iterator i = attributes.begin(); i != attributes.end(); ++i, ++c)
            memcpy(&element->attributes[c], &(XmlDatabaseAttribute&)*i, sizeof(XmlDatabaseAttribute));
        }
        
        // copy subelements
        element->elementCount = 0;
        element->contents = 0;
        return;
      }
      
      ++position;
      continue;
    case '>': 
      // delimit element name
      *position = '\0';

      ++position;
      break;
    default:
      ++position;
      continue;
    }

    // set contents pointer
    contents = position;
    
    // parse sub elements
    while(*position != '\0')
    {
      // parse end of tag
      findStartOrEndTag(position);
      if(isEndTag(position))
      {
        // delimit element contents
        *position = '\0';

        // copy attributes
        {
          int c = 0;
          element->attributeCount = (int)attributes.size();
          element->attributes = allocateXmlDatabaseAttributes(element->attributeCount);
          for(vector<XmlDatabaseAttribute>::iterator i = attributes.begin(); i != attributes.end(); ++i, ++c)
            memcpy(&element->attributes[c], &(XmlDatabaseAttribute&)*i, sizeof(XmlDatabaseAttribute));
        }

        // copy sub elements
        element->elementCount = (int)subElements.size();
        if(element->elementCount == 0)
          element->contents = contents;
        else
        {
          int c = 0;
          element->subElements = allocateXmlDatabaseElements(element->elementCount);
          for(vector<XmlDatabaseElement>::iterator i = subElements.begin(); i != subElements.end(); ++i, ++c)
            memcpy(&element->subElements[c], &(XmlDatabaseElement&)*i, sizeof(XmlDatabaseElement));
        }
        
        // skip past end of tag
        position += 2;
        while(*position != '\0')
        {
          if(*position == '>')
          {
            ++position;
            return;
          }
          ++position;
        }
        throw(0);
      }
      
      // parse sub element
      XmlDatabaseElement subElement;
      parseTag(&subElement, position);
      subElements.push_back(subElement);
    }
    throw(0);
  }
  throw(0);
}

void parseRoot(XmlDatabaseElement* root, char*& position)
{
  findStartTag(position);
  parseTag(root, position);
}

inline bool isNewline(const char* position)
{
  return (*(uint16*)position == *(uint16*)"\r\n") || (*position == '\n');
}

/*inline void isWhitespace(const char* position)
{
  return (*(uint16*)position == *(uint16*)"\r\n") || (*position == '\n'); etc
}*/

void parseXmlHeader(char*& position)
{
  while(*position != '\0')
  {
    // skip white space
    switch(*position)
    { 
    case ' ': case '\t': case '\n': case '\r':    
      ++position;
      continue;
    }

    // skip comments
    if(isStartOfComment(position))
    {
      position += 4;
      while(*position != '\0' && !isEndOfComment(position))
        ++position;
      continue;
    }

    // find start of tag
    if(*(uint32*)position == *(const uint32*)"<?xm")
    {
      position += 4;
      if(*position == 'l')
      {
        switch(position[1])
        {
        case ' ': case '\n': case '\r': case '\t': case '?':
          ++position;
          // find end of tag
          while(*position != 0)
          {
            if(*(uint16*)position == *(const uint16*)"?>")
            {
              position += 2;
              return;
            }
            ++position;
          }
          throw(0);
        }
      }
    }
    position++;
  }
  throw(0);
}

// API
void XmlBase::buildXmlDatabase(const char* fileName)
{
  char* position = 0;
  fileData = 0;

  // map file
  position = fileData = loadFileData(fileName);

  // create initial memory pool
  poolPosition = currentPool = new char[MEMPOOL_SIZE];
  memoryPools.push_back(currentPool);

  // parse xml header
  parseXmlHeader(position);

  // parse xml (build database)
  parseRoot(&root, position);
}

void XmlBase::destroyXmlDatabase()
{
  if(fileData != 0)
    delete[] fileData;
  for(vector<void*>::iterator i = memoryPools.begin(); i != memoryPools.end(); ++i)
    delete[] (char*)*i;
  memoryPools.clear();
}
