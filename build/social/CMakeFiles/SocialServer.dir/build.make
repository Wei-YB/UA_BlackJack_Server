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
include social/CMakeFiles/SocialServer.dir/depend.make

# Include the progress variables for this target.
include social/CMakeFiles/SocialServer.dir/progress.make

# Include the compile flags for this target's objects.
include social/CMakeFiles/SocialServer.dir/flags.make

../SocialServer/grpc/UA_BlackJack.pb.cc: ../protos/UA_BlackJack.proto
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ../../SocialServer/grpc/UA_BlackJack.pb.cc, ../../SocialServer/grpc/UA_BlackJack.pb.h, ../../SocialServer/grpc/UA_BlackJack.grpc.pb.cc, ../../SocialServer/grpc/UA_BlackJack.grpc.pb.h"
	cd /home/tong/UA_BlackJack_Server/build/social && ../grpc/third_party/protobuf/protoc-3.15.8.0 --grpc_out /home/tong/UA_BlackJack_Server/SocialServer/grpc --cpp_out /home/tong/UA_BlackJack_Server/SocialServer/grpc -I /home/tong/UA_BlackJack_Server/protos --plugin=protoc-gen-grpc="/home/tong/UA_BlackJack_Server/build/grpc/grpc_cpp_plugin" /home/tong/UA_BlackJack_Server/protos/UA_BlackJack.proto

../SocialServer/grpc/UA_BlackJack.pb.h: ../SocialServer/grpc/UA_BlackJack.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate ../SocialServer/grpc/UA_BlackJack.pb.h

../SocialServer/grpc/UA_BlackJack.grpc.pb.cc: ../SocialServer/grpc/UA_BlackJack.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate ../SocialServer/grpc/UA_BlackJack.grpc.pb.cc

../SocialServer/grpc/UA_BlackJack.grpc.pb.h: ../SocialServer/grpc/UA_BlackJack.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate ../SocialServer/grpc/UA_BlackJack.grpc.pb.h

social/CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.grpc.pb.cc.o: social/CMakeFiles/SocialServer.dir/flags.make
social/CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.grpc.pb.cc.o: ../SocialServer/grpc/UA_BlackJack.grpc.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object social/CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.grpc.pb.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.grpc.pb.cc.o -c /home/tong/UA_BlackJack_Server/SocialServer/grpc/UA_BlackJack.grpc.pb.cc

social/CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.grpc.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.grpc.pb.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/SocialServer/grpc/UA_BlackJack.grpc.pb.cc > CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.grpc.pb.cc.i

social/CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.grpc.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.grpc.pb.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/SocialServer/grpc/UA_BlackJack.grpc.pb.cc -o CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.grpc.pb.cc.s

social/CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.pb.cc.o: social/CMakeFiles/SocialServer.dir/flags.make
social/CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.pb.cc.o: ../SocialServer/grpc/UA_BlackJack.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object social/CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.pb.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.pb.cc.o -c /home/tong/UA_BlackJack_Server/SocialServer/grpc/UA_BlackJack.pb.cc

social/CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.pb.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/SocialServer/grpc/UA_BlackJack.pb.cc > CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.pb.cc.i

social/CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.pb.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/SocialServer/grpc/UA_BlackJack.pb.cc -o CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.pb.cc.s

social/CMakeFiles/SocialServer.dir/src/SocialClientDB-syn.cc.o: social/CMakeFiles/SocialServer.dir/flags.make
social/CMakeFiles/SocialServer.dir/src/SocialClientDB-syn.cc.o: ../SocialServer/src/SocialClientDB-syn.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object social/CMakeFiles/SocialServer.dir/src/SocialClientDB-syn.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SocialServer.dir/src/SocialClientDB-syn.cc.o -c /home/tong/UA_BlackJack_Server/SocialServer/src/SocialClientDB-syn.cc

social/CMakeFiles/SocialServer.dir/src/SocialClientDB-syn.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SocialServer.dir/src/SocialClientDB-syn.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/SocialServer/src/SocialClientDB-syn.cc > CMakeFiles/SocialServer.dir/src/SocialClientDB-syn.cc.i

social/CMakeFiles/SocialServer.dir/src/SocialClientDB-syn.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SocialServer.dir/src/SocialClientDB-syn.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/SocialServer/src/SocialClientDB-syn.cc -o CMakeFiles/SocialServer.dir/src/SocialClientDB-syn.cc.s

social/CMakeFiles/SocialServer.dir/src/SocialServer-asyn.cc.o: social/CMakeFiles/SocialServer.dir/flags.make
social/CMakeFiles/SocialServer.dir/src/SocialServer-asyn.cc.o: ../SocialServer/src/SocialServer-asyn.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object social/CMakeFiles/SocialServer.dir/src/SocialServer-asyn.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SocialServer.dir/src/SocialServer-asyn.cc.o -c /home/tong/UA_BlackJack_Server/SocialServer/src/SocialServer-asyn.cc

social/CMakeFiles/SocialServer.dir/src/SocialServer-asyn.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SocialServer.dir/src/SocialServer-asyn.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/SocialServer/src/SocialServer-asyn.cc > CMakeFiles/SocialServer.dir/src/SocialServer-asyn.cc.i

social/CMakeFiles/SocialServer.dir/src/SocialServer-asyn.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SocialServer.dir/src/SocialServer-asyn.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/SocialServer/src/SocialServer-asyn.cc -o CMakeFiles/SocialServer.dir/src/SocialServer-asyn.cc.s

social/CMakeFiles/SocialServer.dir/src/main.cc.o: social/CMakeFiles/SocialServer.dir/flags.make
social/CMakeFiles/SocialServer.dir/src/main.cc.o: ../SocialServer/src/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object social/CMakeFiles/SocialServer.dir/src/main.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SocialServer.dir/src/main.cc.o -c /home/tong/UA_BlackJack_Server/SocialServer/src/main.cc

social/CMakeFiles/SocialServer.dir/src/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SocialServer.dir/src/main.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/SocialServer/src/main.cc > CMakeFiles/SocialServer.dir/src/main.cc.i

social/CMakeFiles/SocialServer.dir/src/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SocialServer.dir/src/main.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/social && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/SocialServer/src/main.cc -o CMakeFiles/SocialServer.dir/src/main.cc.s

# Object files for target SocialServer
SocialServer_OBJECTS = \
"CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.grpc.pb.cc.o" \
"CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.pb.cc.o" \
"CMakeFiles/SocialServer.dir/src/SocialClientDB-syn.cc.o" \
"CMakeFiles/SocialServer.dir/src/SocialServer-asyn.cc.o" \
"CMakeFiles/SocialServer.dir/src/main.cc.o"

# External object files for target SocialServer
SocialServer_EXTERNAL_OBJECTS =

social/SocialServer: social/CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.grpc.pb.cc.o
social/SocialServer: social/CMakeFiles/SocialServer.dir/grpc/UA_BlackJack.pb.cc.o
social/SocialServer: social/CMakeFiles/SocialServer.dir/src/SocialClientDB-syn.cc.o
social/SocialServer: social/CMakeFiles/SocialServer.dir/src/SocialServer-asyn.cc.o
social/SocialServer: social/CMakeFiles/SocialServer.dir/src/main.cc.o
social/SocialServer: social/CMakeFiles/SocialServer.dir/build.make
social/SocialServer: grpc/libgrpc++_reflection.a
social/SocialServer: grpc/libgrpc++.a
social/SocialServer: grpc/third_party/protobuf/libprotobuf.a
social/SocialServer: ../thirdparty/spdlog/libspdlog.a
social/SocialServer: grpc/libgrpc.a
social/SocialServer: grpc/third_party/zlib/libz.a
social/SocialServer: grpc/third_party/cares/cares/lib/libcares.a
social/SocialServer: grpc/libaddress_sorting.a
social/SocialServer: grpc/third_party/re2/libre2.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/hash/libabsl_hash.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/hash/libabsl_city.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/hash/libabsl_wyhash.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/container/libabsl_raw_hash_set.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/container/libabsl_hashtablez_sampler.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/base/libabsl_exponential_biased.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/status/libabsl_statusor.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/types/libabsl_bad_variant_access.a
social/SocialServer: grpc/libgpr.a
social/SocialServer: grpc/libupb.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/status/libabsl_status.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/strings/libabsl_cord.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/strings/libabsl_str_format_internal.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/synchronization/libabsl_synchronization.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/debugging/libabsl_stacktrace.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/debugging/libabsl_symbolize.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/debugging/libabsl_debugging_internal.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/debugging/libabsl_demangle_internal.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/synchronization/libabsl_graphcycles_internal.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/base/libabsl_malloc_internal.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/time/libabsl_time.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/base/libabsl_throw_delegate.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/strings/libabsl_strings_internal.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/base/libabsl_base.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/base/libabsl_spinlock_wait.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/numeric/libabsl_int128.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/time/libabsl_civil_time.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/time/libabsl_time_zone.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/types/libabsl_bad_optional_access.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/base/libabsl_raw_logging_internal.a
social/SocialServer: grpc/third_party/abseil-cpp/absl/base/libabsl_log_severity.a
social/SocialServer: grpc/third_party/boringssl-with-bazel/libssl.a
social/SocialServer: grpc/third_party/boringssl-with-bazel/libcrypto.a
social/SocialServer: social/CMakeFiles/SocialServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable SocialServer"
	cd /home/tong/UA_BlackJack_Server/build/social && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SocialServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
social/CMakeFiles/SocialServer.dir/build: social/SocialServer

.PHONY : social/CMakeFiles/SocialServer.dir/build

social/CMakeFiles/SocialServer.dir/clean:
	cd /home/tong/UA_BlackJack_Server/build/social && $(CMAKE_COMMAND) -P CMakeFiles/SocialServer.dir/cmake_clean.cmake
.PHONY : social/CMakeFiles/SocialServer.dir/clean

social/CMakeFiles/SocialServer.dir/depend: ../SocialServer/grpc/UA_BlackJack.pb.cc
social/CMakeFiles/SocialServer.dir/depend: ../SocialServer/grpc/UA_BlackJack.pb.h
social/CMakeFiles/SocialServer.dir/depend: ../SocialServer/grpc/UA_BlackJack.grpc.pb.cc
social/CMakeFiles/SocialServer.dir/depend: ../SocialServer/grpc/UA_BlackJack.grpc.pb.h
	cd /home/tong/UA_BlackJack_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tong/UA_BlackJack_Server /home/tong/UA_BlackJack_Server/SocialServer /home/tong/UA_BlackJack_Server/build /home/tong/UA_BlackJack_Server/build/social /home/tong/UA_BlackJack_Server/build/social/CMakeFiles/SocialServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : social/CMakeFiles/SocialServer.dir/depend
