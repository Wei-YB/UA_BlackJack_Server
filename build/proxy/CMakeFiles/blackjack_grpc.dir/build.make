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
include proxy/CMakeFiles/blackjack_grpc.dir/depend.make

# Include the progress variables for this target.
include proxy/CMakeFiles/blackjack_grpc.dir/progress.make

# Include the compile flags for this target's objects.
include proxy/CMakeFiles/blackjack_grpc.dir/flags.make

../ProxyServer/include/grpc/UA_BlackJack.pb.cc: ../protos/UA_BlackJack.proto
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ../../ProxyServer/include/grpc/UA_BlackJack.pb.cc, ../../ProxyServer/include/grpc/UA_BlackJack.pb.h, ../../ProxyServer/include/grpc/UA_BlackJack.grpc.pb.cc, ../../ProxyServer/include/grpc/UA_BlackJack.grpc.pb.h"
	cd /home/tong/UA_BlackJack_Server/build/proxy && ../grpc/third_party/protobuf/protoc-3.15.8.0 --grpc_out /home/tong/UA_BlackJack_Server/ProxyServer/include/grpc --cpp_out /home/tong/UA_BlackJack_Server/ProxyServer/include/grpc -I /home/tong/UA_BlackJack_Server/protos --plugin=protoc-gen-grpc="/home/tong/UA_BlackJack_Server/build/grpc/grpc_cpp_plugin" /home/tong/UA_BlackJack_Server/protos/UA_BlackJack.proto

../ProxyServer/include/grpc/UA_BlackJack.pb.h: ../ProxyServer/include/grpc/UA_BlackJack.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate ../ProxyServer/include/grpc/UA_BlackJack.pb.h

../ProxyServer/include/grpc/UA_BlackJack.grpc.pb.cc: ../ProxyServer/include/grpc/UA_BlackJack.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate ../ProxyServer/include/grpc/UA_BlackJack.grpc.pb.cc

../ProxyServer/include/grpc/UA_BlackJack.grpc.pb.h: ../ProxyServer/include/grpc/UA_BlackJack.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate ../ProxyServer/include/grpc/UA_BlackJack.grpc.pb.h

proxy/CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.pb.cc.o: proxy/CMakeFiles/blackjack_grpc.dir/flags.make
proxy/CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.pb.cc.o: ../ProxyServer/include/grpc/UA_BlackJack.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object proxy/CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.pb.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.pb.cc.o -c /home/tong/UA_BlackJack_Server/ProxyServer/include/grpc/UA_BlackJack.pb.cc

proxy/CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.pb.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/ProxyServer/include/grpc/UA_BlackJack.pb.cc > CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.pb.cc.i

proxy/CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.pb.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/ProxyServer/include/grpc/UA_BlackJack.pb.cc -o CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.pb.cc.s

proxy/CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.grpc.pb.cc.o: proxy/CMakeFiles/blackjack_grpc.dir/flags.make
proxy/CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.grpc.pb.cc.o: ../ProxyServer/include/grpc/UA_BlackJack.grpc.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object proxy/CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.grpc.pb.cc.o"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.grpc.pb.cc.o -c /home/tong/UA_BlackJack_Server/ProxyServer/include/grpc/UA_BlackJack.grpc.pb.cc

proxy/CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.grpc.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.grpc.pb.cc.i"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/ProxyServer/include/grpc/UA_BlackJack.grpc.pb.cc > CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.grpc.pb.cc.i

proxy/CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.grpc.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.grpc.pb.cc.s"
	cd /home/tong/UA_BlackJack_Server/build/proxy && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/ProxyServer/include/grpc/UA_BlackJack.grpc.pb.cc -o CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.grpc.pb.cc.s

# Object files for target blackjack_grpc
blackjack_grpc_OBJECTS = \
"CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.pb.cc.o" \
"CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.grpc.pb.cc.o"

# External object files for target blackjack_grpc
blackjack_grpc_EXTERNAL_OBJECTS =

../ProxyServer/lib/libblackjack_grpc.a: proxy/CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.pb.cc.o
../ProxyServer/lib/libblackjack_grpc.a: proxy/CMakeFiles/blackjack_grpc.dir/include/grpc/UA_BlackJack.grpc.pb.cc.o
../ProxyServer/lib/libblackjack_grpc.a: proxy/CMakeFiles/blackjack_grpc.dir/build.make
../ProxyServer/lib/libblackjack_grpc.a: proxy/CMakeFiles/blackjack_grpc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tong/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library ../../ProxyServer/lib/libblackjack_grpc.a"
	cd /home/tong/UA_BlackJack_Server/build/proxy && $(CMAKE_COMMAND) -P CMakeFiles/blackjack_grpc.dir/cmake_clean_target.cmake
	cd /home/tong/UA_BlackJack_Server/build/proxy && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/blackjack_grpc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
proxy/CMakeFiles/blackjack_grpc.dir/build: ../ProxyServer/lib/libblackjack_grpc.a

.PHONY : proxy/CMakeFiles/blackjack_grpc.dir/build

proxy/CMakeFiles/blackjack_grpc.dir/clean:
	cd /home/tong/UA_BlackJack_Server/build/proxy && $(CMAKE_COMMAND) -P CMakeFiles/blackjack_grpc.dir/cmake_clean.cmake
.PHONY : proxy/CMakeFiles/blackjack_grpc.dir/clean

proxy/CMakeFiles/blackjack_grpc.dir/depend: ../ProxyServer/include/grpc/UA_BlackJack.pb.cc
proxy/CMakeFiles/blackjack_grpc.dir/depend: ../ProxyServer/include/grpc/UA_BlackJack.pb.h
proxy/CMakeFiles/blackjack_grpc.dir/depend: ../ProxyServer/include/grpc/UA_BlackJack.grpc.pb.cc
proxy/CMakeFiles/blackjack_grpc.dir/depend: ../ProxyServer/include/grpc/UA_BlackJack.grpc.pb.h
	cd /home/tong/UA_BlackJack_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tong/UA_BlackJack_Server /home/tong/UA_BlackJack_Server/ProxyServer /home/tong/UA_BlackJack_Server/build /home/tong/UA_BlackJack_Server/build/proxy /home/tong/UA_BlackJack_Server/build/proxy/CMakeFiles/blackjack_grpc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : proxy/CMakeFiles/blackjack_grpc.dir/depend

