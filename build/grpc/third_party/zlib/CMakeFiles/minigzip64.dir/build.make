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
include grpc/third_party/zlib/CMakeFiles/minigzip64.dir/depend.make

# Include the progress variables for this target.
include grpc/third_party/zlib/CMakeFiles/minigzip64.dir/progress.make

# Include the compile flags for this target's objects.
include grpc/third_party/zlib/CMakeFiles/minigzip64.dir/flags.make

grpc/third_party/zlib/CMakeFiles/minigzip64.dir/test/minigzip.o: grpc/third_party/zlib/CMakeFiles/minigzip64.dir/flags.make
grpc/third_party/zlib/CMakeFiles/minigzip64.dir/test/minigzip.o: ../thirdparty/grpc/third_party/zlib/test/minigzip.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object grpc/third_party/zlib/CMakeFiles/minigzip64.dir/test/minigzip.o"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/minigzip64.dir/test/minigzip.o   -c /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/test/minigzip.c

grpc/third_party/zlib/CMakeFiles/minigzip64.dir/test/minigzip.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/minigzip64.dir/test/minigzip.i"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/test/minigzip.c > CMakeFiles/minigzip64.dir/test/minigzip.i

grpc/third_party/zlib/CMakeFiles/minigzip64.dir/test/minigzip.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/minigzip64.dir/test/minigzip.s"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/test/minigzip.c -o CMakeFiles/minigzip64.dir/test/minigzip.s

# Object files for target minigzip64
minigzip64_OBJECTS = \
"CMakeFiles/minigzip64.dir/test/minigzip.o"

# External object files for target minigzip64
minigzip64_EXTERNAL_OBJECTS =

grpc/third_party/zlib/minigzip64: grpc/third_party/zlib/CMakeFiles/minigzip64.dir/test/minigzip.o
grpc/third_party/zlib/minigzip64: grpc/third_party/zlib/CMakeFiles/minigzip64.dir/build.make
grpc/third_party/zlib/minigzip64: grpc/third_party/zlib/libz.so.1.2.11
grpc/third_party/zlib/minigzip64: grpc/third_party/zlib/CMakeFiles/minigzip64.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable minigzip64"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/zlib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/minigzip64.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
grpc/third_party/zlib/CMakeFiles/minigzip64.dir/build: grpc/third_party/zlib/minigzip64

.PHONY : grpc/third_party/zlib/CMakeFiles/minigzip64.dir/build

grpc/third_party/zlib/CMakeFiles/minigzip64.dir/clean:
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/zlib && $(CMAKE_COMMAND) -P CMakeFiles/minigzip64.dir/cmake_clean.cmake
.PHONY : grpc/third_party/zlib/CMakeFiles/minigzip64.dir/clean

grpc/third_party/zlib/CMakeFiles/minigzip64.dir/depend:
	cd /root/ricki/UA_BlackJack_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ricki/UA_BlackJack_Server /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib /root/ricki/UA_BlackJack_Server/build /root/ricki/UA_BlackJack_Server/build/grpc/third_party/zlib /root/ricki/UA_BlackJack_Server/build/grpc/third_party/zlib/CMakeFiles/minigzip64.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grpc/third_party/zlib/CMakeFiles/minigzip64.dir/depend

