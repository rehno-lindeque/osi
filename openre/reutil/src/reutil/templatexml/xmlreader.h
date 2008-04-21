#ifndef __TEMPLATEXML_XMLREADER_H__
#define __TEMPLATEXML_XMLREADER_H__
//////////////////////////////////////////////////////////////////////////////
//
//    XMLREADER.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      TemplateXML reader

    WARNINGS:
      + XmlReader is not currently thread safe because it uses a global
        variable internally. (This should be relatively easy to fix though)

      + XmlReader assumes that xml files are well-formed and does not
        explicitly check the structure
    
    BUGS:
      + XmlReader may interperet a few obscure patterns wrong such as an
        '>' character in an attribute of the <?xml ... ?> header element
      
      + Similarly, comments inside of element tags are not allowed.
        (Not sure whether this is actually a bug though)
*/

namespace TemplateXML {

class XmlBase
{
protected:
  char* fileData;
  XmlDatabaseElement root;

  void buildXmlDatabase(const char* fileName);
  void destroyXmlDatabase();
};

template<class Type> class XmlReader : public XmlBase
{
public:
  inline void loadFile(const char* fileName, Type*& rootElement)
  { 
    buildXmlDatabase(fileName);
    constructRootElement(rootElement, &root);
    destroyXmlDatabase();
    parserClearIdTable();
  }

  inline void destroyData(Type*& rootElement)
  {
    destroyRootElement(rootElement);
  }
};

}

#endif
