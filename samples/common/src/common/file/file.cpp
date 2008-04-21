//////////////////////////////////////////////////////////////////////////////
//
//    FILE.CPP
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                                 INCLUDES                                 */
#include "file.h"

using namespace std;

/*                               CLASS SOURCE                               */
File::File(const char* filename, ios::openmode flags) : fstream(filename, flags)
{

}

File::~File()
{

}
