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
CMAKE_SOURCE_DIR = /home/tong/UA_BlackJack_Server/gRPC_demo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tong/UA_BlackJack_Server/gRPC_demo/cmake/build

# Include any dependencies generated for this target.
include CMakeFiles/client-asyn.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client-asyn.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client-asyn.dir/flags.make

CMakeFiles/client-asyn.dir/client-asyn.cc.o: CMakeFiles/client-asyn.dir/flags.make
CMakeFiles/client-asyn.dir/client-asyn.cc.o: ../../client-asyn.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/gRPC_demo/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client-asyn.dir/client-asyn.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client-asyn.dir/client-asyn.cc.o -c /home/tong/UA_BlackJack_Server/gRPC_demo/client-asyn.cc

CMakeFiles/client-asyn.dir/client-asyn.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client-asyn.dir/client-asyn.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/gRPC_demo/client-asyn.cc > CMakeFiles/client-asyn.dir/client-asyn.cc.i

CMakeFiles/client-asyn.dir/client-asyn.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client-asyn.dir/client-asyn.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/gRPC_demo/client-asyn.cc -o CMakeFiles/client-asyn.dir/client-asyn.cc.s

# Object files for target client-asyn
client__asyn_OBJECTS = \
"CMakeFiles/client-asyn.dir/client-asyn.cc.o"

# External object files for target client-asyn
client__asyn_EXTERNAL_OBJECTS =

client-asyn: CMakeFiles/client-asyn.dir/client-asyn.cc.o
client-asyn: CMakeFiles/client-asyn.dir/build.make
client-asyn: libhw_grpc_proto.a
client-asyn: /root/.local/lib/libgrpc++_reflection.a
client-asyn: /root/.local/lib/libgrpc++.a
client-asyn: /root/.local/lib64/libprotobuf.a
client-asyn: /root/.local/lib/libgrpc.a
client-asyn: /root/.local/lib/libz.a
client-asyn: /root/.local/lib/libcares.a
client-asyn: /root/.local/lib/libaddress_sorting.a
client-asyn: /root/.local/lib/libre2.a
client-asyn: /root/.local/lib/libabsl_hash.a
client-asyn: /root/.local/lib/libabsl_city.a
client-asyn: /root/.local/lib/libabsl_wyhash.a
client-asyn: /root/.local/lib/libabsl_raw_hash_set.a
client-asyn: /root/.local/lib/libabsl_hashtablez_sampler.a
client-asyn: /root/.local/lib/libabsl_exponential_biased.a
client-asyn: /root/.local/lib/libabsl_statusor.a
client-asyn: /root/.local/lib/libabsl_bad_variant_access.a
client-asyn: /root/.local/lib/libgpr.a
client-asyn: /root/.local/lib/libupb.a
client-asyn: /root/.local/lib/libabsl_status.a
client-asyn: /root/.local/lib/libabsl_cord.a
client-asyn: /root/.local/lib/libabsl_str_format_internal.a
client-asyn: /root/.local/lib/libabsl_synchronization.a
client-asyn: /root/.local/lib/libabsl_stacktrace.a
client-asyn: /root/.local/lib/libabsl_symbolize.a
client-asyn: /root/.local/lib/libabsl_debugging_internal.a
client-asyn: /root/.local/lib/libabsl_demangle_internal.a
client-asyn: /root/.local/lib/libabsl_graphcycles_internal.a
client-asyn: /root/.local/lib/libabsl_malloc_internal.a
client-asyn: /root/.local/lib/libabsl_time.a
client-asyn: /root/.local/lib/libabsl_strings.a
client-asyn: /root/.local/lib/libabsl_throw_delegate.a
client-asyn: /root/.local/lib/libabsl_strings_internal.a
client-asyn: /root/.local/lib/libabsl_base.a
client-asyn: /root/.local/lib/libabsl_spinlock_wait.a
client-asyn: /root/.local/lib/libabsl_int128.a
client-asyn: /root/.local/lib/libabsl_civil_time.a
client-asyn: /root/.local/lib/libabsl_time_zone.a
client-asyn: /root/.local/lib/libabsl_bad_optional_access.a
client-asyn: /root/.local/lib/libabsl_raw_logging_internal.a
client-asyn: /root/.local/lib/libabsl_log_severity.a
client-asyn: /root/.local/lib/libssl.a
client-asyn: /root/.local/lib/libcrypto.a
client-asyn: CMakeFiles/client-asyn.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tong/UA_BlackJack_Server/gRPC_demo/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable client-asyn"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client-asyn.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client-asyn.dir/build: client-asyn

.PHONY : CMakeFiles/client-asyn.dir/build

CMakeFiles/client-asyn.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client-asyn.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client-asyn.dir/clean

CMakeFiles/client-asyn.dir/depend:
	cd /home/tong/UA_BlackJack_Server/gRPC_demo/cmake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tong/UA_BlackJack_Server/gRPC_demo /home/tong/UA_BlackJack_Server/gRPC_demo /home/tong/UA_BlackJack_Server/gRPC_demo/cmake/build /home/tong/UA_BlackJack_Server/gRPC_demo/cmake/build /home/tong/UA_BlackJack_Server/gRPC_demo/cmake/build/CMakeFiles/client-asyn.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client-asyn.dir/depend

