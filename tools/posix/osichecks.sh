#!/bin/sh
# Copyright (c) 2009 Rehno Lindeque. All rights reserved.

cd ../..

# Check for consistency in the source files
echo
echo
echo
echo --------------- Base Libraries --------------- 
echo
echo
echo
echo common/functions.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/common/functions.h implementations/basege/src/basege/common/functions.h
diff -y --suppress-common-lines implementations/base/src/base/common/functions.h implementations/basere/src/basere/common/functions.h
diff -y --suppress-common-lines implementations/base/src/base/common/functions.h implementations/basegl/src/basegl/common/functions.h
diff -y --suppress-common-lines implementations/base/src/base/common/functions.h implementations/basegui/src/basegui/common/functions.h
diff -y --suppress-common-lines implementations/base/src/base/common/functions.h implementations/basekernel/src/basekernel/common/functions.h
diff -y --suppress-common-lines implementations/base/src/base/common/functions.h implementations/baseparser/src/baseparser/common/functions.h

echo
echo common/object.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/common/object.h implementations/basege/src/basege/common/object.h
diff -y --suppress-common-lines implementations/base/src/base/common/object.h implementations/basere/src/basere/common/object.h
diff -y --suppress-common-lines implementations/base/src/base/common/object.h implementations/basegl/src/basegl/common/object.h
diff -y --suppress-common-lines implementations/base/src/base/common/object.h implementations/basegui/src/basegui/common/object.h
diff -y --suppress-common-lines implementations/base/src/base/common/object.h implementations/basekernel/src/basekernel/common/object.h
diff -y --suppress-common-lines implementations/base/src/base/common/object.h implementations/baseparser/src/baseparser/common/object.h

echo
echo common/structures.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/common/structures.h implementations/basege/src/basege/common/structures.h
diff -y --suppress-common-lines implementations/base/src/base/common/structures.h implementations/basere/src/basere/common/structures.h
diff -y --suppress-common-lines implementations/base/src/base/common/structures.h implementations/basegl/src/basegl/common/structures.h
diff -y --suppress-common-lines implementations/base/src/base/common/structures.h implementations/basegui/src/basegui/common/structures.h
diff -y --suppress-common-lines implementations/base/src/base/common/structures.h implementations/basekernel/src/basekernel/common/structures.h
diff -y --suppress-common-lines implementations/base/src/base/common/structures.h implementations/baseparser/src/baseparser/common/structures.h

echo
echo common/types.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/common/types.h implementations/basege/src/basege/common/types.h
diff -y --suppress-common-lines implementations/base/src/base/common/types.h implementations/basere/src/basere/common/types.h
diff -y --suppress-common-lines implementations/base/src/base/common/types.h implementations/basegl/src/basegl/common/types.h
diff -y --suppress-common-lines implementations/base/src/base/common/types.h implementations/basegui/src/basegui/common/types.h
diff -y --suppress-common-lines implementations/base/src/base/common/types.h implementations/basekernel/src/basekernel/common/types.h
diff -y --suppress-common-lines implementations/base/src/base/common/types.h implementations/baseparser/src/baseparser/common/types.h

echo
echo math/axisalignedboundingbox.h
echo -----------------------------
diff -y --suppress-common-lines implementations/base/src/base/math/axisalignedboundingbox.h implementations/basege/src/basege/math/axisalignedboundingbox.h
diff -y --suppress-common-lines implementations/base/src/base/math/axisalignedboundingbox.h implementations/basere/src/basere/math/axisalignedboundingbox.h
diff -y --suppress-common-lines implementations/base/src/base/math/axisalignedboundingbox.h implementations/basegl/src/basegl/math/axisalignedboundingbox.h
diff -y --suppress-common-lines implementations/base/src/base/math/axisalignedboundingbox.h implementations/basegui/src/basegui/math/axisalignedboundingbox.h
diff -y --suppress-common-lines implementations/base/src/base/math/axisalignedboundingbox.h implementations/basekernel/src/basekernel/math/axisalignedboundingbox.h
diff -y --suppress-common-lines implementations/base/src/base/math/axisalignedboundingbox.h implementations/baseparser/src/baseparser/math/axisalignedboundingbox.h

echo
echo math/axisalignedboundingbox.hxx
echo -----------------------------
diff -y --suppress-common-lines implementations/base/src/base/math/axisalignedboundingbox.hxx implementations/basege/src/basege/math/axisalignedboundingbox.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/axisalignedboundingbox.hxx implementations/basere/src/basere/math/axisalignedboundingbox.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/axisalignedboundingbox.hxx implementations/basegl/src/basegl/math/axisalignedboundingbox.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/axisalignedboundingbox.hxx implementations/basegui/src/basegui/math/axisalignedboundingbox.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/axisalignedboundingbox.hxx implementations/basekernel/src/basekernel/math/axisalignedboundingbox.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/axisalignedboundingbox.hxx implementations/baseparser/src/baseparser/math/axisalignedboundingbox.hxx

echo
echo math/hcoord3.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/math/hcoord3.h implementations/basege/src/basege/math/hcoord3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hcoord3.h implementations/basere/src/basere/math/hcoord3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hcoord3.h implementations/basegl/src/basegl/math/hcoord3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hcoord3.h implementations/basegui/src/basegui/math/hcoord3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hcoord3.h implementations/basekernel/src/basekernel/math/hcoord3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hcoord3.h implementations/baseparser/src/baseparser/math/hcoord3.h

echo
echo math/hcoord3.hxx
echo ----------------
diff -y --suppress-common-lines implementations/base/src/base/math/hcoord3.hxx implementations/basege/src/basege/math/hcoord3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hcoord3.hxx implementations/basere/src/basere/math/hcoord3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hcoord3.hxx implementations/basegl/src/basegl/math/hcoord3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hcoord3.hxx implementations/basegui/src/basegui/math/hcoord3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hcoord3.hxx implementations/basekernel/src/basekernel/math/hcoord3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hcoord3.hxx implementations/baseparser/src/baseparser/math/hcoord3.hxx

echo
echo math/hpoint3.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/math/hpoint3.h implementations/basege/src/basege/math/hpoint3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hpoint3.h implementations/basere/src/basere/math/hpoint3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hpoint3.h implementations/basegl/src/basegl/math/hpoint3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hpoint3.h implementations/basegui/src/basegui/math/hpoint3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hpoint3.h implementations/basekernel/src/basekernel/math/hpoint3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hpoint3.h implementations/baseparser/src/baseparser/math/hpoint3.h

echo
echo math/hpoint3.hxx
echo ----------------
diff -y --suppress-common-lines implementations/base/src/base/math/hpoint3.hxx implementations/basege/src/basege/math/hpoint3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hpoint3.hxx implementations/basere/src/basere/math/hpoint3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hpoint3.hxx implementations/basegl/src/basegl/math/hpoint3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hpoint3.hxx implementations/basegui/src/basegui/math/hpoint3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hpoint3.hxx implementations/basekernel/src/basekernel/math/hpoint3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hpoint3.hxx implementations/baseparser/src/baseparser/math/hpoint3.hxx

echo
echo math/hvector3.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/math/hvector3.h implementations/basege/src/basege/math/hvector3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hvector3.h implementations/basere/src/basere/math/hvector3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hvector3.h implementations/basegl/src/basegl/math/hvector3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hvector3.h implementations/basegui/src/basegui/math/hvector3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hvector3.h implementations/basekernel/src/basekernel/math/hvector3.h
diff -y --suppress-common-lines implementations/base/src/base/math/hvector3.h implementations/baseparser/src/baseparser/math/hvector3.h

echo
echo math/hvector3.hxx
echo ----------------
diff -y --suppress-common-lines implementations/base/src/base/math/hvector3.hxx implementations/basege/src/basege/math/hvector3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hvector3.hxx implementations/basere/src/basere/math/hvector3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hvector3.hxx implementations/basegl/src/basegl/math/hvector3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hvector3.hxx implementations/basegui/src/basegui/math/hvector3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hvector3.hxx implementations/basekernel/src/basekernel/math/hvector3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/hvector3.hxx implementations/baseparser/src/baseparser/math/hvector3.hxx

echo
echo math/math.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/math/math.h implementations/basege/src/basege/math/math.h
diff -y --suppress-common-lines implementations/base/src/base/math/math.h implementations/basere/src/basere/math/math.h
diff -y --suppress-common-lines implementations/base/src/base/math/math.h implementations/basegl/src/basegl/math/math.h
diff -y --suppress-common-lines implementations/base/src/base/math/math.h implementations/basegui/src/basegui/math/math.h
diff -y --suppress-common-lines implementations/base/src/base/math/math.h implementations/basekernel/src/basekernel/math/math.h
diff -y --suppress-common-lines implementations/base/src/base/math/math.h implementations/baseparser/src/baseparser/math/math.h

echo
echo math/matrix3.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/math/matrix3.h implementations/basege/src/basege/math/matrix3.h
diff -y --suppress-common-lines implementations/base/src/base/math/matrix3.h implementations/basere/src/basere/math/matrix3.h
diff -y --suppress-common-lines implementations/base/src/base/math/matrix3.h implementations/basegl/src/basegl/math/matrix3.h
diff -y --suppress-common-lines implementations/base/src/base/math/matrix3.h implementations/basegui/src/basegui/math/matrix3.h
diff -y --suppress-common-lines implementations/base/src/base/math/matrix3.h implementations/basekernel/src/basekernel/math/matrix3.h
diff -y --suppress-common-lines implementations/base/src/base/math/matrix3.h implementations/baseparser/src/baseparser/math/matrix3.h

echo
echo math/matrix3.hxx
echo ----------------
diff -y --suppress-common-lines implementations/base/src/base/math/matrix3.hxx implementations/basege/src/basege/math/matrix3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/matrix3.hxx implementations/basere/src/basere/math/matrix3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/matrix3.hxx implementations/basegl/src/basegl/math/matrix3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/matrix3.hxx implementations/basegui/src/basegui/math/matrix3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/matrix3.hxx implementations/basekernel/src/basekernel/math/matrix3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/matrix3.hxx implementations/baseparser/src/baseparser/math/matrix3.hxx

echo
echo math/matrix4.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/math/matrix4.h implementations/basege/src/basege/math/matrix4.h
diff -y --suppress-common-lines implementations/base/src/base/math/matrix4.h implementations/basere/src/basere/math/matrix4.h
diff -y --suppress-common-lines implementations/base/src/base/math/matrix4.h implementations/basegl/src/basegl/math/matrix4.h
diff -y --suppress-common-lines implementations/base/src/base/math/matrix4.h implementations/basegui/src/basegui/math/matrix4.h
diff -y --suppress-common-lines implementations/base/src/base/math/matrix4.h implementations/basekernel/src/basekernel/math/matrix4.h
diff -y --suppress-common-lines implementations/base/src/base/math/matrix4.h implementations/baseparser/src/baseparser/math/matrix4.h

echo
echo math/matrix4.hxx
echo ----------------
diff -y --suppress-common-lines implementations/base/src/base/math/matrix4.hxx implementations/basege/src/basege/math/matrix4.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/matrix4.hxx implementations/basere/src/basere/math/matrix4.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/matrix4.hxx implementations/basegl/src/basegl/math/matrix4.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/matrix4.hxx implementations/basegui/src/basegui/math/matrix4.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/matrix4.hxx implementations/basekernel/src/basekernel/math/matrix4.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/matrix4.hxx implementations/baseparser/src/baseparser/math/matrix4.hxx

echo
echo math/quaternion.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/math/quaternion.h implementations/basege/src/basege/math/quaternion.h
diff -y --suppress-common-lines implementations/base/src/base/math/quaternion.h implementations/basere/src/basere/math/quaternion.h
diff -y --suppress-common-lines implementations/base/src/base/math/quaternion.h implementations/basegl/src/basegl/math/quaternion.h
diff -y --suppress-common-lines implementations/base/src/base/math/quaternion.h implementations/basegui/src/basegui/math/quaternion.h
diff -y --suppress-common-lines implementations/base/src/base/math/quaternion.h implementations/basekernel/src/basekernel/math/quaternion.h
diff -y --suppress-common-lines implementations/base/src/base/math/quaternion.h implementations/baseparser/src/baseparser/math/quaternion.h

echo
echo math/quaternion.hxx
echo ----------------
diff -y --suppress-common-lines implementations/base/src/base/math/quaternion.hxx implementations/basege/src/basege/math/quaternion.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/quaternion.hxx implementations/basere/src/basere/math/quaternion.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/quaternion.hxx implementations/basegl/src/basegl/math/quaternion.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/quaternion.hxx implementations/basegui/src/basegui/math/quaternion.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/quaternion.hxx implementations/basekernel/src/basekernel/math/quaternion.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/quaternion.hxx implementations/baseparser/src/baseparser/math/quaternion.hxx

echo
echo math/unitquaternion.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/math/unitquaternion.h implementations/basege/src/basege/math/unitquaternion.h
diff -y --suppress-common-lines implementations/base/src/base/math/unitquaternion.h implementations/basere/src/basere/math/unitquaternion.h
diff -y --suppress-common-lines implementations/base/src/base/math/unitquaternion.h implementations/basegl/src/basegl/math/unitquaternion.h
diff -y --suppress-common-lines implementations/base/src/base/math/unitquaternion.h implementations/basegui/src/basegui/math/unitquaternion.h
diff -y --suppress-common-lines implementations/base/src/base/math/unitquaternion.h implementations/basekernel/src/basekernel/math/unitquaternion.h
diff -y --suppress-common-lines implementations/base/src/base/math/unitquaternion.h implementations/baseparser/src/baseparser/math/unitquaternion.h

echo
echo math/unitquaternion.hxx
echo ----------------
diff -y --suppress-common-lines implementations/base/src/base/math/unitquaternion.hxx implementations/basege/src/basege/math/unitquaternion.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/unitquaternion.hxx implementations/basere/src/basere/math/unitquaternion.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/unitquaternion.hxx implementations/basegl/src/basegl/math/unitquaternion.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/unitquaternion.hxx implementations/basegui/src/basegui/math/unitquaternion.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/unitquaternion.hxx implementations/basekernel/src/basekernel/math/unitquaternion.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/unitquaternion.hxx implementations/baseparser/src/baseparser/math/unitquaternion.hxx

echo
echo math/vector2.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/math/vector2.h implementations/basege/src/basege/math/vector2.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector2.h implementations/basere/src/basere/math/vector2.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector2.h implementations/basegl/src/basegl/math/vector2.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector2.h implementations/basegui/src/basegui/math/vector2.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector2.h implementations/basekernel/src/basekernel/math/vector2.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector2.h implementations/baseparser/src/baseparser/math/vector2.h

echo
echo math/vector2.hxx
echo ----------------
diff -y --suppress-common-lines implementations/base/src/base/math/vector2.hxx implementations/basege/src/basege/math/vector2.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector2.hxx implementations/basere/src/basere/math/vector2.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector2.hxx implementations/basegl/src/basegl/math/vector2.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector2.hxx implementations/basegui/src/basegui/math/vector2.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector2.hxx implementations/basekernel/src/basekernel/math/vector2.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector2.hxx implementations/baseparser/src/baseparser/math/vector2.hxx

echo
echo math/vector3.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/math/vector3.h implementations/basege/src/basege/math/vector3.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector3.h implementations/basere/src/basere/math/vector3.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector3.h implementations/basegl/src/basegl/math/vector3.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector3.h implementations/basegui/src/basegui/math/vector3.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector3.h implementations/basekernel/src/basekernel/math/vector3.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector3.h implementations/baseparser/src/baseparser/math/vector3.h

echo
echo math/vector3.hxx
echo ----------------
diff -y --suppress-common-lines implementations/base/src/base/math/vector3.hxx implementations/basege/src/basege/math/vector3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector3.hxx implementations/basere/src/basere/math/vector3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector3.hxx implementations/basegl/src/basegl/math/vector3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector3.hxx implementations/basegui/src/basegui/math/vector3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector3.hxx implementations/basekernel/src/basekernel/math/vector3.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector3.hxx implementations/baseparser/src/baseparser/math/vector3.hxx

echo
echo math/vector4.h
echo --------------
diff -y --suppress-common-lines implementations/base/src/base/math/vector4.h implementations/basege/src/basege/math/vector4.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector4.h implementations/basere/src/basere/math/vector4.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector4.h implementations/basegl/src/basegl/math/vector4.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector4.h implementations/basegui/src/basegui/math/vector4.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector4.h implementations/basekernel/src/basekernel/math/vector4.h
diff -y --suppress-common-lines implementations/base/src/base/math/vector4.h implementations/baseparser/src/baseparser/math/vector4.h

echo
echo math/vector4.hxx
echo ----------------
diff -y --suppress-common-lines implementations/base/src/base/math/vector4.hxx implementations/basege/src/basege/math/vector4.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector4.hxx implementations/basere/src/basere/math/vector4.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector4.hxx implementations/basegl/src/basegl/math/vector4.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector4.hxx implementations/basegui/src/basegui/math/vector4.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector4.hxx implementations/basekernel/src/basekernel/math/vector4.hxx
diff -y --suppress-common-lines implementations/base/src/base/math/vector4.hxx implementations/baseparser/src/baseparser/math/vector4.hxx

echo
echo Sconscript
echo ----------
diff -y --suppress-common-lines implementations/basege/project/scons/SConscript implementations/basere/project/scons/SConscript
diff -y --suppress-common-lines implementations/basege/project/scons/SConscript implementations/basegl/project/scons/SConscript
diff -y --suppress-common-lines implementations/basege/project/scons/SConscript implementations/basegui/project/scons/SConscript
diff -y --suppress-common-lines implementations/basege/project/scons/SConscript implementations/basekernel/project/scons/SConscript
diff -y --suppress-common-lines implementations/basege/project/scons/SConscript implementations/baseparser/project/scons/SConscript


echo
echo
echo
echo --------------- Implementations --------------- 
echo
echo
echo Sconstruct
echo ----------
diff -y --suppress-common-lines implementations/nativege/project/scons/SConstruct implementations/glge/project/scons/SConstruct
diff -y --suppress-common-lines implementations/nativege/project/scons/SConstruct implementations/glrasre/project/scons/SConstruct
diff -y --suppress-common-lines implementations/nativege/project/scons/SConstruct implementations/glrayre/project/scons/SConstruct
diff -y --suppress-common-lines implementations/nativege/project/scons/SConstruct implementations/qrayre/project/scons/SConstruct
diff -y --suppress-common-lines implementations/nativege/project/scons/SConstruct implementations/linglkernel/project/scons/SConstruct
diff -y --suppress-common-lines implementations/nativege/project/scons/SConstruct implementations/winglkernel/project/scons/SConstruct
diff -y --suppress-common-lines implementations/nativege/project/scons/SConstruct implementations/glgui/project/scons/SConstruct
diff -y --suppress-common-lines implementations/nativege/project/scons/SConstruct implementations/wingui/project/scons/SConstruct

echo
echo Sconscript
echo ----------
diff -y --suppress-common-lines implementations/nativege/project/scons/SConscript implementations/glge/project/scons/SConscript
diff -y --suppress-common-lines implementations/nativege/project/scons/SConscript implementations/glrasre/project/scons/SConscript
diff -y --suppress-common-lines implementations/nativege/project/scons/SConscript implementations/glrayre/project/scons/SConscript
diff -y --suppress-common-lines implementations/nativege/project/scons/SConscript implementations/qrayre/project/scons/SConscript
diff -y --suppress-common-lines implementations/nativege/project/scons/SConscript implementations/linglkernel/project/scons/SConscript
diff -y --suppress-common-lines implementations/nativege/project/scons/SConscript implementations/winglkernel/project/scons/SConscript
diff -y --suppress-common-lines implementations/nativege/project/scons/SConscript implementations/glgui/project/scons/SConscript
diff -y --suppress-common-lines implementations/nativege/project/scons/SConscript implementations/wingui/project/scons/SConscript

# todo: check for copyright and licensing headers

