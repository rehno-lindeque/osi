#ifndef __SAMPLES_FILE_H__
#define __SAMPLES_FILE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    FILE.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      File utility class.

    USAGE:
      types.h must be included before this header

    IMPLEMENTATION:
      The std library's fstream class is wrapped by this class
*/

/*                                 INCLUDES                                 */
#include <fstream>

using namespace std;

/*                                  CLASSES                                 */
class File : public fstream
{
protected:
public:
  /* flags - combination of ios::openmode flags OR'd together
   */
  File(const char* filename, ios::openmode flags);
  ~File();
};

#endif
