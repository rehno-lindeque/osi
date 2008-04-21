#ifndef __QRAYRE_BIH_TYPES_H__
#define __QRAYRE_BIH_TYPES_H__
//////////////////////////////////////////////////////////////////////////////
//
//    TYPES.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      Types for use with algorithms relating to Binary Interval Hierarchies
*/

enum BIH_AXIS
{
  X_AXIS = 0x0,
  Y_AXIS = 0x1,
  Z_AXIS = 0x2
};

enum BIH_NODE_TYPE
{
  X_AXIS_SPLIT = 0x0,
  Y_AXIS_SPLIT = 0x1,
  Z_AXIS_SPLIT = 0x2,
  LEAF         = 0x3
};

struct BIH_NodeHeader
{
  BIH_NODE_TYPE nodeType;
  union
  {
    struct
    {
      OSfloat leftBoundary;
      OSfloat rightBoundary;
    };
    OSfloat boundaries[2];
  };
  OSuint32 rightOffset;
};

struct BIH_LeafNodeHeader
{
  BIH_NODE_TYPE nodeType;
  OSuint32 primitiveOffset;
  OSuint32 nPrimitives;
  OSuint32 reserved;
};

#endif
