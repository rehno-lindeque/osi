#ifdef __SAMPLES_MATRIX4_H__
#ifndef __SAMPLES_MATRIX4_HXX__
#define __SAMPLES_MATRIX4_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    MATRIX4.HXX
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
_Matrix4<Type> operator * (Type arg1, const _Matrix4<Type>& arg2)
{
  return arg2 * arg1;
}

template<class Type>
_Matrix4<Type>::_Matrix4(const Type* f)
{
  set(f);
}

template<class Type>
_Matrix4<Type>::_Matrix4(const _Matrix4<Type>& m)
{
  set((Type*)m);
}

template<class Type>
_Matrix4<Type>::_Matrix4(Type _00, Type _01, Type _02, Type _03,
                         Type _10, Type _11, Type _12, Type _13,
                         Type _20, Type _21, Type _22, Type _23,
                         Type _30, Type _31, Type _32, Type _33)
{
  set(_00, _01, _02, _03,
	    _10, _11, _12, _13,
	    _20, _21, _22, _23,
	    _30, _31, _32, _33);
}

template<class Type>
void _Matrix4<Type>::set(Type _00, Type _01, Type _02, Type _03,
                    Type _10, Type _11, Type _12, Type _13,
                    Type _20, Type _21, Type _22, Type _23,
                    Type _30, Type _31, Type _32, Type _33)
{
  Matrix4::_00 = _00; Matrix4::_01 = _01; Matrix4::_02 = _02; Matrix4::_03 = _03;
  Matrix4::_10 = _10; Matrix4::_11 = _11; Matrix4::_12 = _12; Matrix4::_13 = _13;
  Matrix4::_20 = _20; Matrix4::_21 = _21; Matrix4::_22 = _22; Matrix4::_23 = _23;
  Matrix4::_30 = _30; Matrix4::_31 = _31; Matrix4::_32 = _32; Matrix4::_33 = _33;
}

template<class Type>
void _Matrix4<Type>::set(const Type* f)
{
  memcpy(m, f, 4*4*sizeof(Type));
}

template<class Type>
Type& _Matrix4<Type>::operator () (uint row, uint col) 
{ 
  return m[row][col]; 
}

template<class Type>
Type _Matrix4<Type>::operator  () (uint row, uint col) const
{ 
  return m[row][col]; 
} 

template<class Type>
Type& _Matrix4<Type>::operator () (uint n) 
{ 
  return m[0][n]; 
}

template<class Type>
Type _Matrix4<Type>::operator  () (uint n) const
{ 
  return m[0][n]; 
} 

template<class Type>
_Matrix4<Type>::operator const Type* () const
{
  return (Type*)m;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::operator *= (const _Matrix4<Type>& arg)
{
  Type result[4][4];
  for (uint cy = 0; cy<4; cy++) 
    for(uint cx = 0; cx<4; cx++)
      result[cy][cx] = m[cy][0] * arg(0,cx) +
                       m[cy][1] * arg(1,cx) +
                       m[cy][2] * arg(2,cx) +
                       m[cy][3] * arg(3,cx);
  set((Type*)result);
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::operator += (const _Matrix4<Type>& arg)
{
  for (uint cy = 0; cy<4; cy++)    
    for(uint cx = 0; cx<4; cx++)        
      m[cy][cx] += arg(cy,cx);  
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::operator -= (const _Matrix4<Type>& arg)
{  
  for (uint cy = 0; cy<4; cy++)
    for(uint cx = 0; cx<4; cx++)    
      m[cy][cx] -= arg(cy,cx);
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::operator *= (Type f)
{  
  for (uint cy = 0; cy<4; cy++)
    for(uint cx = 0; cx<4; cx++)    
      m[cy][cx] *= f;
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::operator /= (Type f)
{
  (*this) *= 1/f;
  return *this;
}

template<class Type>
_Matrix4<Type> _Matrix4<Type>::operator * (const _Matrix4<Type>& m) const
{
  _Matrix4<Type> Ret;
  
  for (uint cy = 0; cy<4; cy++)
    for(uint cx = 0; cx<4; cx++)    
      Ret(cy,cx) = _Matrix4<Type>::m[cy][0] * m(0,cx) +
                   _Matrix4<Type>::m[cy][1] * m(1,cx) +
                   _Matrix4<Type>::m[cy][2] * m(2,cx) +
                   _Matrix4<Type>::m[cy][3] * m(3,cx);
  return Ret;
}

template<class Type>
_Matrix4<Type> _Matrix4<Type>::operator + (const _Matrix4<Type>& m) const
{
  _Matrix4<Type> Ret;

  for (uint cy = 0; cy<4; cy++)
    for(uint cx = 0; cx<4; cx++)    
      Ret(cy,cx) = _Matrix4<Type>::m[cy][cx] + m(cy,cx);
  return Ret;
}

template<class Type>
_Matrix4<Type> _Matrix4<Type>::operator - (const _Matrix4<Type>& m) const
{  
  _Matrix4<Type> Ret;

  for (uint cy = 0; cy<4; cy++)
    for(uint cx = 0; cx<4; cx++)    
      Ret(cy,cx) = _Matrix4<Type>::m[cy][cx] - m(cy,cx);
  
  return Ret;
}

template<class Type>
_Matrix4<Type> _Matrix4<Type>::operator * (Type f) const
{
  return _Matrix4<Type>( m[0][0] * f, m[0][1] * f, m[0][2] * f, m[0][3] * f,
                  m[1][0] * f, m[1][1] * f, m[1][2] * f, m[1][3] * f,
                  m[2][0] * f, m[2][1] * f, m[2][2] * f, m[2][3] * f,
                  m[3][0] * f, m[3][1] * f, m[3][2] * f, m[3][3] * f  );
}

template<class Type>
_Matrix4<Type> _Matrix4<Type>::operator / (Type f) const
{
  return (*this) * (1.0f/f);
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::inverse()
{
  Type MatrixBuffer[3][3];
  Type IMatrixBuffer[4][4];
  Type det = getDeterminant();

  for(uint cdy = 0; cdy < 4; cdy++)
    for(uint cdx = 0; cdx < 4; cdx++)
    {
      for(uint cy = 0, ccy = 0; cy < 4; cy++)
      {
        if(cdy == cy) 
          continue;
        for(uint cx = 0, ccx = 0; cx < 4; cx++)
        { 
          if(cdx == cx) 
            continue;
          MatrixBuffer[ccy][ccx] = m[cy][cx];
          ccx++;
        }    
        ccy++;
      }
      IMatrixBuffer[cdy][cdx] = ((_Matrix3<Type>*)MatrixBuffer)->getDeterminant() / det;
    }   
  this->_Matrix4<Type>::_Matrix4(*IMatrixBuffer);  
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::transpose()
{
  set(_00, _10, _20, _30,
      _01, _11, _21, _31,
      _02, _12, _22, _32,
      _03, _13, _23, _33);
  return *this;
}

template<class Type>
Type _Matrix4<Type>::getDeterminant() const
{
  _Matrix3<Type> A( _11, _12, _13,
                    _21, _22, _23,
                    _31, _32, _33);
  _Matrix3<Type> B( _01, _02, _03,
                    _21, _22, _23,
                    _31, _32, _33);
  _Matrix3<Type> C( _01, _02, _03,
                    _11, _12, _13,
                    _31, _32, _33);
  _Matrix3<Type> D( _01, _02, _03,
                    _11, _12, _13,
                    _21, _22, _23);
  return _00 * A.getDeterminant() - _10 * B.getDeterminant() + _20 * C.getDeterminant() - _30 * D.getDeterminant();
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setIdentity()
{
  set(IDENTITY_MATRIX4);
  /*set((Type)1.0, (Type)0.0, (Type)0.0, (Type)0.0,
        (Type)0.0, (Type)1.0, (Type)0.0, (Type)0.0,
        (Type)0.0, (Type)0.0, (Type)1.0, (Type)0.0,
        (Type)0.0, (Type)0.0, (Type)0.0, (Type)1.0);*/
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setClear() 
{
  memset(m, 0, 4*4*sizeof(Type));
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setView(const _Vector3<Type>& Pos, const _Vector3<Type>& LookAt, const _Vector3<Type>& Up)
{
  _Vector3<Type> ZAxis = (LookAt - Pos).getNormalized();
  _Vector3<Type> XAxis = _Vector3<Type>::cross((_Vector3<Type>)Up, (_Vector3<Type>)ZAxis).getNormalized();
  _Vector3<Type> YAxis = _Vector3<Type>::cross((_Vector3<Type>&)ZAxis, (_Vector3<Type>&)XAxis).getNormalized();
  
  set(XAxis(0),                   YAxis(0),                   ZAxis(0),                   0.0f,
      XAxis(1),                   YAxis(1),                   ZAxis(1),                   0.0f,
      XAxis(2),                   YAxis(2),                   ZAxis(2),                   0.0f,
      -_Vector3<Type>::dot((_Vector3<Type>)XAxis, (_Vector3<Type>)Pos), -_Vector3<Type>::dot((_Vector3<Type>)YAxis, (_Vector3<Type>)Pos), -_Vector3<Type>::dot((_Vector3<Type>)ZAxis, (_Vector3<Type>)Pos), 1.0f);
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setPerspectiveProjectionFOV(Type YFOV, Type AspectRatio, Type ZNearPlane, Type ZFarPlane)
{
  Type ViewSpaceHeight;
  Type ViewSpaceWidth;
  
  Type& zn = ZNearPlane;
  Type& zf = ZFarPlane;
  Type& h  = ViewSpaceHeight = cos((Type)YFOV/2.0f)/sin((Type)YFOV/2.0f);
  Type& w  = ViewSpaceWidth  = h / AspectRatio;
  
  set(w,    0.0f, 0.0f,           0.0f,
      0.0f, h,    0.0f,           0.0f,
      0.0f, 0.0f, zf/(zf-zn),     1.0f,
      0.0f, 0.0f, -zn*zf/(zf-zn), 0.0f);
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setPerspectiveProjection(Type ViewWidth, Type ViewHeight, Type ZNearPlane, Type ZFarPlane)
{
  Type& w  = ViewWidth;
  Type& h  = ViewHeight;
  Type& zn = ZNearPlane;
  Type& zf = ZFarPlane;  
  
  set(2.0f*zn/w, 0.0f,      0.0f,          0.0f,
      0.0f,      2.0f*zn/h, 0.0f,          0.0f,
      0.0f,      0.0f,      zf/(zf-zn),    1.0f,
      0.0f,      0.0f,      zn*zf/(zn-zf), 0.0f);
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setPerspectiveProjection(Type MinX, Type MaxX, Type MinY, Type MaxY, Type MinZ, Type MaxZ)
{
  Type& sx  = MinX;
  Type& bx  = MaxX;
  Type& sy  = MinY; 
  Type& by  = MaxY;  
  Type& zn  = MinZ;
  Type& zf  = MaxZ;
  
  set(2.0f*zn/(bx-sx), 0.0f,            0.0f,          0.0f,
      0.0f,            2.0f*zn/(by-sy), 0.0f,          0.0f,
      (sx+bx)/(sx-bx), (by+sy)/(sy-by), zf/(zf-zn),    1.0f,
      0.0f,            0.0f,            zn*zf/(zn-zf), 0.0f);
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setOrthoProjection(Type ViewWidth, Type ViewHeight, Type ZNearPlane, Type ZFarPlane)
{
  Type& w  = ViewWidth;
  Type& h  = ViewHeight;
  Type& zn = ZNearPlane;
  Type& zf = ZFarPlane;
  
  set(2.0f/w, 0.0f,   0.0f,         0.0f,
      0.0f,   2.0f/h, 0.0f,         0.0f,
      0.0f,   0.0f,   1.0f/(zf-zn), 0.0f,
      0.0f,   0.0f,   zn/(zn-zf),   1.0f);
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setOrthoProjection(Type MinX, Type MaxX, Type MinY, Type MaxY, Type MinZ, Type MaxZ)
{
  Type sx = MinX;
  Type bx = MaxX;
  Type sy = MinY;
  Type by = MaxY;
  Type zn = MinZ;
  Type zf = MaxZ;

  set(2.0f/(bx-sx),    0.0f,            0.0f,         0.0f,
      0.0f,            2.0f/(by-sy),    0.0f,         0.0f,
      0.0f,            0.0f,            1.0f/(zf-zn), 0.0f,
      (sx+bx)/(sx-bx), (by+sy)/(sy-by), zn/(zn-zf),   1.0f);
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setTranslation(Type x, Type y, Type z)
{
  set((Type)1.0, (Type)0.0, (Type)0.0, (Type)0.0,
      (Type)0.0, (Type)1.0, (Type)0.0, (Type)0.0,
      (Type)0.0, (Type)0.0, (Type)1.0, (Type)0.0,
      x,         y,         z,         (Type)1.0);
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setTranslation(_Vector3<Type>& v)
{
  set((Type)1.0, (Type)0.0, (Type)0.0, (Type)0.0,
      (Type)0.0, (Type)1.0, (Type)0.0, (Type)0.0,
      (Type)0.0, (Type)0.0, (Type)1.0, (Type)0.0,
      v(0),      v(1),      v(2),      (Type)1.0);
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setRotationX(Type angle)
{
  Type SIN = sin(angle);
  Type COS = cos(angle);
  set(1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, COS,  SIN,  0.0f,
      0.0f, -SIN, COS,  0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);  
  return *this;  
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setRotationY(Type angle)
{
  Type SIN = sin(angle);
  Type COS = cos(angle);
  set(COS,       (Type)0.0, -SIN,      (Type)0.0,
      (Type)0.0, (Type)1.0, (Type)0.0, (Type)0.0,
      SIN,       (Type)0.0, COS,       (Type)0.0,
      (Type)0.0, (Type)0.0, (Type)0.0, (Type)1.0);
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setRotationZ(Type angle)
{
  Type SIN = sin(angle);
  Type COS = cos(angle);
  set(COS,  SIN,  0.0f, 0.0f,
      -SIN, COS,  0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f  );  
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setAxisRotation(const _Vector3<Type>& axis, Type angle)
{    
  _UnitQuaternion<Type> rotationQuaternion(axis, angle);
  (*this) = rotationQuaternion.getTransformation();
  return *this;
}

template<class Type>
_Matrix4<Type>& _Matrix4<Type>::setScaling(Type x, Type y, Type z)
{
  set(x,         (Type)0.0, (Type)0.0, (Type)0.0,
	    (Type)0.0, y,         (Type)0.0, (Type)0.0,
	    (Type)0.0, (Type)0.0, z,         (Type)0.0,
	    (Type)0.0, (Type)0.0, (Type)0.0, (Type)1.0);
  return *this;
}

#endif
#endif
