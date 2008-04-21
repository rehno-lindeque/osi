//////////////////////////////////////////////////////////////////////////////
//
//    PARSE.CPP
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*
    NOTES:
      + There are some differences between different implementations of the
        hash_map. Gnu compilers use the __gnu_cxx namespace for their
        implementation and store the header under the ext namespace while
        msvc use the stdext namespace.
        Furthermore it appears that the msvc compiler only has a string hash
        for std::string whereas gnu compilers only have a string hash for
        const char*.
        Finally the gnu compiler's hash_map doesn't use a string comparison by
        default, so a custom one had to be provided.
    
      + Floating point numbers now also support numbers in the format -1e-005
        Because the blender exporter appears to use this format for very small
        numbers
      
*/

#include "parse.h"

#include <vector>
using namespace std;

#ifdef __GNUC__
  #include <ext/hash_map>
  using namespace __gnu_cxx;
  
  struct string_equal : public binary_function<const char*, const char*, bool>
  {
    bool inline operator()(const char* s1, const char* s2) const { return strcmp(s1, s2) == 0; }
  };

  typedef hash_map<const char*, void*, hash<const char*>, string_equal> HashMap;

#else
  #include <hash_map>
  using namespace stdext;
  
  #include <string>
  typedef hash_map<string, void*> HashMap;
#endif

HashMap elementIds;

// types
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef int int32;
typedef short int16;
typedef char int8;

void TemplateXML::parserAddId(const char* type, const char* id, void* element)
{
  // construct a key from the type & the id
  static char key[255];
  size_t typeLen = strlen(type);
  memcpy(key, type, typeLen);
  memcpy(key+typeLen, id, strlen(id)+1);

  // insert key with value into hash table
  elementIds[key] = element;
}

void* TemplateXML::parserLookupId(const char* type, const char* id)
{
  //assert(id[0] == '#')

  // construct a key from the type & the id
  static char key[255];
  size_t typeLen = strlen(type);
  memcpy(key, type, typeLen);
  memcpy(key+typeLen, id+1, strlen(id+1)+1);

  // get value from hash table using key
  if(elementIds.count(key) == 0)
    return 0;
  return elementIds[key];
}

void* TemplateXML::parserLookupId(const char* id)
{
  //Note: this is pretty slow since it doesn't use the hash function

  const char* keyStr;
  int keyLen;
  int idLen;

  // calculate id length
  idLen = (int)strlen(id+1);

  // construct a key from the type & the id
  for(HashMap::iterator i = elementIds.begin(); i != elementIds.end(); ++i)
  {
#ifdef __GNUC__
    keyStr = i->first;
#else
    keyStr = i->first.c_str();
#endif
    keyLen = (int)strlen(keyStr);
    if(idLen >= keyLen)
      continue;

    keyStr += keyLen - idLen;
    if(_stricmp(keyStr, id+1) == 0)
      return i->second;
  }
  return 0;
}

void TemplateXML::parserClearIdTable()
{
  elementIds.clear();
}

inline bool isEndOfComment(const char* position)
{
  return (*(uint32*)position & 0xffffff00) == *(const uint32*)"-->\0";
};

inline bool isStartOfComment(const char* position)
{
  return *(uint32*)position == *(const uint32*)"<!--";
}

int TemplateXML::parseContents(float*& values, const char* contents)
{
  const char* position = contents;
  vector<float> vals;
  int value;
 
  while(*position != '\0')
  {
    int sign = 1; // positive
    value = 0;

    // skip non-numbers
    while(!((*position >= '0' && *position <= '9') || *position == '-'))
    {
      // skip comment
      if(isStartOfComment(position))
      {
        position += 4;
        while(*position != '\0')
        {
          if(isEndOfComment(position))
          {
            position += 3;
            break;
          }
          ++position;
        }
        if(*position == '\0')
          break; //throw(0);
      }
      ++position;
      if(*position == '\0')
        break; //throw(0);
    }

    // parse first number (before decimal point)
    {
      //parse negative sign
      if(*position == '-')
      {
        sign = -1; // negative
        ++position;
      }
      
      // parse numerals
      do
      {
        value *= 10;
        value += (int)(*position - '0');
        ++position;
      } while(*position >= '0' && *position <= '9');

      // parse  exponent
      if(*position == 'e' || *position == 'E')
      {
        bool positive = true;
        ++position;

        // parse negative sign
        if(*position == '-')
        {
          positive = false;
          ++position;
        }

        // parse exponent number
        int exp = 0;
        while(*position >= '0' && *position <= '9')
        {
          exp *= 10;
          exp += (int)(*position - '0');
          ++position;
        }
        
        // calculate float value
        double val = value;
        if(positive)
          while(exp > 0)
          {
            val *= 10;
            --exp;
          }
        else
          while(exp > 0)
          {
            val /= 10;
            --exp;
          }

        // add to list
        vals.push_back((float)sign*val);
        continue;
      }
    }

    // parse decimal point
    if(*position != '.')
    {
      vals.push_back((float)value);
      continue;
    }
    ++position;

    double val = value;
    double factor = 0.1;

    // parse second number (after decimal point)
    while(*position >= '0' && *position <= '9')
    {
      val += factor * (int)(*position - '0');
      factor *= 0.1;
      //++decimals;
      ++position;
    }

    // parse  exponent
    if(*position == 'e' || *position == 'E')
    {
      bool positive = true;
      ++position;

      // parse negative sign
      if(*position == '-')
      {
        positive = false;
        ++position;
      }

      // parse exponent number
      int exp = 0;
      while(*position >= '0' && *position <= '9')
      {
        exp *= 10;
        exp += (int)(*position - '0');
        ++position;
      }
      
      if(positive)
        while(exp > 0)
        {
          val *= 10;
          --exp;
        }
      else
        while(exp > 0)
        {
          val /= 10;
          --exp;
        }

      // add to list
      vals.push_back((float)sign*val);
      continue;
    }

    // add to list
    vals.push_back((float)sign*val);
  }

  // copy to array
  int length = (int)vals.size();
  int c = 0;
  values = new float[length];
  for(vector<float>::iterator i = vals.begin(); i != vals.end(); ++i, ++c)
    values[c] = *i;
  return length;
}


int TemplateXML::parseContents(int*& values, const char* contents)
{
  // todo: this doesn't parse negative values yet

  const char* position = contents;
  vector<int> vals;
  int value;
 
  while(*position != '\0')
  {
    value = 0;

    // skip non-numbers
    while(*position < '0' || *position > '9')
    {
      // skip comment
      if(isStartOfComment(position))
      {
        position += 4;
        while(*position != '\0')
        {
          if(isEndOfComment(position))
          {
            position += 3;
            break;
          }
          ++position;
        }
        if(*position == '\0')
          break; //throw(0);
      }
      ++position;
      if(*position == '\0')
        break; //throw(0);
    }

    // parse number
    do
    {
      value *= 10;
      value += (int)(*position - '0');
      ++position;
    } while(*position >= '0' && *position <= '9');

    // add to list
    vals.push_back(value);
  }

  // copy to array
  int length = (int)vals.size();
  int c = 0;
  values = new int[length];
  for(vector<int>::iterator i = vals.begin(); i != vals.end(); ++i, ++c)
    values[c] = *i;
  return length;
}