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
include grpc/third_party/re2/CMakeFiles/string_generator_test.dir/depend.make

# Include the progress variables for this target.
include grpc/third_party/re2/CMakeFiles/string_generator_test.dir/progress.make

# Include the compile flags for this target's objects.
include grpc/third_party/re2/CMakeFiles/string_generator_test.dir/flags.make

grpc/third_party/re2/CMakeFiles/string_generator_test.dir/re2/testing/string_generator_test.cc.o: grpc/third_party/re2/CMakeFiles/string_generator_test.dir/flags.make
grpc/third_party/re2/CMakeFiles/string_generator_test.dir/re2/testing/string_generator_test.cc.o: ../thirdparty/grpc/third_party/re2/re2/testing/string_generator_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object grpc/third_party/re2/CMakeFiles/string_generator_test.dir/re2/testing/string_generator_test.cc.o"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/re2 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/string_generator_test.dir/re2/testing/string_generator_test.cc.o -c /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/re2/re2/testing/string_generator_test.cc

grpc/third_party/re2/CMakeFiles/string_generator_test.dir/re2/testing/string_generator_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/string_generator_test.dir/re2/testing/string_generator_test.cc.i"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/re2 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/re2/re2/testing/string_generator_test.cc > CMakeFiles/string_generator_test.dir/re2/testing/string_generator_test.cc.i

grpc/third_party/re2/CMakeFiles/string_generator_test.dir/re2/testing/string_generator_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/string_generator_test.dir/re2/testing/string_generator_test.cc.s"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/re2 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/re2/re2/testing/string_generator_test.cc -o CMakeFiles/string_generator_test.dir/re2/testing/string_generator_test.cc.s

grpc/third_party/re2/CMakeFiles/string_generator_test.dir/util/test.cc.o: grpc/third_party/re2/CMakeFiles/string_generator_test.dir/flags.make
grpc/third_party/re2/CMakeFiles/string_generator_test.dir/util/test.cc.o: ../thirdparty/grpc/third_party/re2/util/test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object grpc/third_party/re2/CMakeFiles/string_generator_test.dir/util/test.cc.o"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/re2 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/string_generator_test.dir/util/test.cc.o -c /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/re2/util/test.cc

grpc/third_party/re2/CMakeFiles/string_generator_test.dir/util/test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/string_generator_test.dir/util/test.cc.i"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/re2 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/re2/util/test.cc > CMakeFiles/string_generator_test.dir/util/test.cc.i

grpc/third_party/re2/CMakeFiles/string_generator_test.dir/util/test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/string_generator_test.dir/util/test.cc.s"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/re2 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/re2/util/test.cc -o CMakeFiles/string_generator_test.dir/util/test.cc.s

# Object files for target string_generator_test
string_generator_test_OBJECTS = \
"CMakeFiles/string_generator_test.dir/re2/testing/string_generator_test.cc.o" \
"CMakeFiles/string_generator_test.dir/util/test.cc.o"

# External object files for target string_generator_test
string_generator_test_EXTERNAL_OBJECTS =

grpc/third_party/re2/string_generator_test: grpc/third_party/re2/CMakeFiles/string_generator_test.dir/re2/testing/string_generator_test.cc.o
grpc/third_party/re2/string_generator_test: grpc/third_party/re2/CMakeFiles/string_generator_test.dir/util/test.cc.o
grpc/third_party/re2/string_generator_test: grpc/third_party/re2/CMakeFiles/string_generator_test.dir/build.make
grpc/third_party/re2/string_generator_test: grpc/third_party/re2/libtesting.a
grpc/third_party/re2/string_generator_test: grpc/third_party/re2/libre2.a
grpc/third_party/re2/string_generator_test: grpc/third_party/re2/CMakeFiles/string_generator_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable string_generator_test"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/re2 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/string_generator_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
grpc/third_party/re2/CMakeFiles/string_generator_test.dir/build: grpc/third_party/re2/string_generator_test

.PHONY : grpc/third_party/re2/CMakeFiles/string_generator_test.dir/build

grpc/third_party/re2/CMakeFiles/string_generator_test.dir/clean:
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/re2 && $(CMAKE_COMMAND) -P CMakeFiles/string_generator_test.dir/cmake_clean.cmake
.PHONY : grpc/third_party/re2/CMakeFiles/string_generator_test.dir/clean

grpc/third_party/re2/CMakeFiles/string_generator_test.dir/depend:
	cd /root/ricki/UA_BlackJack_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ricki/UA_BlackJack_Server /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/re2 /root/ricki/UA_BlackJack_Server/build /root/ricki/UA_BlackJack_Server/build/grpc/third_party/re2 /root/ricki/UA_BlackJack_Server/build/grpc/third_party/re2/CMakeFiles/string_generator_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grpc/third_party/re2/CMakeFiles/string_generator_test.dir/depend

