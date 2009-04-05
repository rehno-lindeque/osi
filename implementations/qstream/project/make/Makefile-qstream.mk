##############################################################################
#
#     QSTREAM
#
#     Copyright (c) 2008 Rehno Lindeque. All rights reserved.
#
##############################################################################

USER_TOOL = g++

USER_BUILD_DIR  = ../../build/posix-g++
USER_INCLUDE_DIRS = ../../../../api \
                    ../../../basestream/src \
                    ../../../base/src
USER_SOURCE_FILES = ../../src/*.cpp
USER_DEBUGFLAGS = -g -D_DEBUG -Wall

all: $(USER_BUILD_DIR)
	@#echo "Building QStream objects:"
	@#echo "Change directory to the build directory"
	@#echo "Compile files to .o"
    cd $(USER_BUILD_DIR)
	$(USER_TOOL) -c $(addprefix -I,$(USER_INCLUDE_DIRS)) $(USER_SOURCE_FILES)
		
$(USER_BUILD_DIR):
	mkdir -p $(USER_BUILD_DIR)

%.o: %.cpp
	$(USER_TOOL) -c $< -o $@ $(addprefix -I,$(USER_INCLUDE_DIRS))