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
include grpc/CMakeFiles/grpc++_alts.dir/depend.make

# Include the progress variables for this target.
include grpc/CMakeFiles/grpc++_alts.dir/progress.make

# Include the compile flags for this target's objects.
include grpc/CMakeFiles/grpc++_alts.dir/flags.make

grpc/CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_context.cc.o: grpc/CMakeFiles/grpc++_alts.dir/flags.make
grpc/CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_context.cc.o: ../thirdparty/grpc/src/cpp/common/alts_context.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object grpc/CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_context.cc.o"
	cd /root/ricki/UA_BlackJack_Server/build/grpc && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_context.cc.o -c /root/ricki/UA_BlackJack_Server/thirdparty/grpc/src/cpp/common/alts_context.cc

grpc/CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_context.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_context.cc.i"
	cd /root/ricki/UA_BlackJack_Server/build/grpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ricki/UA_BlackJack_Server/thirdparty/grpc/src/cpp/common/alts_context.cc > CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_context.cc.i

grpc/CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_context.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_context.cc.s"
	cd /root/ricki/UA_BlackJack_Server/build/grpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ricki/UA_BlackJack_Server/thirdparty/grpc/src/cpp/common/alts_context.cc -o CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_context.cc.s

grpc/CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_util.cc.o: grpc/CMakeFiles/grpc++_alts.dir/flags.make
grpc/CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_util.cc.o: ../thirdparty/grpc/src/cpp/common/alts_util.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object grpc/CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_util.cc.o"
	cd /root/ricki/UA_BlackJack_Server/build/grpc && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_util.cc.o -c /root/ricki/UA_BlackJack_Server/thirdparty/grpc/src/cpp/common/alts_util.cc

grpc/CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_util.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_util.cc.i"
	cd /root/ricki/UA_BlackJack_Server/build/grpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ricki/UA_BlackJack_Server/thirdparty/grpc/src/cpp/common/alts_util.cc > CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_util.cc.i

grpc/CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_util.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_util.cc.s"
	cd /root/ricki/UA_BlackJack_Server/build/grpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ricki/UA_BlackJack_Server/thirdparty/grpc/src/cpp/common/alts_util.cc -o CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_util.cc.s

# Object files for target grpc++_alts
grpc_______alts_OBJECTS = \
"CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_context.cc.o" \
"CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_util.cc.o"

# External object files for target grpc++_alts
grpc_______alts_EXTERNAL_OBJECTS =

grpc/libgrpc++_alts.a: grpc/CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_context.cc.o
grpc/libgrpc++_alts.a: grpc/CMakeFiles/grpc++_alts.dir/src/cpp/common/alts_util.cc.o
grpc/libgrpc++_alts.a: grpc/CMakeFiles/grpc++_alts.dir/build.make
grpc/libgrpc++_alts.a: grpc/CMakeFiles/grpc++_alts.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libgrpc++_alts.a"
	cd /root/ricki/UA_BlackJack_Server/build/grpc && $(CMAKE_COMMAND) -P CMakeFiles/grpc++_alts.dir/cmake_clean_target.cmake
	cd /root/ricki/UA_BlackJack_Server/build/grpc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/grpc++_alts.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
grpc/CMakeFiles/grpc++_alts.dir/build: grpc/libgrpc++_alts.a

.PHONY : grpc/CMakeFiles/grpc++_alts.dir/build

grpc/CMakeFiles/grpc++_alts.dir/clean:
	cd /root/ricki/UA_BlackJack_Server/build/grpc && $(CMAKE_COMMAND) -P CMakeFiles/grpc++_alts.dir/cmake_clean.cmake
.PHONY : grpc/CMakeFiles/grpc++_alts.dir/clean

grpc/CMakeFiles/grpc++_alts.dir/depend:
	cd /root/ricki/UA_BlackJack_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ricki/UA_BlackJack_Server /root/ricki/UA_BlackJack_Server/thirdparty/grpc /root/ricki/UA_BlackJack_Server/build /root/ricki/UA_BlackJack_Server/build/grpc /root/ricki/UA_BlackJack_Server/build/grpc/CMakeFiles/grpc++_alts.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grpc/CMakeFiles/grpc++_alts.dir/depend

