#ifndef __BASEGE_AXISALIGNEDBOUNDINGBOX_H__
#define __BASEGE_AXISALIGNEDBOUNDINGBOX_H__
//////////////////////////////////////////////////////////////////////////////
//
//    AXISALIGNEDBOUNDINGBOX.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
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
  inline _AxisAlignedBoundingBox() {}
  inline _AxisAlignedBoundingBox(const Type* aabb);
  inline _AxisAlignedBoundingBox(const AxisAlignedBoundingBox& boundingBox);
  inline _AxisAlignedBoundingBox(Type x1, Type x2, Type y1, Type y2, Type z1, Type z2);
    
  // destructor
  inline ~_AxisAlignedBoundingBox() {}
  
  // access grants
  inline Type& operator() (uint n);
  inline Type  operator() (uint n) const;
  
  // type casts
  inline operator const Type* () const;

  // bounding box operations
  inline void apply_union(AxisAlignedBoundingBox& b);
  inline bool contains(const _Vector3<Type>& v) const;
};

typedef _AxisAlignedBoundingBox<float> AxisAlignedBoundingBox;
typedef _AxisAlignedBoundingBox<float> AxisAlignedBoundingBoxf;
typedef _AxisAlignedBoundingBox<double> AxisAlignedBoundingBoxd;

#endif
