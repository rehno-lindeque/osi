#!/bin/sh
# Copyright (c) 2009 Rehno Lindeque. All rights reserved.

cd ..

echo
echo 'Copying common build system files to all projects...'

cp -r common/buildfiles/* ../samples/parsertest/project/
cp -r common/buildfiles/* ../implementations/qparser/project/

echo 'Done'
echo

