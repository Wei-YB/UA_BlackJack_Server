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
CMAKE_SOURCE_DIR = /home/tong/UA_BlackJack_Server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tong/UA_BlackJack_Server/build

# Include any dependencies generated for this target.
include grpc/third_party/protobuf/CMakeFiles/protoc.dir/depend.make

# Include the progress variables for this target.
include grpc/third_party/protobuf/CMakeFiles/protoc.dir/progress.make

# Include the compile flags for this target's objects.
include grpc/third_party/protobuf/CMakeFiles/protoc.dir/flags.make

grpc/third_party/protobuf/CMakeFiles/protoc.dir/__/src/google/protobuf/compiler/main.cc.o: grpc/third_party/protobuf/CMakeFiles/protoc.dir/flags.make
grpc/third_party/protobuf/CMakeFiles/protoc.dir/__/src/google/protobuf/compiler/main.cc.o: ../thirdparty/grpc/third_party/protobuf/src/google/protobuf/compiler/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object grpc/third_party/protobuf/CMakeFiles/protoc.dir/__/src/google/protobuf/compiler/main.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/protobuf && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/protoc.dir/__/src/google/protobuf/compiler/main.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/protobuf/src/google/protobuf/compiler/main.cc

grpc/third_party/protobuf/CMakeFiles/protoc.dir/__/src/google/protobuf/compiler/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/protoc.dir/__/src/google/protobuf/compiler/main.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/protobuf && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/protobuf/src/google/protobuf/compiler/main.cc > CMakeFiles/protoc.dir/__/src/google/protobuf/compiler/main.cc.i

grpc/third_party/protobuf/CMakeFiles/protoc.dir/__/src/google/protobuf/compiler/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/protoc.dir/__/src/google/protobuf/compiler/main.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/protobuf && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/protobuf/src/google/protobuf/compiler/main.cc -o CMakeFiles/protoc.dir/__/src/google/protobuf/compiler/main.cc.s

# Object files for target protoc
protoc_OBJECTS = \
"CMakeFiles/protoc.dir/__/src/google/protobuf/compiler/main.cc.o"

# External object files for target protoc
protoc_EXTERNAL_OBJECTS =

grpc/third_party/protobuf/protoc-3.15.8.0: grpc/third_party/protobuf/CMakeFiles/protoc.dir/__/src/google/protobuf/compiler/main.cc.o
grpc/third_party/protobuf/protoc-3.15.8.0: grpc/third_party/protobuf/CMakeFiles/protoc.dir/build.make
grpc/third_party/protobuf/protoc-3.15.8.0: grpc/third_party/protobuf/libprotoc.a
grpc/third_party/protobuf/protoc-3.15.8.0: grpc/third_party/protobuf/libprotobuf.a
grpc/third_party/protobuf/protoc-3.15.8.0: grpc/third_party/protobuf/CMakeFiles/protoc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable protoc"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/protobuf && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/protoc.dir/link.txt --verbose=$(VERBOSE)
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/protobuf && $(CMAKE_COMMAND) -E cmake_symlink_executable protoc-3.15.8.0 protoc

grpc/third_party/protobuf/protoc: grpc/third_party/protobuf/protoc-3.15.8.0


# Rule to build all files generated by this target.
grpc/third_party/protobuf/CMakeFiles/protoc.dir/build: grpc/third_party/protobuf/protoc

.PHONY : grpc/third_party/protobuf/CMakeFiles/protoc.dir/build

grpc/third_party/protobuf/CMakeFiles/protoc.dir/clean:
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/protobuf && $(CMAKE_COMMAND) -P CMakeFiles/protoc.dir/cmake_clean.cmake
.PHONY : grpc/third_party/protobuf/CMakeFiles/protoc.dir/clean

grpc/third_party/protobuf/CMakeFiles/protoc.dir/depend:
	cd /home/tong/UA_BlackJack_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tong/UA_BlackJack_Server /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/protobuf/cmake /home/tong/UA_BlackJack_Server/build /home/tong/UA_BlackJack_Server/build/grpc/third_party/protobuf /home/tong/UA_BlackJack_Server/build/grpc/third_party/protobuf/CMakeFiles/protoc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grpc/third_party/protobuf/CMakeFiles/protoc.dir/depend

