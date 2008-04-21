#ifndef __BASE_AXISALIGNEDBOUNDINGBOX_H__
#define __BASE_AXISALIGNEDBOUNDINGBOX_H__
//////////////////////////////////////////////////////////////////////////////
//
//    AXISALIGNEDBOUNDINGBOX.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Axis aligned bounding box class

    DEPENDENCIES:
      math.h must be used
*/

/*                                CLASSES                                   */
template<class Type = float>
class _AxisAlignedBoundingBox
{
protected:
	typedef _AxisAlignedBoundingBox<Type> AxisAlignedBoundingBox;

#pragma pack(push,1)
  union {
    struct { Type x1, y1, z1, x2, y2, z2; };
    Type aabb[6];
  };
#pragma pack(pop)

public:
  // constructors
  FORCE_INLINE _AxisAlignedBoundingBox() {}
  FORCE_INLINE _AxisAlignedBoundingBox(const Type* aabb);
  FORCE_INLINE _AxisAlignedBoundingBox(const AxisAlignedBoundingBox& boundingBox);
  FORCE_INLINE _AxisAlignedBoundingBox(Type x1, Type x2, Type y1, Type y2, Type z1, Type z2);

  // destructor
  FORCE_INLINE ~_AxisAlignedBoundingBox() {}

  // access grants
  FORCE_INLINE Type& operator() (uint n);
  FORCE_INLINE Type  operator() (uint n) const;

  // type casts
  FORCE_INLINE operator const Type* () const;

  // bounding box operations
  INLINE AxisAlignedBoundingBox& apply_union(const AxisAlignedBoundingBox& aabb);
  INLINE bool contains(const _Vector3<Type>& v) const;
};

typedef _AxisAlignedBoundingBox<float> AxisAlignedBoundingBox;
typedef _AxisAlignedBoundingBox<float> AxisAlignedBoundingBoxf;
typedef _AxisAlignedBoundingBox<double> AxisAlignedBoundingBoxd;

#endif
