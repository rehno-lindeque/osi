#ifdef __BASE_AXISALIGNEDBOUNDINGBOX_H__
#ifndef __BASE_AXISALIGNEDBOUNDINGBOX_HXX__
#define __BASE_AXISALIGNEDBOUNDINGBOX_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    AXISALIGNEDBOUNDINGBOX.HXX
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
FORCE_INLINE _AxisAlignedBoundingBox<Type>::_AxisAlignedBoundingBox(const Type* aabb)
{
  memcpy(_AxisAlignedBoundingBox<Type>::aabb, aabb, sizeof(Type)*6);
}

template<class Type>
FORCE_INLINE _AxisAlignedBoundingBox<Type>::_AxisAlignedBoundingBox(Type x1, Type x2, Type y1, Type y2, Type z1, Type z2)
{
  _AxisAlignedBoundingBox<Type>::x1 = x1;
  _AxisAlignedBoundingBox<Type>::x2 = x2;
  _AxisAlignedBoundingBox<Type>::y1 = y1;
  _AxisAlignedBoundingBox<Type>::y2 = y2;
  _AxisAlignedBoundingBox<Type>::z1 = z1;
  _AxisAlignedBoundingBox<Type>::z2 = z2;
}

template<class Type>
FORCE_INLINE _AxisAlignedBoundingBox<Type>::_AxisAlignedBoundingBox(const _AxisAlignedBoundingBox& boundingBox)
{
  memcpy(aabb, boundingBox.aabb, sizeof(Type)*6);
}

template<class Type>
FORCE_INLINE Type& _AxisAlignedBoundingBox<Type>::operator() (uint n)
{
  return aabb[n];
}

template<class Type>
FORCE_INLINE Type _AxisAlignedBoundingBox<Type>::operator() (uint n) const
{
  return aabb[n];
}

template<class Type>
FORCE_INLINE _AxisAlignedBoundingBox<Type>::operator const Type* () const
{
  return aabb;
}

template<class Type>
INLINE _AxisAlignedBoundingBox<Type>& _AxisAlignedBoundingBox<Type>::apply_union(const _AxisAlignedBoundingBox<Type>& aabb)
{
  //todo: SSE optimization (?)
  x1 = min(x1, aabb.x1);
  y1 = min(y1, aabb.y1);
  z1 = min(z1, aabb.z1);
  x2 = max(x2, aabb.x2);
  y2 = max(y2, aabb.y2);
  z2 = max(z2, aabb.z2);
  return *this;
}

template<class Type>
INLINE bool _AxisAlignedBoundingBox<Type>::contains(const _Vector3<Type>& v) const
{
  // todo: SSE optimization (?)
  return v.x >= x1 && v.x <= x2
      && v.y >= y1 && v.y <= y2
      && v.z >= z1 && v.z <= z2;
}

#endif
#endif
