#ifndef __REUTIL_FILELOADER_H__
#define __REUTIL_FILELOADER_H__
//////////////////////////////////////////////////////////////////////////////
//
//    FILELOADER.H
//
//    Copyright © 2006 Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      File loader base class.
*/

/*                                 INCLUDES                                 */
#include "exception.h"
#include "types.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <osi/re/re.h>

/*                                  CLASSES                                  */
namespace REUtil
{
class FileLoader
{
public:
  virtual void load(const char* fileName) = 0;
  virtual void destroyObjects() = 0;
  virtual ~FileLoader() {}
};
}

#endif
