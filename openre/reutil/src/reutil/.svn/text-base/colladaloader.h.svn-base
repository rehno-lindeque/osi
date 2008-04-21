#ifndef __REUTIL_COLLADALOADER_H__
#define __REUTIL_COLLADALOADER_H__
//////////////////////////////////////////////////////////////////////////////
//
//    COLLADALOADER.H
//
//    Copyright © 2006 Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      Loads a collada file into OpenRE

    IMPLEMENTATION:
      Instancing now works for elements. If the same source is referenced
      more than once, it will re-use the same elements id.

    TODO:
      + Allow sharing of points if all the same elements are contained
        (locations, normals, colors etc)
    
    BUGS: 
      + Some objects will be created even if some obligatory components are
        missing. 
      + In general, the loader is not very robust. It needs to be thoroughly
        checked for points of failure and possibly memory leaks.
        Currently the loader assumes that the input file is "reasonably
        correct".
*/

/*                                 INCLUDES                                 */
#include "fileloader.h"

namespace REUtil {

/*                                  CLASSES                                  */
class ColladaLoader : public FileLoader
{
public:
	enum LoadOption
	{
		LOAD_EVERYTHING = 0,
		LOAD_SCENES,
		LOAD_MAIN_SCENE 
	};
	
protected:
	LoadOption loadOption;
	ColladaLoader();

public:
	uint nScenes, nMeshes, nCameras;
	REid* scenes,* meshes,* cameras;
	
	static ColladaLoader* create();
	virtual ~ColladaLoader();
	
  void destroyData();
	void load(const char* fileName, LoadOption loadOption);
  virtual void load(const char* fileName);
  virtual void destroyObjects();
};

}

#endif
