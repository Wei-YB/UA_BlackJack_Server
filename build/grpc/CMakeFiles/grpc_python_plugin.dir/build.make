# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake3

# The command to remove a file.
RM = /usr/bin/cmake3 -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/ricki/UA_BlackJack_Server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/ricki/UA_BlackJack_Server/build

# Include any dependencies generated for this target.
include grpc/CMakeFiles/grpc_python_plugin.dir/depend.make

# Include the progress variables for this target.
include grpc/CMakeFiles/grpc_python_plugin.dir/progress.make

# Include the compile flags for this target's objects.
include grpc/CMakeFiles/grpc_python_plugin.dir/flags.make

grpc/CMakeFiles/grpc_python_plugin.dir/src/compiler/python_plugin.cc.o: grpc/CMakeFiles/grpc_python_plugin.dir/flags.make
grpc/CMakeFiles/grpc_python_plugin.dir/src/compiler/python_plugin.cc.o: ../thirdparty/grpc/src/compiler/python_plugin.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object grpc/CMakeFiles/grpc_python_plugin.dir/src/compiler/python_plugin.cc.o"
	cd /root/ricki/UA_BlackJack_Server/build/grpc && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grpc_python_plugin.dir/src/compiler/python_plugin.cc.o -c /root/ricki/UA_BlackJack_Server/thirdparty/grpc/src/compiler/python_plugin.cc

grpc/CMakeFiles/grpc_python_plugin.dir/src/compiler/python_plugin.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grpc_python_plugin.dir/src/compiler/python_plugin.cc.i"
	cd /root/ricki/UA_BlackJack_Server/build/grpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ricki/UA_BlackJack_Server/thirdparty/grpc/src/compiler/python_plugin.cc > CMakeFiles/grpc_python_plugin.dir/src/compiler/python_plugin.cc.i

grpc/CMakeFiles/grpc_python_plugin.dir/src/compiler/python_plugin.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grpc_python_plugin.dir/src/compiler/python_plugin.cc.s"
	cd /root/ricki/UA_BlackJack_Server/build/grpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ricki/UA_BlackJack_Server/thirdparty/grpc/src/compiler/python_plugin.cc -o CMakeFiles/grpc_python_plugin.dir/src/compiler/python_plugin.cc.s

# Object files for target grpc_python_plugin
grpc_python_plugin_OBJECTS = \
"CMakeFiles/grpc_python_plugin.dir/src/compiler/python_plugin.cc.o"

# External object files for target grpc_python_plugin
grpc_python_plugin_EXTERNAL_OBJECTS =

grpc/grpc_python_plugin: grpc/CMakeFiles/grpc_python_plugin.dir/src/compiler/python_plugin.cc.o
grpc/grpc_python_plugin: grpc/CMakeFiles/grpc_python_plugin.dir/build.make
grpc/grpc_python_plugin: grpc/third_party/protobuf/libprotoc.a
grpc/grpc_python_plugin: grpc/third_party/protobuf/libprotobuf.a
grpc/grpc_python_plugin: grpc/libgrpc_plugin_support.a
grpc/grpc_python_plugin: grpc/third_party/protobuf/libprotoc.a
grpc/grpc_python_plugin: grpc/third_party/protobuf/libprotobuf.a
grpc/grpc_python_plugin: grpc/CMakeFiles/grpc_python_plugin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable grpc_python_plugin"
	cd /root/ricki/UA_BlackJack_Server/build/grpc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/grpc_python_plugin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
grpc/CMakeFiles/grpc_python_plugin.dir/build: grpc/grpc_python_plugin

.PHONY : grpc/CMakeFiles/grpc_python_plugin.dir/build

grpc/CMakeFiles/grpc_python_plugin.dir/clean:
	cd /root/ricki/UA_BlackJack_Server/build/grpc && $(CMAKE_COMMAND) -P CMakeFiles/grpc_python_plugin.dir/cmake_clean.cmake
.PHONY : grpc/CMakeFiles/grpc_python_plugin.dir/clean

grpc/CMakeFiles/grpc_python_plugin.dir/depend:
	cd /root/ricki/UA_BlackJack_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ricki/UA_BlackJack_Server /root/ricki/UA_BlackJack_Server/thirdparty/grpc /root/ricki/UA_BlackJack_Server/build /root/ricki/UA_BlackJack_Server/build/grpc /root/ricki/UA_BlackJack_Server/build/grpc/CMakeFiles/grpc_python_plugin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grpc/CMakeFiles/grpc_python_plugin.dir/depend

