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

# Utility rule file for tools_cxx.

# Include the progress variables for this target.
include grpc/CMakeFiles/tools_cxx.dir/progress.make

grpc/CMakeFiles/tools_cxx: grpc/gen_hpack_tables
grpc/CMakeFiles/tools_cxx: grpc/gen_legal_metadata_characters
grpc/CMakeFiles/tools_cxx: grpc/gen_percent_encoding_tables


tools_cxx: grpc/CMakeFiles/tools_cxx
tools_cxx: grpc/CMakeFiles/tools_cxx.dir/build.make

.PHONY : tools_cxx

# Rule to build all files generated by this target.
grpc/CMakeFiles/tools_cxx.dir/build: tools_cxx

.PHONY : grpc/CMakeFiles/tools_cxx.dir/build

grpc/CMakeFiles/tools_cxx.dir/clean:
	cd /root/ricki/UA_BlackJack_Server/build/grpc && $(CMAKE_COMMAND) -P CMakeFiles/tools_cxx.dir/cmake_clean.cmake
.PHONY : grpc/CMakeFiles/tools_cxx.dir/clean

grpc/CMakeFiles/tools_cxx.dir/depend:
	cd /root/ricki/UA_BlackJack_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ricki/UA_BlackJack_Server /root/ricki/UA_BlackJack_Server/thirdparty/grpc /root/ricki/UA_BlackJack_Server/build /root/ricki/UA_BlackJack_Server/build/grpc /root/ricki/UA_BlackJack_Server/build/grpc/CMakeFiles/tools_cxx.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grpc/CMakeFiles/tools_cxx.dir/depend

