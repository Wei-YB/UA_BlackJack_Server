# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ricki-laptop/UA_BackJackServer/proxy/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ricki-laptop/UA_BackJackServer/proxy/test/build

# Include any dependencies generated for this target.
include CMakeFiles/simpleBackEnd.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/simpleBackEnd.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/simpleBackEnd.dir/flags.make

CMakeFiles/simpleBackEnd.dir/simpleBackEnd.cpp.o: CMakeFiles/simpleBackEnd.dir/flags.make
CMakeFiles/simpleBackEnd.dir/simpleBackEnd.cpp.o: ../simpleBackEnd.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ricki-laptop/UA_BackJackServer/proxy/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/simpleBackEnd.dir/simpleBackEnd.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simpleBackEnd.dir/simpleBackEnd.cpp.o -c /home/ricki-laptop/UA_BackJackServer/proxy/test/simpleBackEnd.cpp

CMakeFiles/simpleBackEnd.dir/simpleBackEnd.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simpleBackEnd.dir/simpleBackEnd.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ricki-laptop/UA_BackJackServer/proxy/test/simpleBackEnd.cpp > CMakeFiles/simpleBackEnd.dir/simpleBackEnd.cpp.i

CMakeFiles/simpleBackEnd.dir/simpleBackEnd.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simpleBackEnd.dir/simpleBackEnd.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ricki-laptop/UA_BackJackServer/proxy/test/simpleBackEnd.cpp -o CMakeFiles/simpleBackEnd.dir/simpleBackEnd.cpp.s

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc.o: CMakeFiles/simpleBackEnd.dir/flags.make
CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc.o: /home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ricki-laptop/UA_BackJackServer/proxy/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc.o -c /home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc > CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc.i

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc -o CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc.s

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc.o: CMakeFiles/simpleBackEnd.dir/flags.make
CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc.o: /home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ricki-laptop/UA_BackJackServer/proxy/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc.o -c /home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc > CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc.i

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc -o CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc.s

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc.o: CMakeFiles/simpleBackEnd.dir/flags.make
CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc.o: /home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ricki-laptop/UA_BackJackServer/proxy/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc.o -c /home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc > CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc.i

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc -o CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc.s

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc.o: CMakeFiles/simpleBackEnd.dir/flags.make
CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc.o: /home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ricki-laptop/UA_BackJackServer/proxy/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc.o -c /home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc > CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc.i

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc -o CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc.s

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc.o: CMakeFiles/simpleBackEnd.dir/flags.make
CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc.o: /home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ricki-laptop/UA_BackJackServer/proxy/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc.o -c /home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc > CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc.i

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc -o CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc.s

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc.o: CMakeFiles/simpleBackEnd.dir/flags.make
CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc.o: /home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ricki-laptop/UA_BackJackServer/proxy/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc.o -c /home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc > CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc.i

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc -o CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc.s

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc.o: CMakeFiles/simpleBackEnd.dir/flags.make
CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc.o: /home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ricki-laptop/UA_BackJackServer/proxy/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc.o -c /home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc > CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc.i

CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc -o CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc.s

# Object files for target simpleBackEnd
simpleBackEnd_OBJECTS = \
"CMakeFiles/simpleBackEnd.dir/simpleBackEnd.cpp.o" \
"CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc.o" \
"CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc.o" \
"CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc.o" \
"CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc.o" \
"CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc.o" \
"CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc.o" \
"CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc.o"

# External object files for target simpleBackEnd
simpleBackEnd_EXTERNAL_OBJECTS =

simpleBackEnd: CMakeFiles/simpleBackEnd.dir/simpleBackEnd.cpp.o
simpleBackEnd: CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/CircularBuffer.cc.o
simpleBackEnd: CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/Client.cc.o
simpleBackEnd: CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ClientProxyProtocol.cc.o
simpleBackEnd: CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/EventLoop.cc.o
simpleBackEnd: CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/ProxyServer.cc.o
simpleBackEnd: CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpConnection.cc.o
simpleBackEnd: CMakeFiles/simpleBackEnd.dir/home/ricki-laptop/UA_BackJackServer/proxy/src/TcpServer.cc.o
simpleBackEnd: CMakeFiles/simpleBackEnd.dir/build.make
simpleBackEnd: /home/ricki-laptop/UA_BackJackServer/proxy/lib/libblackjack_grpc.a
simpleBackEnd: /lib/libgrpc++_reflection.a
simpleBackEnd: /lib/libgrpc++.a
simpleBackEnd: /usr/local/lib/libprotobuf.so
simpleBackEnd: /lib/libgrpc.a
simpleBackEnd: /lib/libz.a
simpleBackEnd: /lib/libcares.a
simpleBackEnd: /lib/libaddress_sorting.a
simpleBackEnd: /lib/libre2.a
simpleBackEnd: /lib/libabsl_hash.a
simpleBackEnd: /lib/libabsl_city.a
simpleBackEnd: /lib/libabsl_wyhash.a
simpleBackEnd: /lib/libabsl_raw_hash_set.a
simpleBackEnd: /lib/libabsl_hashtablez_sampler.a
simpleBackEnd: /lib/libabsl_exponential_biased.a
simpleBackEnd: /lib/libabsl_statusor.a
simpleBackEnd: /lib/libabsl_bad_variant_access.a
simpleBackEnd: /lib/libgpr.a
simpleBackEnd: /lib/libupb.a
simpleBackEnd: /lib/libabsl_status.a
simpleBackEnd: /lib/libabsl_cord.a
simpleBackEnd: /lib/libabsl_str_format_internal.a
simpleBackEnd: /lib/libabsl_synchronization.a
simpleBackEnd: /lib/libabsl_stacktrace.a
simpleBackEnd: /lib/libabsl_symbolize.a
simpleBackEnd: /lib/libabsl_debugging_internal.a
simpleBackEnd: /lib/libabsl_demangle_internal.a
simpleBackEnd: /lib/libabsl_graphcycles_internal.a
simpleBackEnd: /lib/libabsl_malloc_internal.a
simpleBackEnd: /lib/libabsl_time.a
simpleBackEnd: /lib/libabsl_strings.a
simpleBackEnd: /lib/libabsl_throw_delegate.a
simpleBackEnd: /lib/libabsl_strings_internal.a
simpleBackEnd: /lib/libabsl_base.a
simpleBackEnd: /lib/libabsl_spinlock_wait.a
simpleBackEnd: /lib/libabsl_int128.a
simpleBackEnd: /lib/libabsl_civil_time.a
simpleBackEnd: /lib/libabsl_time_zone.a
simpleBackEnd: /lib/libabsl_bad_optional_access.a
simpleBackEnd: /lib/libabsl_raw_logging_internal.a
simpleBackEnd: /lib/libabsl_log_severity.a
simpleBackEnd: /lib/libssl.a
simpleBackEnd: /lib/libcrypto.a
simpleBackEnd: CMakeFiles/simpleBackEnd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ricki-laptop/UA_BackJackServer/proxy/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable simpleBackEnd"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simpleBackEnd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/simpleBackEnd.dir/build: simpleBackEnd

.PHONY : CMakeFiles/simpleBackEnd.dir/build

CMakeFiles/simpleBackEnd.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/simpleBackEnd.dir/cmake_clean.cmake
.PHONY : CMakeFiles/simpleBackEnd.dir/clean

CMakeFiles/simpleBackEnd.dir/depend:
	cd /home/ricki-laptop/UA_BackJackServer/proxy/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ricki-laptop/UA_BackJackServer/proxy/test /home/ricki-laptop/UA_BackJackServer/proxy/test /home/ricki-laptop/UA_BackJackServer/proxy/test/build /home/ricki-laptop/UA_BackJackServer/proxy/test/build /home/ricki-laptop/UA_BackJackServer/proxy/test/build/CMakeFiles/simpleBackEnd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/simpleBackEnd.dir/depend

