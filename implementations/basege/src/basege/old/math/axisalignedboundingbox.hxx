#ifdef __BASEGE_AXISALIGNEDBOUNDINGBOX_H__
#ifndef __BASEGE_AXISALIGNEDBOUNDINGBOX_HXX__
#define __BASEGE_AXISALIGNEDBOUNDINGBOX_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    AXISALIGNEDBOUNDINGBOX.HXX
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
inline _AxisAlignedBoundingBox<Type>::_AxisAlignedBoundingBox(const Type* aabb)
{
  memcpy(_AxisAlignedBoundingBox<Type>::aabb, aabb, sizeof(Type)*6);
}

template<class Type>
inline _AxisAlignedBoundingBox<Type>::_AxisAlignedBoundingBox(Type x1, Type x2, Type y1, Type y2, Type z1, Type z2)
{
  _AxisAlignedBoundingBox<Type>::x1 = x1;
  _AxisAlignedBoundingBox<Type>::x2 = x2;
  _AxisAlignedBoundingBox<Type>::y1 = y1;
  _AxisAlignedBoundingBox<Type>::y2 = y2;
  _AxisAlignedBoundingBox<Type>::z1 = z1;
  _AxisAlignedBoundingBox<Type>::z2 = z2;
}

template<class Type>
inline _AxisAlignedBoundingBox<Type>::_AxisAlignedBoundingBox(const _AxisAlignedBoundingBox& boundingBox)
{
  memcpy(aabb, boundingBox.aabb, sizeof(Type)*6);
}

template<class Type>
inline Type& _AxisAlignedBoundingBox<Type>::operator() (uint n)
{
  return aabb[n];
}

template<class Type>
inline Type _AxisAlignedBoundingBox<Type>::operator() (uint n) const
{
  return aabb[n];
}

template<class Type>
inline _AxisAlignedBoundingBox<Type>::operator const Type* () const
{
  return aabb;
}

template<class Type>
inline void _AxisAlignedBoundingBox<Type>::apply_union(const _AxisAlignedBoundingBox<Type>& b)
{
  //todo: SSE optimization (?)
  x1 = min(x1, b.x1);
  y1 = min(y1, b.y1);
  z1 = min(z1, b.z1);
  x2 = max(x2, b.x2);
  y2 = max(y2, b.y2);
  z2 = max(z2, b.z2);
}

template<class Type>
inline bool _AxisAlignedBoundingBox<Type>::contains(const _Vector3<Type>& v) const
{
  // todo: SSE optimization (?)
  return v.x >= x1 && v.x <= x2
      && v.y >= y1 && v.y <= y2
      && v.z >= z1 && v.z <= z2;
}

#endif
#endif
