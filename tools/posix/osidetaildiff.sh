#!/bin/sh
# Copyright (c) 2009 Rehno Lindeque. All rights reserved.

cd ..

## Check build system files for consistency
echo
echo 'Check build system files for consistency'
echo '========================================'

# GLGE
echo
echo 'implementations/glge'
echo '--------------'
diff -y --left-column common/reference/project/scons/SConscript ../implementations/glge/project/scons/SConscript
diff -y --left-column common/reference/project/scons/SConstargetscript ../implementations/glge/project/scons/SConstargetscript
diff -y --left-column reference/project/scons/SConstruct ../implementations/glge/project/scons/SConstruct

# QParser
echo
echo 'implementations/qparser'
echo '--------------'
diff -y --left-column common/reference/project/scons/SConscript ../implementations/qparser/project/scons/SConscript
diff -y --left-column common/reference/project/scons/SConstargetscript ../implementations/qparser/project/scons/SConstargetscript
diff -y --left-column reference/project/scons/SConstruct ../implementations/qparser/project/scons/SConstruct

