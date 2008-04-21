#ifndef __BASE_QUATERNION_H__
#define __BASE_QUATERNION_H__
//////////////////////////////////////////////////////////////////////////////
//
//    QUATERNION.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Quaternion class

    DEPENDENCIES:
      math.h must be used
*/

/*                                CLASSES                                   */
template<class Type = float>
class _Quaternion
{
protected:

#pragma pack(push, 1)

  union
  {
    struct { Type _0, _1, _2, _3; };
    struct { Type x, y, z, w; };
    Type q[4];
  };

#pragma pack(pop)

protected:
  // constructors
  inline _Quaternion() {}

public:
  // access grants
  inline Type& operator () (uint n);
  inline Type  operator () (uint n) const;
  inline void get(Type& xOut, Type& yOut, Type& zOut, Type& wOut) const;

  // type casts & assignments
  inline operator Type* () const;

  // quaternion operations
  //todo inline Type getLength() const;
};

typedef _Quaternion<float> Quaternion;
typedef _Quaternion<float> Quaternionf;

#endif
