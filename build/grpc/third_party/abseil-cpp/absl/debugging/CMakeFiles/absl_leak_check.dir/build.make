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
include grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/depend.make

# Include the progress variables for this target.
include grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/progress.make

# Include the compile flags for this target's objects.
include grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/flags.make

grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/leak_check.cc.o: grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/flags.make
grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/leak_check.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/debugging/leak_check.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/leak_check.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/debugging && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_leak_check.dir/leak_check.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/debugging/leak_check.cc

grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/leak_check.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_leak_check.dir/leak_check.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/debugging && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/debugging/leak_check.cc > CMakeFiles/absl_leak_check.dir/leak_check.cc.i

grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/leak_check.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_leak_check.dir/leak_check.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/debugging && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/debugging/leak_check.cc -o CMakeFiles/absl_leak_check.dir/leak_check.cc.s

# Object files for target absl_leak_check
absl_leak_check_OBJECTS = \
"CMakeFiles/absl_leak_check.dir/leak_check.cc.o"

# External object files for target absl_leak_check
absl_leak_check_EXTERNAL_OBJECTS =

grpc/third_party/abseil-cpp/absl/debugging/libabsl_leak_check.a: grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/leak_check.cc.o
grpc/third_party/abseil-cpp/absl/debugging/libabsl_leak_check.a: grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/build.make
grpc/third_party/abseil-cpp/absl/debugging/libabsl_leak_check.a: grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libabsl_leak_check.a"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/debugging && $(CMAKE_COMMAND) -P CMakeFiles/absl_leak_check.dir/cmake_clean_target.cmake
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/debugging && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/absl_leak_check.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/build: grpc/third_party/abseil-cpp/absl/debugging/libabsl_leak_check.a

.PHONY : grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/build

grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/clean:
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/debugging && $(CMAKE_COMMAND) -P CMakeFiles/absl_leak_check.dir/cmake_clean.cmake
.PHONY : grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/clean

grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/depend:
	cd /home/tong/UA_BlackJack_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tong/UA_BlackJack_Server /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/debugging /home/tong/UA_BlackJack_Server/build /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/debugging /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grpc/third_party/abseil-cpp/absl/debugging/CMakeFiles/absl_leak_check.dir/depend

