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
include grpc/third_party/zlib/CMakeFiles/zlib.dir/depend.make

# Include the progress variables for this target.
include grpc/third_party/zlib/CMakeFiles/zlib.dir/progress.make

# Include the compile flags for this target's objects.
include grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make

grpc/third_party/zlib/CMakeFiles/zlib.dir/adler32.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/adler32.o: ../thirdparty/grpc/third_party/zlib/adler32.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/adler32.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/adler32.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/adler32.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/adler32.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/adler32.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/adler32.c > CMakeFiles/zlib.dir/adler32.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/adler32.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/adler32.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/adler32.c -o CMakeFiles/zlib.dir/adler32.s

grpc/third_party/zlib/CMakeFiles/zlib.dir/compress.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/compress.o: ../thirdparty/grpc/third_party/zlib/compress.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/compress.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/compress.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/compress.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/compress.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/compress.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/compress.c > CMakeFiles/zlib.dir/compress.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/compress.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/compress.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/compress.c -o CMakeFiles/zlib.dir/compress.s

grpc/third_party/zlib/CMakeFiles/zlib.dir/crc32.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/crc32.o: ../thirdparty/grpc/third_party/zlib/crc32.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/crc32.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/crc32.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/crc32.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/crc32.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/crc32.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/crc32.c > CMakeFiles/zlib.dir/crc32.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/crc32.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/crc32.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/crc32.c -o CMakeFiles/zlib.dir/crc32.s

grpc/third_party/zlib/CMakeFiles/zlib.dir/deflate.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/deflate.o: ../thirdparty/grpc/third_party/zlib/deflate.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/deflate.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/deflate.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/deflate.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/deflate.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/deflate.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/deflate.c > CMakeFiles/zlib.dir/deflate.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/deflate.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/deflate.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/deflate.c -o CMakeFiles/zlib.dir/deflate.s

grpc/third_party/zlib/CMakeFiles/zlib.dir/gzclose.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/gzclose.o: ../thirdparty/grpc/third_party/zlib/gzclose.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/gzclose.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/gzclose.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/gzclose.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/gzclose.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/gzclose.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/gzclose.c > CMakeFiles/zlib.dir/gzclose.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/gzclose.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/gzclose.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/gzclose.c -o CMakeFiles/zlib.dir/gzclose.s

grpc/third_party/zlib/CMakeFiles/zlib.dir/gzlib.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/gzlib.o: ../thirdparty/grpc/third_party/zlib/gzlib.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/gzlib.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/gzlib.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/gzlib.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/gzlib.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/gzlib.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/gzlib.c > CMakeFiles/zlib.dir/gzlib.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/gzlib.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/gzlib.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/gzlib.c -o CMakeFiles/zlib.dir/gzlib.s

grpc/third_party/zlib/CMakeFiles/zlib.dir/gzread.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/gzread.o: ../thirdparty/grpc/third_party/zlib/gzread.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/gzread.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/gzread.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/gzread.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/gzread.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/gzread.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/gzread.c > CMakeFiles/zlib.dir/gzread.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/gzread.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/gzread.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/gzread.c -o CMakeFiles/zlib.dir/gzread.s

grpc/third_party/zlib/CMakeFiles/zlib.dir/gzwrite.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/gzwrite.o: ../thirdparty/grpc/third_party/zlib/gzwrite.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/gzwrite.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/gzwrite.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/gzwrite.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/gzwrite.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/gzwrite.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/gzwrite.c > CMakeFiles/zlib.dir/gzwrite.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/gzwrite.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/gzwrite.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/gzwrite.c -o CMakeFiles/zlib.dir/gzwrite.s

grpc/third_party/zlib/CMakeFiles/zlib.dir/inflate.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/inflate.o: ../thirdparty/grpc/third_party/zlib/inflate.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/inflate.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/inflate.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/inflate.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/inflate.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/inflate.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/inflate.c > CMakeFiles/zlib.dir/inflate.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/inflate.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/inflate.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/inflate.c -o CMakeFiles/zlib.dir/inflate.s

grpc/third_party/zlib/CMakeFiles/zlib.dir/infback.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/infback.o: ../thirdparty/grpc/third_party/zlib/infback.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/infback.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/infback.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/infback.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/infback.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/infback.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/infback.c > CMakeFiles/zlib.dir/infback.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/infback.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/infback.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/infback.c -o CMakeFiles/zlib.dir/infback.s

grpc/third_party/zlib/CMakeFiles/zlib.dir/inftrees.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/inftrees.o: ../thirdparty/grpc/third_party/zlib/inftrees.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/inftrees.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/inftrees.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/inftrees.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/inftrees.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/inftrees.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/inftrees.c > CMakeFiles/zlib.dir/inftrees.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/inftrees.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/inftrees.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/inftrees.c -o CMakeFiles/zlib.dir/inftrees.s

grpc/third_party/zlib/CMakeFiles/zlib.dir/inffast.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/inffast.o: ../thirdparty/grpc/third_party/zlib/inffast.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/inffast.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/inffast.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/inffast.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/inffast.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/inffast.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/inffast.c > CMakeFiles/zlib.dir/inffast.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/inffast.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/inffast.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/inffast.c -o CMakeFiles/zlib.dir/inffast.s

grpc/third_party/zlib/CMakeFiles/zlib.dir/trees.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/trees.o: ../thirdparty/grpc/third_party/zlib/trees.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/trees.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/trees.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/trees.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/trees.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/trees.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/trees.c > CMakeFiles/zlib.dir/trees.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/trees.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/trees.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/trees.c -o CMakeFiles/zlib.dir/trees.s

grpc/third_party/zlib/CMakeFiles/zlib.dir/uncompr.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/uncompr.o: ../thirdparty/grpc/third_party/zlib/uncompr.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/uncompr.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/uncompr.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/uncompr.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/uncompr.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/uncompr.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/uncompr.c > CMakeFiles/zlib.dir/uncompr.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/uncompr.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/uncompr.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/uncompr.c -o CMakeFiles/zlib.dir/uncompr.s

grpc/third_party/zlib/CMakeFiles/zlib.dir/zutil.o: grpc/third_party/zlib/CMakeFiles/zlib.dir/flags.make
grpc/third_party/zlib/CMakeFiles/zlib.dir/zutil.o: ../thirdparty/grpc/third_party/zlib/zutil.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building C object grpc/third_party/zlib/CMakeFiles/zlib.dir/zutil.o"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zlib.dir/zutil.o   -c /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/zutil.c

grpc/third_party/zlib/CMakeFiles/zlib.dir/zutil.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zlib.dir/zutil.i"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/zutil.c > CMakeFiles/zlib.dir/zutil.i

grpc/third_party/zlib/CMakeFiles/zlib.dir/zutil.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zlib.dir/zutil.s"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib/zutil.c -o CMakeFiles/zlib.dir/zutil.s

# Object files for target zlib
zlib_OBJECTS = \
"CMakeFiles/zlib.dir/adler32.o" \
"CMakeFiles/zlib.dir/compress.o" \
"CMakeFiles/zlib.dir/crc32.o" \
"CMakeFiles/zlib.dir/deflate.o" \
"CMakeFiles/zlib.dir/gzclose.o" \
"CMakeFiles/zlib.dir/gzlib.o" \
"CMakeFiles/zlib.dir/gzread.o" \
"CMakeFiles/zlib.dir/gzwrite.o" \
"CMakeFiles/zlib.dir/inflate.o" \
"CMakeFiles/zlib.dir/infback.o" \
"CMakeFiles/zlib.dir/inftrees.o" \
"CMakeFiles/zlib.dir/inffast.o" \
"CMakeFiles/zlib.dir/trees.o" \
"CMakeFiles/zlib.dir/uncompr.o" \
"CMakeFiles/zlib.dir/zutil.o"

# External object files for target zlib
zlib_EXTERNAL_OBJECTS =

grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/adler32.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/compress.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/crc32.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/deflate.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/gzclose.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/gzlib.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/gzread.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/gzwrite.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/inflate.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/infback.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/inftrees.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/inffast.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/trees.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/uncompr.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/zutil.o
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/build.make
grpc/third_party/zlib/libz.so.1.2.11: grpc/third_party/zlib/CMakeFiles/zlib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Linking C shared library libz.so"
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/zlib.dir/link.txt --verbose=$(VERBOSE)
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && $(CMAKE_COMMAND) -E cmake_symlink_library libz.so.1.2.11 libz.so.1 libz.so

grpc/third_party/zlib/libz.so.1: grpc/third_party/zlib/libz.so.1.2.11
	@$(CMAKE_COMMAND) -E touch_nocreate grpc/third_party/zlib/libz.so.1

grpc/third_party/zlib/libz.so: grpc/third_party/zlib/libz.so.1.2.11
	@$(CMAKE_COMMAND) -E touch_nocreate grpc/third_party/zlib/libz.so

# Rule to build all files generated by this target.
grpc/third_party/zlib/CMakeFiles/zlib.dir/build: grpc/third_party/zlib/libz.so

.PHONY : grpc/third_party/zlib/CMakeFiles/zlib.dir/build

grpc/third_party/zlib/CMakeFiles/zlib.dir/clean:
	cd /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib && $(CMAKE_COMMAND) -P CMakeFiles/zlib.dir/cmake_clean.cmake
.PHONY : grpc/third_party/zlib/CMakeFiles/zlib.dir/clean

grpc/third_party/zlib/CMakeFiles/zlib.dir/depend:
	cd /home/tong/UA_BlackJack_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tong/UA_BlackJack_Server /home/tong/UA_BlackJack_Server/thirdparty/grpc/third_party/zlib /home/tong/UA_BlackJack_Server/build /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib /home/tong/UA_BlackJack_Server/build/grpc/third_party/zlib/CMakeFiles/zlib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grpc/third_party/zlib/CMakeFiles/zlib.dir/depend

