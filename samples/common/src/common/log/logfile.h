#ifndef __SAMPLES_LOGFILE_H__
#define __SAMPLES_LOGFILE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    LOGFILE.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      Opens and writes to a simple log (text) file.
*/

/*                                 INCLUDES                                 */
#include "../file/file.h"

/*                                  CLASSES                                 */
class LogFile : public File
{
public:
	LogFile(const char* filename);
	~LogFile();
};

#endif
