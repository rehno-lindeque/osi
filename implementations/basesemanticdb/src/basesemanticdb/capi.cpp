//////////////////////////////////////////////////////////////////////////////
//
//    CAPI.CPP
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              IMPLEMENTATION                              */
#if defined(OSI_C_STATIC_BUILD) || defined(OSI_C_DYNAMIC_BUILD)

#include <osix/semanticdb/semanticdb.hpp>
#include <osix/semanticdb/semanticdb.h>
#include <base/common/types.h>

OSIX::SemanticDB *semanticDB = null;

/* debug */
//void* OSI_API_C_CALL debugInit() { return semanticDB->debugInit(); }

void OSI_API_C_CALL semanticDBInit() { semanticDB = OSIX::semanticDBInit(); }

#endif
