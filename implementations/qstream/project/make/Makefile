##############################################################################
#
#     QSTREAM
#
#     Copyright (c) 2008 Rehno Lindeque. All rights reserved.
#
##############################################################################

USER_TOOL = g++

USER_BUILD_DIR  = ../../lib
USER_TARGET     = qstream.a
USER_MAKE_FILES = Makefile-qstream.mk ../../../basestream/project/make/Makefile-basestream.mk
USER_OBJ_FILES  = ../../build/posix-g++/*
USER_LIB_DIRS   = 
USER_LIBS       = #libstdc++.a
USER_INCLUDE_DIRS = ../../../../api \
                    ../../../basestream/src \
                    ../../../base/src
USER_DEFINITIONS =
USER_DEBUGFLAGS = -g -D_DEBUG -Wall

all: $(USER_TARGET)
		
$(USER_TARGET):
	@#echo "Building $@:"
	for MAKE_FILE in ${USER_MAKE_FILES}; \
	do \
	  make -f $${MAKE_FILE}; \
	done
	@#echo "Finished building $@"



