//////////////////////////////////////////////////////////////////////////////
//
//    MAIN.CPP
//
//    Copyright (c) 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Sample renderer for a collada scene using an OpenRE implementation.
*/
/*                                  INCLUDES                                */

#include <windows.h>

/* Common*/
#include <common/types.h>
#include <common/time/time.h>
#include <common/log/logfile.h>
#include <math.h>

/* OpenRE */
#include <osi/re/re.h>
#include <rewrapper/openreapplication.h>
#include <reutil/colladaloader.h>

// #define USE_GLRAYRE
#define USE_GLRASRE
#if defined(USE_GLRAYRE)
  #include <osi/re/ext/glrayre.h>
#pragma lib
#elif defined(USE_GLRASRE)
  #include <osi/re/ext/glrasre.h>
#endif

#define DEBUG_LOG(msg) log << msg << endl; log.flush();

/*                                  CLASSES                                 */
class REColladaDisplay : public REWrapper::OpenREApp
{
protected:
  LogFile log;
  Time timer;
  uint64 startTime, endTime;
  REUtil::ColladaLoader* colladaLoader;

private:
  virtual void init()
  {
    log << "OpenRE Test log\n";
    log << "---------------\n";
    //log << "Scene setup time: ";

    startTime = timer.getCounter();

    // Initialize OpenRE
    DEBUG_LOG("Initializing OpenRE")
    reInit();
    reDisplayMode(RE_PERSISTENT);

    // Construct scene
    DEBUG_LOG("Construct Scene")
    
  	colladaLoader = REUtil::ColladaLoader::create();
  	DEBUG_LOG("Arguments:" << commandLine.arguments[0])

    try { colladaLoader->load(commandLine.arguments[0], REUtil::ColladaLoader::LOAD_MAIN_SCENE); }
  	catch(REUtil::Exception* e)
  	{
  		DEBUG_LOG("Collada Loader Failed: " << e->message)
  		return;
  	}

    // Construct Render
    DEBUG_LOG("Construct Render")
    REid scene = colladaLoader->scenes[0];
    REid render = reBeginRender(RE_FRAME_BUFFER, scene);
      // Use the first available camera
      if(colladaLoader->nCameras > 0)
        reCamera(colladaLoader->cameras[0]);
    reEndRender();

    // Render scene
    DEBUG_LOG("Render");
    reRender(render);

		// Delete Objects
		DEBUG_LOG("Delete Objects");
    //reDelObject(camera);
    reDelObject(render);
    reDelObject(scene);
    
    // Destroy objects
    colladaLoader->destroyObjects();
    //colladaLoader->destroyData();
		delete colladaLoader;
		colladaLoader = null;
    		
		// Display 
    reDisplay();
  }

  virtual void main()
  {
    reDisplay();
  }

  virtual void end()
  {
    reShutdown();
  }

public:
  REColladaDisplay() : OpenREApp("OpenRE Collada Scene Display. Copyright (c) 2006, Rehno Lindeque. All rights reserved."), log("test1.log") {}
}application;
