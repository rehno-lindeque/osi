//////////////////////////////////////////////////////////////////////////////
//
//    CAPI.CPP
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

/*                              IMPLEMENTATION                              */
#if defined(OSI_C_STATIC_BUILD) || defined(OSI_C_SHARED_BUILD)

#include <osix/lex/lex.hpp>
#include <osix/lex/lex.h>
#include <base/common/types.h>

OSIX::Lex *lex = null;

/* debug */
//void* OSI_API_C_CALL debugInit() { return lex->debugInit(); }

void OSI_API_C_CALL LexInit() { lex = OSIX::lexInit(); }

#endif
