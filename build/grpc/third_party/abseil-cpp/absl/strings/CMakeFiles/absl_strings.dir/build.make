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
include grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/depend.make

# Include the progress variables for this target.
include grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/progress.make

# Include the compile flags for this target's objects.
include grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/flags.make

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/ascii.cc.o: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/flags.make
grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/ascii.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/strings/ascii.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/ascii.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_strings.dir/ascii.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/ascii.cc

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/ascii.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_strings.dir/ascii.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/ascii.cc > CMakeFiles/absl_strings.dir/ascii.cc.i

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/ascii.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_strings.dir/ascii.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/ascii.cc -o CMakeFiles/absl_strings.dir/ascii.cc.s

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/charconv.cc.o: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/flags.make
grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/charconv.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/strings/charconv.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/charconv.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_strings.dir/charconv.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/charconv.cc

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/charconv.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_strings.dir/charconv.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/charconv.cc > CMakeFiles/absl_strings.dir/charconv.cc.i

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/charconv.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_strings.dir/charconv.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/charconv.cc -o CMakeFiles/absl_strings.dir/charconv.cc.s

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/escaping.cc.o: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/flags.make
grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/escaping.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/strings/escaping.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/escaping.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_strings.dir/escaping.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/escaping.cc

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/escaping.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_strings.dir/escaping.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/escaping.cc > CMakeFiles/absl_strings.dir/escaping.cc.i

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/escaping.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_strings.dir/escaping.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/escaping.cc -o CMakeFiles/absl_strings.dir/escaping.cc.s

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/charconv_bigint.cc.o: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/flags.make
grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/charconv_bigint.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/strings/internal/charconv_bigint.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/charconv_bigint.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_strings.dir/internal/charconv_bigint.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/internal/charconv_bigint.cc

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/charconv_bigint.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_strings.dir/internal/charconv_bigint.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/internal/charconv_bigint.cc > CMakeFiles/absl_strings.dir/internal/charconv_bigint.cc.i

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/charconv_bigint.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_strings.dir/internal/charconv_bigint.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/internal/charconv_bigint.cc -o CMakeFiles/absl_strings.dir/internal/charconv_bigint.cc.s

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/charconv_parse.cc.o: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/flags.make
grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/charconv_parse.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/strings/internal/charconv_parse.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/charconv_parse.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_strings.dir/internal/charconv_parse.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/internal/charconv_parse.cc

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/charconv_parse.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_strings.dir/internal/charconv_parse.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/internal/charconv_parse.cc > CMakeFiles/absl_strings.dir/internal/charconv_parse.cc.i

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/charconv_parse.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_strings.dir/internal/charconv_parse.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/internal/charconv_parse.cc -o CMakeFiles/absl_strings.dir/internal/charconv_parse.cc.s

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/memutil.cc.o: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/flags.make
grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/memutil.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/strings/internal/memutil.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/memutil.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_strings.dir/internal/memutil.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/internal/memutil.cc

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/memutil.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_strings.dir/internal/memutil.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/internal/memutil.cc > CMakeFiles/absl_strings.dir/internal/memutil.cc.i

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/memutil.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_strings.dir/internal/memutil.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/internal/memutil.cc -o CMakeFiles/absl_strings.dir/internal/memutil.cc.s

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/match.cc.o: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/flags.make
grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/match.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/strings/match.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/match.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_strings.dir/match.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/match.cc

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/match.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_strings.dir/match.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/match.cc > CMakeFiles/absl_strings.dir/match.cc.i

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/match.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_strings.dir/match.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/match.cc -o CMakeFiles/absl_strings.dir/match.cc.s

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/numbers.cc.o: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/flags.make
grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/numbers.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/strings/numbers.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/numbers.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_strings.dir/numbers.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/numbers.cc

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/numbers.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_strings.dir/numbers.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/numbers.cc > CMakeFiles/absl_strings.dir/numbers.cc.i

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/numbers.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_strings.dir/numbers.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/numbers.cc -o CMakeFiles/absl_strings.dir/numbers.cc.s

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_cat.cc.o: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/flags.make
grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_cat.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/strings/str_cat.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_cat.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_strings.dir/str_cat.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/str_cat.cc

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_cat.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_strings.dir/str_cat.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/str_cat.cc > CMakeFiles/absl_strings.dir/str_cat.cc.i

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_cat.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_strings.dir/str_cat.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/str_cat.cc -o CMakeFiles/absl_strings.dir/str_cat.cc.s

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_replace.cc.o: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/flags.make
grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_replace.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/strings/str_replace.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_replace.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_strings.dir/str_replace.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/str_replace.cc

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_replace.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_strings.dir/str_replace.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/str_replace.cc > CMakeFiles/absl_strings.dir/str_replace.cc.i

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_replace.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_strings.dir/str_replace.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/str_replace.cc -o CMakeFiles/absl_strings.dir/str_replace.cc.s

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_split.cc.o: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/flags.make
grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_split.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/strings/str_split.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_split.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_strings.dir/str_split.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/str_split.cc

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_split.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_strings.dir/str_split.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/str_split.cc > CMakeFiles/absl_strings.dir/str_split.cc.i

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_split.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_strings.dir/str_split.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/str_split.cc -o CMakeFiles/absl_strings.dir/str_split.cc.s

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/string_view.cc.o: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/flags.make
grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/string_view.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/strings/string_view.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/string_view.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_strings.dir/string_view.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/string_view.cc

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/string_view.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_strings.dir/string_view.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/string_view.cc > CMakeFiles/absl_strings.dir/string_view.cc.i

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/string_view.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_strings.dir/string_view.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/string_view.cc -o CMakeFiles/absl_strings.dir/string_view.cc.s

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/substitute.cc.o: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/flags.make
grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/substitute.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/strings/substitute.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/substitute.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_strings.dir/substitute.cc.o -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/substitute.cc

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/substitute.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_strings.dir/substitute.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/substitute.cc > CMakeFiles/absl_strings.dir/substitute.cc.i

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/substitute.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_strings.dir/substitute.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings/substitute.cc -o CMakeFiles/absl_strings.dir/substitute.cc.s

# Object files for target absl_strings
absl_strings_OBJECTS = \
"CMakeFiles/absl_strings.dir/ascii.cc.o" \
"CMakeFiles/absl_strings.dir/charconv.cc.o" \
"CMakeFiles/absl_strings.dir/escaping.cc.o" \
"CMakeFiles/absl_strings.dir/internal/charconv_bigint.cc.o" \
"CMakeFiles/absl_strings.dir/internal/charconv_parse.cc.o" \
"CMakeFiles/absl_strings.dir/internal/memutil.cc.o" \
"CMakeFiles/absl_strings.dir/match.cc.o" \
"CMakeFiles/absl_strings.dir/numbers.cc.o" \
"CMakeFiles/absl_strings.dir/str_cat.cc.o" \
"CMakeFiles/absl_strings.dir/str_replace.cc.o" \
"CMakeFiles/absl_strings.dir/str_split.cc.o" \
"CMakeFiles/absl_strings.dir/string_view.cc.o" \
"CMakeFiles/absl_strings.dir/substitute.cc.o"

# External object files for target absl_strings
absl_strings_EXTERNAL_OBJECTS =

grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/ascii.cc.o
grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/charconv.cc.o
grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/escaping.cc.o
grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/charconv_bigint.cc.o
grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/charconv_parse.cc.o
grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/internal/memutil.cc.o
grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/match.cc.o
grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/numbers.cc.o
grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_cat.cc.o
grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_replace.cc.o
grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/str_split.cc.o
grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/string_view.cc.o
grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/substitute.cc.o
grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/build.make
grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a: grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX static library libabsl_strings.a"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && $(CMAKE_COMMAND) -P CMakeFiles/absl_strings.dir/cmake_clean_target.cmake
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/absl_strings.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/build: grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a

.PHONY : grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/build

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/clean:
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings && $(CMAKE_COMMAND) -P CMakeFiles/absl_strings.dir/cmake_clean.cmake
.PHONY : grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/clean

grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/depend:
	cd /home/tong/UA_BlackJack_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tong/UA_BlackJack_Server /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/strings /home/tong/UA_BlackJack_Server/build /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings /home/tong/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grpc/third_party/abseil-cpp/absl/strings/CMakeFiles/absl_strings.dir/depend
