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
include client/CMakeFiles/Client.dir/depend.make

# Include the progress variables for this target.
include client/CMakeFiles/Client.dir/progress.make

# Include the compile flags for this target's objects.
include client/CMakeFiles/Client.dir/flags.make

../Client/grpc/UA_BlackJack.pb.cc: ../protos/UA_BlackJack.proto
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ../../Client/grpc/UA_BlackJack.pb.cc, ../../Client/grpc/UA_BlackJack.pb.h, ../../Client/grpc/UA_BlackJack.grpc.pb.cc, ../../Client/grpc/UA_BlackJack.grpc.pb.h"
	cd /home/tong/UA_BlackJack_Server/build/client && ../grpc/third_party/protobuf/protoc-3.15.8.0 --grpc_out /home/tong/UA_BlackJack_Server/Client/grpc --cpp_out /home/tong/UA_BlackJack_Server/Client/grpc -I /home/tong/UA_BlackJack_Server/protos --plugin=protoc-gen-grpc="/home/tong/UA_BlackJack_Server/build/grpc/grpc_cpp_plugin" /home/tong/UA_BlackJack_Server/protos/UA_BlackJack.proto

../Client/grpc/UA_BlackJack.pb.h: ../Client/grpc/UA_BlackJack.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate ../Client/grpc/UA_BlackJack.pb.h

../Client/grpc/UA_BlackJack.grpc.pb.cc: ../Client/grpc/UA_BlackJack.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate ../Client/grpc/UA_BlackJack.grpc.pb.cc

../Client/grpc/UA_BlackJack.grpc.pb.h: ../Client/grpc/UA_BlackJack.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate ../Client/grpc/UA_BlackJack.grpc.pb.h

client/CMakeFiles/Client.dir/grpc/UA_BlackJack.grpc.pb.cc.o: client/CMakeFiles/Client.dir/flags.make
client/CMakeFiles/Client.dir/grpc/UA_BlackJack.grpc.pb.cc.o: ../Client/grpc/UA_BlackJack.grpc.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object client/CMakeFiles/Client.dir/grpc/UA_BlackJack.grpc.pb.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/grpc/UA_BlackJack.grpc.pb.cc.o -c /home/tong/UA_BlackJack_Server/Client/grpc/UA_BlackJack.grpc.pb.cc

client/CMakeFiles/Client.dir/grpc/UA_BlackJack.grpc.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/grpc/UA_BlackJack.grpc.pb.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/Client/grpc/UA_BlackJack.grpc.pb.cc > CMakeFiles/Client.dir/grpc/UA_BlackJack.grpc.pb.cc.i

client/CMakeFiles/Client.dir/grpc/UA_BlackJack.grpc.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/grpc/UA_BlackJack.grpc.pb.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/Client/grpc/UA_BlackJack.grpc.pb.cc -o CMakeFiles/Client.dir/grpc/UA_BlackJack.grpc.pb.cc.s

client/CMakeFiles/Client.dir/grpc/UA_BlackJack.pb.cc.o: client/CMakeFiles/Client.dir/flags.make
client/CMakeFiles/Client.dir/grpc/UA_BlackJack.pb.cc.o: ../Client/grpc/UA_BlackJack.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object client/CMakeFiles/Client.dir/grpc/UA_BlackJack.pb.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/grpc/UA_BlackJack.pb.cc.o -c /home/tong/UA_BlackJack_Server/Client/grpc/UA_BlackJack.pb.cc

client/CMakeFiles/Client.dir/grpc/UA_BlackJack.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/grpc/UA_BlackJack.pb.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/Client/grpc/UA_BlackJack.pb.cc > CMakeFiles/Client.dir/grpc/UA_BlackJack.pb.cc.i

client/CMakeFiles/Client.dir/grpc/UA_BlackJack.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/grpc/UA_BlackJack.pb.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/Client/grpc/UA_BlackJack.pb.cc -o CMakeFiles/Client.dir/grpc/UA_BlackJack.pb.cc.s

client/CMakeFiles/Client.dir/src/Client.cc.o: client/CMakeFiles/Client.dir/flags.make
client/CMakeFiles/Client.dir/src/Client.cc.o: ../Client/src/Client.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object client/CMakeFiles/Client.dir/src/Client.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/Client.cc.o -c /home/tong/UA_BlackJack_Server/Client/src/Client.cc

client/CMakeFiles/Client.dir/src/Client.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/Client.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/Client/src/Client.cc > CMakeFiles/Client.dir/src/Client.cc.i

client/CMakeFiles/Client.dir/src/Client.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/Client.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/Client/src/Client.cc -o CMakeFiles/Client.dir/src/Client.cc.s

client/CMakeFiles/Client.dir/src/Display.cc.o: client/CMakeFiles/Client.dir/flags.make
client/CMakeFiles/Client.dir/src/Display.cc.o: ../Client/src/Display.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object client/CMakeFiles/Client.dir/src/Display.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/Display.cc.o -c /home/tong/UA_BlackJack_Server/Client/src/Display.cc

client/CMakeFiles/Client.dir/src/Display.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/Display.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/Client/src/Display.cc > CMakeFiles/Client.dir/src/Display.cc.i

client/CMakeFiles/Client.dir/src/Display.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/Display.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/Client/src/Display.cc -o CMakeFiles/Client.dir/src/Display.cc.s

client/CMakeFiles/Client.dir/src/Rio.cc.o: client/CMakeFiles/Client.dir/flags.make
client/CMakeFiles/Client.dir/src/Rio.cc.o: ../Client/src/Rio.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object client/CMakeFiles/Client.dir/src/Rio.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/Rio.cc.o -c /home/tong/UA_BlackJack_Server/Client/src/Rio.cc

client/CMakeFiles/Client.dir/src/Rio.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/Rio.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/Client/src/Rio.cc > CMakeFiles/Client.dir/src/Rio.cc.i

client/CMakeFiles/Client.dir/src/Rio.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/Rio.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/client && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/Client/src/Rio.cc -o CMakeFiles/Client.dir/src/Rio.cc.s

# Object files for target Client
Client_OBJECTS = \
"CMakeFiles/Client.dir/grpc/UA_BlackJack.grpc.pb.cc.o" \
"CMakeFiles/Client.dir/grpc/UA_BlackJack.pb.cc.o" \
"CMakeFiles/Client.dir/src/Client.cc.o" \
"CMakeFiles/Client.dir/src/Display.cc.o" \
"CMakeFiles/Client.dir/src/Rio.cc.o"

# External object files for target Client
Client_EXTERNAL_OBJECTS =

client/Client: client/CMakeFiles/Client.dir/grpc/UA_BlackJack.grpc.pb.cc.o
client/Client: client/CMakeFiles/Client.dir/grpc/UA_BlackJack.pb.cc.o
client/Client: client/CMakeFiles/Client.dir/src/Client.cc.o
client/Client: client/CMakeFiles/Client.dir/src/Display.cc.o
client/Client: client/CMakeFiles/Client.dir/src/Rio.cc.o
client/Client: client/CMakeFiles/Client.dir/build.make
client/Client: grpc/libgrpc++_reflection.a
client/Client: grpc/libgrpc++.a
client/Client: grpc/third_party/protobuf/libprotobuf.a
client/Client: ../thirdparty/spdlog/libspdlog.a
client/Client: grpc/libgrpc.a
client/Client: grpc/third_party/zlib/libz.a
client/Client: grpc/third_party/cares/cares/lib/libcares.a
client/Client: grpc/libaddress_sorting.a
client/Client: grpc/third_party/re2/libre2.a
client/Client: grpc/third_party/abseil-cpp/absl/hash/libabsl_hash.a
client/Client: grpc/third_party/abseil-cpp/absl/hash/libabsl_city.a
client/Client: grpc/third_party/abseil-cpp/absl/hash/libabsl_wyhash.a
client/Client: grpc/third_party/abseil-cpp/absl/container/libabsl_raw_hash_set.a
client/Client: grpc/third_party/abseil-cpp/absl/container/libabsl_hashtablez_sampler.a
client/Client: grpc/third_party/abseil-cpp/absl/base/libabsl_exponential_biased.a
client/Client: grpc/third_party/abseil-cpp/absl/status/libabsl_statusor.a
client/Client: grpc/third_party/abseil-cpp/absl/types/libabsl_bad_variant_access.a
client/Client: grpc/libgpr.a
client/Client: grpc/libupb.a
client/Client: grpc/third_party/abseil-cpp/absl/status/libabsl_status.a
client/Client: grpc/third_party/abseil-cpp/absl/strings/libabsl_cord.a
client/Client: grpc/third_party/abseil-cpp/absl/strings/libabsl_str_format_internal.a
client/Client: grpc/third_party/abseil-cpp/absl/synchronization/libabsl_synchronization.a
client/Client: grpc/third_party/abseil-cpp/absl/debugging/libabsl_stacktrace.a
client/Client: grpc/third_party/abseil-cpp/absl/debugging/libabsl_symbolize.a
client/Client: grpc/third_party/abseil-cpp/absl/debugging/libabsl_debugging_internal.a
client/Client: grpc/third_party/abseil-cpp/absl/debugging/libabsl_demangle_internal.a
client/Client: grpc/third_party/abseil-cpp/absl/synchronization/libabsl_graphcycles_internal.a
client/Client: grpc/third_party/abseil-cpp/absl/base/libabsl_malloc_internal.a
client/Client: grpc/third_party/abseil-cpp/absl/time/libabsl_time.a
client/Client: grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a
client/Client: grpc/third_party/abseil-cpp/absl/base/libabsl_throw_delegate.a
client/Client: grpc/third_party/abseil-cpp/absl/strings/libabsl_strings_internal.a
client/Client: grpc/third_party/abseil-cpp/absl/base/libabsl_base.a
client/Client: grpc/third_party/abseil-cpp/absl/base/libabsl_spinlock_wait.a
client/Client: grpc/third_party/abseil-cpp/absl/numeric/libabsl_int128.a
client/Client: grpc/third_party/abseil-cpp/absl/time/libabsl_civil_time.a
client/Client: grpc/third_party/abseil-cpp/absl/time/libabsl_time_zone.a
client/Client: grpc/third_party/abseil-cpp/absl/types/libabsl_bad_optional_access.a
client/Client: grpc/third_party/abseil-cpp/absl/base/libabsl_raw_logging_internal.a
client/Client: grpc/third_party/abseil-cpp/absl/base/libabsl_log_severity.a
client/Client: grpc/third_party/boringssl-with-bazel/libssl.a
client/Client: grpc/third_party/boringssl-with-bazel/libcrypto.a
client/Client: client/CMakeFiles/Client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable Client"
	cd /home/tong/UA_BlackJack_Server/build/client && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
client/CMakeFiles/Client.dir/build: client/Client

.PHONY : client/CMakeFiles/Client.dir/build

client/CMakeFiles/Client.dir/clean:
	cd /home/tong/UA_BlackJack_Server/build/client && $(CMAKE_COMMAND) -P CMakeFiles/Client.dir/cmake_clean.cmake
.PHONY : client/CMakeFiles/Client.dir/clean

client/CMakeFiles/Client.dir/depend: ../Client/grpc/UA_BlackJack.pb.cc
client/CMakeFiles/Client.dir/depend: ../Client/grpc/UA_BlackJack.pb.h
client/CMakeFiles/Client.dir/depend: ../Client/grpc/UA_BlackJack.grpc.pb.cc
client/CMakeFiles/Client.dir/depend: ../Client/grpc/UA_BlackJack.grpc.pb.h
	cd /home/tong/UA_BlackJack_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tong/UA_BlackJack_Server /home/tong/UA_BlackJack_Server/Client /home/tong/UA_BlackJack_Server/build /home/tong/UA_BlackJack_Server/build/client /home/tong/UA_BlackJack_Server/build/client/CMakeFiles/Client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : client/CMakeFiles/Client.dir/depend

