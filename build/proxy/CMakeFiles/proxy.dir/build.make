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
include proxy/CMakeFiles/proxy.dir/depend.make

# Include the progress variables for this target.
include proxy/CMakeFiles/proxy.dir/progress.make

# Include the compile flags for this target's objects.
include proxy/CMakeFiles/proxy.dir/flags.make

proxy/CMakeFiles/proxy.dir/src/main.cpp.o: proxy/CMakeFiles/proxy.dir/flags.make
proxy/CMakeFiles/proxy.dir/src/main.cpp.o: ../ProxyServer/src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object proxy/CMakeFiles/proxy.dir/src/main.cpp.o"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/proxy.dir/src/main.cpp.o -c /home/tong/UA_BlackJack_Server/ProxyServer/src/main.cpp

proxy/CMakeFiles/proxy.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/proxy.dir/src/main.cpp.i"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/ProxyServer/src/main.cpp > CMakeFiles/proxy.dir/src/main.cpp.i

proxy/CMakeFiles/proxy.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/proxy.dir/src/main.cpp.s"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/ProxyServer/src/main.cpp -o CMakeFiles/proxy.dir/src/main.cpp.s

proxy/CMakeFiles/proxy.dir/src/CircularBuffer.cc.o: proxy/CMakeFiles/proxy.dir/flags.make
proxy/CMakeFiles/proxy.dir/src/CircularBuffer.cc.o: ../ProxyServer/src/CircularBuffer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object proxy/CMakeFiles/proxy.dir/src/CircularBuffer.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/proxy.dir/src/CircularBuffer.cc.o -c /home/tong/UA_BlackJack_Server/ProxyServer/src/CircularBuffer.cc

proxy/CMakeFiles/proxy.dir/src/CircularBuffer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/proxy.dir/src/CircularBuffer.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/ProxyServer/src/CircularBuffer.cc > CMakeFiles/proxy.dir/src/CircularBuffer.cc.i

proxy/CMakeFiles/proxy.dir/src/CircularBuffer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/proxy.dir/src/CircularBuffer.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/ProxyServer/src/CircularBuffer.cc -o CMakeFiles/proxy.dir/src/CircularBuffer.cc.s

proxy/CMakeFiles/proxy.dir/src/Client.cc.o: proxy/CMakeFiles/proxy.dir/flags.make
proxy/CMakeFiles/proxy.dir/src/Client.cc.o: ../ProxyServer/src/Client.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object proxy/CMakeFiles/proxy.dir/src/Client.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/proxy.dir/src/Client.cc.o -c /home/tong/UA_BlackJack_Server/ProxyServer/src/Client.cc

proxy/CMakeFiles/proxy.dir/src/Client.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/proxy.dir/src/Client.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/ProxyServer/src/Client.cc > CMakeFiles/proxy.dir/src/Client.cc.i

proxy/CMakeFiles/proxy.dir/src/Client.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/proxy.dir/src/Client.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/ProxyServer/src/Client.cc -o CMakeFiles/proxy.dir/src/Client.cc.s

proxy/CMakeFiles/proxy.dir/src/ClientProxyProtocol.cc.o: proxy/CMakeFiles/proxy.dir/flags.make
proxy/CMakeFiles/proxy.dir/src/ClientProxyProtocol.cc.o: ../ProxyServer/src/ClientProxyProtocol.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object proxy/CMakeFiles/proxy.dir/src/ClientProxyProtocol.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/proxy.dir/src/ClientProxyProtocol.cc.o -c /home/tong/UA_BlackJack_Server/ProxyServer/src/ClientProxyProtocol.cc

proxy/CMakeFiles/proxy.dir/src/ClientProxyProtocol.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/proxy.dir/src/ClientProxyProtocol.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/ProxyServer/src/ClientProxyProtocol.cc > CMakeFiles/proxy.dir/src/ClientProxyProtocol.cc.i

proxy/CMakeFiles/proxy.dir/src/ClientProxyProtocol.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/proxy.dir/src/ClientProxyProtocol.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/ProxyServer/src/ClientProxyProtocol.cc -o CMakeFiles/proxy.dir/src/ClientProxyProtocol.cc.s

proxy/CMakeFiles/proxy.dir/src/EventLoop.cc.o: proxy/CMakeFiles/proxy.dir/flags.make
proxy/CMakeFiles/proxy.dir/src/EventLoop.cc.o: ../ProxyServer/src/EventLoop.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object proxy/CMakeFiles/proxy.dir/src/EventLoop.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/proxy.dir/src/EventLoop.cc.o -c /home/tong/UA_BlackJack_Server/ProxyServer/src/EventLoop.cc

proxy/CMakeFiles/proxy.dir/src/EventLoop.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/proxy.dir/src/EventLoop.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/ProxyServer/src/EventLoop.cc > CMakeFiles/proxy.dir/src/EventLoop.cc.i

proxy/CMakeFiles/proxy.dir/src/EventLoop.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/proxy.dir/src/EventLoop.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/ProxyServer/src/EventLoop.cc -o CMakeFiles/proxy.dir/src/EventLoop.cc.s

proxy/CMakeFiles/proxy.dir/src/ProxyServer.cc.o: proxy/CMakeFiles/proxy.dir/flags.make
proxy/CMakeFiles/proxy.dir/src/ProxyServer.cc.o: ../ProxyServer/src/ProxyServer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object proxy/CMakeFiles/proxy.dir/src/ProxyServer.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/proxy.dir/src/ProxyServer.cc.o -c /home/tong/UA_BlackJack_Server/ProxyServer/src/ProxyServer.cc

proxy/CMakeFiles/proxy.dir/src/ProxyServer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/proxy.dir/src/ProxyServer.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/ProxyServer/src/ProxyServer.cc > CMakeFiles/proxy.dir/src/ProxyServer.cc.i

proxy/CMakeFiles/proxy.dir/src/ProxyServer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/proxy.dir/src/ProxyServer.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/ProxyServer/src/ProxyServer.cc -o CMakeFiles/proxy.dir/src/ProxyServer.cc.s

proxy/CMakeFiles/proxy.dir/src/TcpConnection.cc.o: proxy/CMakeFiles/proxy.dir/flags.make
proxy/CMakeFiles/proxy.dir/src/TcpConnection.cc.o: ../ProxyServer/src/TcpConnection.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object proxy/CMakeFiles/proxy.dir/src/TcpConnection.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/proxy.dir/src/TcpConnection.cc.o -c /home/tong/UA_BlackJack_Server/ProxyServer/src/TcpConnection.cc

proxy/CMakeFiles/proxy.dir/src/TcpConnection.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/proxy.dir/src/TcpConnection.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/ProxyServer/src/TcpConnection.cc > CMakeFiles/proxy.dir/src/TcpConnection.cc.i

proxy/CMakeFiles/proxy.dir/src/TcpConnection.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/proxy.dir/src/TcpConnection.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/ProxyServer/src/TcpConnection.cc -o CMakeFiles/proxy.dir/src/TcpConnection.cc.s

proxy/CMakeFiles/proxy.dir/src/TcpServer.cc.o: proxy/CMakeFiles/proxy.dir/flags.make
proxy/CMakeFiles/proxy.dir/src/TcpServer.cc.o: ../ProxyServer/src/TcpServer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object proxy/CMakeFiles/proxy.dir/src/TcpServer.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/proxy.dir/src/TcpServer.cc.o -c /home/tong/UA_BlackJack_Server/ProxyServer/src/TcpServer.cc

proxy/CMakeFiles/proxy.dir/src/TcpServer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/proxy.dir/src/TcpServer.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/ProxyServer/src/TcpServer.cc > CMakeFiles/proxy.dir/src/TcpServer.cc.i

proxy/CMakeFiles/proxy.dir/src/TcpServer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/proxy.dir/src/TcpServer.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/ProxyServer/src/TcpServer.cc -o CMakeFiles/proxy.dir/src/TcpServer.cc.s

proxy/CMakeFiles/proxy.dir/src/log.cc.o: proxy/CMakeFiles/proxy.dir/flags.make
proxy/CMakeFiles/proxy.dir/src/log.cc.o: ../ProxyServer/src/log.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object proxy/CMakeFiles/proxy.dir/src/log.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/proxy.dir/src/log.cc.o -c /home/tong/UA_BlackJack_Server/ProxyServer/src/log.cc

proxy/CMakeFiles/proxy.dir/src/log.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/proxy.dir/src/log.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/ProxyServer/src/log.cc > CMakeFiles/proxy.dir/src/log.cc.i

proxy/CMakeFiles/proxy.dir/src/log.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/proxy.dir/src/log.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/ProxyServer/src/log.cc -o CMakeFiles/proxy.dir/src/log.cc.s

# Object files for target proxy
proxy_OBJECTS = \
"CMakeFiles/proxy.dir/src/main.cpp.o" \
"CMakeFiles/proxy.dir/src/CircularBuffer.cc.o" \
"CMakeFiles/proxy.dir/src/Client.cc.o" \
"CMakeFiles/proxy.dir/src/ClientProxyProtocol.cc.o" \
"CMakeFiles/proxy.dir/src/EventLoop.cc.o" \
"CMakeFiles/proxy.dir/src/ProxyServer.cc.o" \
"CMakeFiles/proxy.dir/src/TcpConnection.cc.o" \
"CMakeFiles/proxy.dir/src/TcpServer.cc.o" \
"CMakeFiles/proxy.dir/src/log.cc.o"

# External object files for target proxy
proxy_EXTERNAL_OBJECTS =

proxy/proxy: proxy/CMakeFiles/proxy.dir/src/main.cpp.o
proxy/proxy: proxy/CMakeFiles/proxy.dir/src/CircularBuffer.cc.o
proxy/proxy: proxy/CMakeFiles/proxy.dir/src/Client.cc.o
proxy/proxy: proxy/CMakeFiles/proxy.dir/src/ClientProxyProtocol.cc.o
proxy/proxy: proxy/CMakeFiles/proxy.dir/src/EventLoop.cc.o
proxy/proxy: proxy/CMakeFiles/proxy.dir/src/ProxyServer.cc.o
proxy/proxy: proxy/CMakeFiles/proxy.dir/src/TcpConnection.cc.o
proxy/proxy: proxy/CMakeFiles/proxy.dir/src/TcpServer.cc.o
proxy/proxy: proxy/CMakeFiles/proxy.dir/src/log.cc.o
proxy/proxy: proxy/CMakeFiles/proxy.dir/build.make
proxy/proxy: ../thirdparty/spdlog/libspdlog.a
proxy/proxy: ../ProxyServer/lib/libblackjack_grpc.a
proxy/proxy: grpc/libgrpc++_reflection.a
proxy/proxy: grpc/libgrpc++.a
proxy/proxy: grpc/third_party/protobuf/libprotobuf.a
proxy/proxy: grpc/libgrpc.a
proxy/proxy: grpc/third_party/zlib/libz.a
proxy/proxy: grpc/third_party/cares/cares/lib/libcares.a
proxy/proxy: grpc/libaddress_sorting.a
proxy/proxy: grpc/third_party/re2/libre2.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/hash/libabsl_hash.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/hash/libabsl_city.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/hash/libabsl_wyhash.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/container/libabsl_raw_hash_set.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/container/libabsl_hashtablez_sampler.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/base/libabsl_exponential_biased.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/status/libabsl_statusor.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/types/libabsl_bad_variant_access.a
proxy/proxy: grpc/libgpr.a
proxy/proxy: grpc/libupb.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/status/libabsl_status.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/strings/libabsl_cord.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/strings/libabsl_str_format_internal.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/synchronization/libabsl_synchronization.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/debugging/libabsl_stacktrace.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/debugging/libabsl_symbolize.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/debugging/libabsl_debugging_internal.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/debugging/libabsl_demangle_internal.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/synchronization/libabsl_graphcycles_internal.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/base/libabsl_malloc_internal.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/time/libabsl_time.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/strings/libabsl_strings.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/base/libabsl_throw_delegate.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/strings/libabsl_strings_internal.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/base/libabsl_base.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/base/libabsl_spinlock_wait.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/numeric/libabsl_int128.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/time/libabsl_civil_time.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/time/libabsl_time_zone.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/types/libabsl_bad_optional_access.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/base/libabsl_raw_logging_internal.a
proxy/proxy: grpc/third_party/abseil-cpp/absl/base/libabsl_log_severity.a
proxy/proxy: grpc/third_party/boringssl-with-bazel/libssl.a
proxy/proxy: grpc/third_party/boringssl-with-bazel/libcrypto.a
proxy/proxy: proxy/CMakeFiles/proxy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable proxy"
	cd /home/tong/UA_BlackJack_Server/build/proxy && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/proxy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
proxy/CMakeFiles/proxy.dir/build: proxy/proxy

.PHONY : proxy/CMakeFiles/proxy.dir/build

proxy/CMakeFiles/proxy.dir/clean:
	cd /home/tong/UA_BlackJack_Server/build/proxy && $(CMAKE_COMMAND) -P CMakeFiles/proxy.dir/cmake_clean.cmake
.PHONY : proxy/CMakeFiles/proxy.dir/clean

proxy/CMakeFiles/proxy.dir/depend:
	cd /home/tong/UA_BlackJack_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tong/UA_BlackJack_Server /home/tong/UA_BlackJack_Server/ProxyServer /home/tong/UA_BlackJack_Server/build /home/tong/UA_BlackJack_Server/build/proxy /home/tong/UA_BlackJack_Server/build/proxy/CMakeFiles/proxy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : proxy/CMakeFiles/proxy.dir/depend

