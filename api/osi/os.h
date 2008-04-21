#ifndef __OSI_OS_H__
#define __OSI_OS_H__
//////////////////////////////////////////////////////////////////////////////
//
//    OS.H
//
//    Ver 0.2 (2006-09-02)
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*
    DESCRIPTION:
      Open Standard Interface header
*/
/*                                 CONSTANTS                                */
#define OS_VERSION 0.2

/*                                   TYPES                                  */
typedef int                 OSint;
typedef unsigned int        OSuint;
typedef unsigned long long  OSuint64;
typedef unsigned int        OSuint32;
typedef unsigned short      OSuint16;
typedef unsigned char       OSuint8;
typedef long long           OSint64;
typedef int                 OSint32;
typedef short               OSint16;
typedef char                OSint8;
typedef float               OSfloat;
typedef double              OSdouble;
typedef char                OSchar;
typedef bool                OSbool;
typedef char*               OSstring;
typedef void*               OSpointer;

#if OSuint64 == OSpointer
  typedef OSuint64            OSid;
#else
  typedef OSuint              OSid;
#endif

typedef OSid                OSobject;
typedef OSuint              OSenum;

typedef struct { OSenum type; OSid data; } OSparam;

enum OS_BASIC_TYPE
{
  OS_INT8         = 0x00000000 +   sizeof(OSint8),
  OS_VEC2_INT8    = 0x00000000 + 2*sizeof(OSint8),
  OS_VEC3_INT8    = 0x00000000 + 3*sizeof(OSint8),
  OS_VEC4_INT8    = 0x00000000 + 4*sizeof(OSint8),

  OS_INT16        = 0x10000000 +   sizeof(OSint16),
  OS_VEC2_INT16   = 0x10000000 + 2*sizeof(OSint16),
  OS_VEC3_INT16   = 0x10000000 + 3*sizeof(OSint16),
  OS_VEC4_INT16   = 0x10000000 + 4*sizeof(OSint16),

  OS_INT32        = 0x20000000 +   sizeof(OSint32),
  OS_VEC2_INT32   = 0x20000000 + 2*sizeof(OSint32),
  OS_VEC3_INT32   = 0x20000000 + 3*sizeof(OSint32),
  OS_VEC4_INT32   = 0x20000000 + 4*sizeof(OSint32),

  OS_UINT8        = 0x30000000 +   sizeof(OSuint8),
  OS_VEC2_UINT8   = 0x30000000 + 2*sizeof(OSuint8),
  OS_VEC3_UINT8   = 0x30000000 + 3*sizeof(OSuint8),
  OS_VEC4_UINT8   = 0x30000000 + 4*sizeof(OSuint8),

  OS_UINT16       = 0x40000000 +   sizeof(OSuint16),
  OS_VEC2_UINT16  = 0x40000000 + 2*sizeof(OSuint16),
  OS_VEC3_UINT16  = 0x40000000 + 3*sizeof(OSuint16),
  OS_VEC4_UINT16  = 0x40000000 + 4*sizeof(OSuint16),

  OS_UINT32       = 0x50000000 +   sizeof(OSuint32),
  OS_VEC2_UINT32  = 0x50000000 + 2*sizeof(OSuint32),
  OS_VEC3_UINT32  = 0x50000000 + 3*sizeof(OSuint32),
  OS_VEC4_UINT32  = 0x50000000 + 4*sizeof(OSuint32),

  OS_FLOAT        = 0x60000000 +   sizeof(OSfloat),
  OS_VEC2_FLOAT   = 0x60000000 + 2*sizeof(OSfloat),
  OS_VEC3_FLOAT   = 0x60000000 + 3*sizeof(OSfloat),
  OS_VEC4_FLOAT   = 0x60000000 + 4*sizeof(OSfloat),

  OS_DOUBLE       = 0x70000000 +   sizeof(OSdouble),
  OS_VEC2_DOUBLE  = 0x70000000 + 2*sizeof(OSdouble),
  OS_VEC3_DOUBLE  = 0x70000000 + 3*sizeof(OSdouble),
  OS_VEC4_DOUBLE  = 0x70000000 + 4*sizeof(OSdouble)
};

enum OS_PARAM_TYPE
{
  OS_INT8_REFERENCE,
  OS_INT16_REFERENCE,
  OS_INT32_REFERENCE,
  OS_UINT8_REFERENCE,
  OS_UINT16_REFERENCE,
  OS_UINT32_REFERENCE,
  OS_FLOAT_REFERENCE,
  OS_DOUBLE_REFERENCE,
  OS_BOOL_REFERENCE,
  OS_CHAR_REFERENCE,
  OS_STRING_REFERENCE
};

/*                                   MACROS                                 */
/* calling convention */
#define OSI_API_C_CALL

#if defined(__GNUC__)
  #define OSI_API_CALL
#elif defined(_MSC_VER)
  #define OSI_API_CALL __stdcall
#else
  #define OSI_API_CALL
#endif

/*                                 FUNCTIONS                                */
/* basic type */
inline OSint osGetBasicTypeSize(enum OS_BASIC_TYPE type) { return type & 0xffff; }

/* parameter *
inline OSparam osReferenceParam(OSint8* reference)  { OSparam p = { OS_INT8_REFERENCE,   (OSid)reference }; return p; }
inline OSparam osReferenceParam(OSint16* reference) { OSparam p = { OS_INT16_REFERENCE,  (OSid)reference }; return p; }
inline OSparam osReferenceParam(OSint32* reference) { OSparam p = { OS_INT32_REFERENCE,  (OSid)reference }; return p; }
inline OSparam osReferenceParam(OSuint8* reference) { OSparam p = { OS_UINT8_REFERENCE,  (OSid)reference }; return p; }
inline OSparam osReferenceParam(OSuint16* reference){ OSparam p = { OS_UINT16_REFERENCE, (OSid)reference }; return p; }
inline OSparam osReferenceParam(OSuint32* reference){ OSparam p = { OS_UINT32_REFERENCE, (OSid)reference }; return p; }
inline OSparam osReferenceParam(OSfloat* reference) { OSparam p = { OS_FLOAT_REFERENCE,  (OSid)reference }; return p; }
inline OSparam osReferenceParam(OSdouble* reference){ OSparam p = { OS_DOUBLE_REFERENCE, (OSid)reference }; return p; }
inline OSparam osReferenceParam(OSbool* reference)  { OSparam p = { OS_BOOL_REFERENCE,   (OSid)reference }; return p; }
//inline OSparam osReferenceParam(OSchar* reference)  { OSparam p = { OS_CHAR_REFERENCE,   (OSid)reference }; return p; }
//inline OSparam osReferenceParam(OSstring reference) { OSparam p = { OS_STRING_REFERENCE, (OSid)reference }; return p; }*/

#endif
