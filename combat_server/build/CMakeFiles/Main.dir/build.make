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
CMAKE_SOURCE_DIR = /home/tong/UA_BlackJack_Server/combat_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tong/UA_BlackJack_Server/combat_server/build

# Include any dependencies generated for this target.
include CMakeFiles/Main.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Main.dir/flags.make

CMakeFiles/Main.dir/main.cc.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/main.cc.o: ../main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Main.dir/main.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Main.dir/main.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/main.cc

CMakeFiles/Main.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/main.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/main.cc > CMakeFiles/Main.dir/main.cc.i

CMakeFiles/Main.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/main.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/main.cc -o CMakeFiles/Main.dir/main.cc.s

CMakeFiles/Main.dir/src/ClientForDatebase.cc.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/src/ClientForDatebase.cc.o: ../src/ClientForDatebase.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Main.dir/src/ClientForDatebase.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Main.dir/src/ClientForDatebase.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/ClientForDatebase.cc

CMakeFiles/Main.dir/src/ClientForDatebase.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/src/ClientForDatebase.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/ClientForDatebase.cc > CMakeFiles/Main.dir/src/ClientForDatebase.cc.i

CMakeFiles/Main.dir/src/ClientForDatebase.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/src/ClientForDatebase.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/ClientForDatebase.cc -o CMakeFiles/Main.dir/src/ClientForDatebase.cc.s

CMakeFiles/Main.dir/src/ClientForLobby.cc.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/src/ClientForLobby.cc.o: ../src/ClientForLobby.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Main.dir/src/ClientForLobby.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Main.dir/src/ClientForLobby.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/ClientForLobby.cc

CMakeFiles/Main.dir/src/ClientForLobby.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/src/ClientForLobby.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/ClientForLobby.cc > CMakeFiles/Main.dir/src/ClientForLobby.cc.i

CMakeFiles/Main.dir/src/ClientForLobby.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/src/ClientForLobby.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/ClientForLobby.cc -o CMakeFiles/Main.dir/src/ClientForLobby.cc.s

CMakeFiles/Main.dir/src/ClientForTestUser.cc.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/src/ClientForTestUser.cc.o: ../src/ClientForTestUser.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Main.dir/src/ClientForTestUser.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Main.dir/src/ClientForTestUser.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/ClientForTestUser.cc

CMakeFiles/Main.dir/src/ClientForTestUser.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/src/ClientForTestUser.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/ClientForTestUser.cc > CMakeFiles/Main.dir/src/ClientForTestUser.cc.i

CMakeFiles/Main.dir/src/ClientForTestUser.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/src/ClientForTestUser.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/ClientForTestUser.cc -o CMakeFiles/Main.dir/src/ClientForTestUser.cc.s

CMakeFiles/Main.dir/src/GameProcess.cc.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/src/GameProcess.cc.o: ../src/GameProcess.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Main.dir/src/GameProcess.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Main.dir/src/GameProcess.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/GameProcess.cc

CMakeFiles/Main.dir/src/GameProcess.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/src/GameProcess.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/GameProcess.cc > CMakeFiles/Main.dir/src/GameProcess.cc.i

CMakeFiles/Main.dir/src/GameProcess.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/src/GameProcess.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/GameProcess.cc -o CMakeFiles/Main.dir/src/GameProcess.cc.s

CMakeFiles/Main.dir/src/MycondintonCo.cc.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/src/MycondintonCo.cc.o: ../src/MycondintonCo.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Main.dir/src/MycondintonCo.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Main.dir/src/MycondintonCo.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/MycondintonCo.cc

CMakeFiles/Main.dir/src/MycondintonCo.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/src/MycondintonCo.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/MycondintonCo.cc > CMakeFiles/Main.dir/src/MycondintonCo.cc.i

CMakeFiles/Main.dir/src/MycondintonCo.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/src/MycondintonCo.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/MycondintonCo.cc -o CMakeFiles/Main.dir/src/MycondintonCo.cc.s

CMakeFiles/Main.dir/src/Player.cc.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/src/Player.cc.o: ../src/Player.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Main.dir/src/Player.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Main.dir/src/Player.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/Player.cc

CMakeFiles/Main.dir/src/Player.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/src/Player.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/Player.cc > CMakeFiles/Main.dir/src/Player.cc.i

CMakeFiles/Main.dir/src/Player.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/src/Player.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/Player.cc -o CMakeFiles/Main.dir/src/Player.cc.s

CMakeFiles/Main.dir/src/Poker.cc.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/src/Poker.cc.o: ../src/Poker.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Main.dir/src/Poker.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Main.dir/src/Poker.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/Poker.cc

CMakeFiles/Main.dir/src/Poker.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/src/Poker.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/Poker.cc > CMakeFiles/Main.dir/src/Poker.cc.i

CMakeFiles/Main.dir/src/Poker.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/src/Poker.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/Poker.cc -o CMakeFiles/Main.dir/src/Poker.cc.s

CMakeFiles/Main.dir/src/Room.cc.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/src/Room.cc.o: ../src/Room.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Main.dir/src/Room.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Main.dir/src/Room.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/Room.cc

CMakeFiles/Main.dir/src/Room.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/src/Room.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/Room.cc > CMakeFiles/Main.dir/src/Room.cc.i

CMakeFiles/Main.dir/src/Room.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/src/Room.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/Room.cc -o CMakeFiles/Main.dir/src/Room.cc.s

CMakeFiles/Main.dir/src/ShuffledPokers.cc.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/src/ShuffledPokers.cc.o: ../src/ShuffledPokers.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/Main.dir/src/ShuffledPokers.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Main.dir/src/ShuffledPokers.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/ShuffledPokers.cc

CMakeFiles/Main.dir/src/ShuffledPokers.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/src/ShuffledPokers.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/ShuffledPokers.cc > CMakeFiles/Main.dir/src/ShuffledPokers.cc.i

CMakeFiles/Main.dir/src/ShuffledPokers.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/src/ShuffledPokers.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/ShuffledPokers.cc -o CMakeFiles/Main.dir/src/ShuffledPokers.cc.s

CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.o: ../3rd/grpc/grpc_cpp/demo.grpc.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/3rd/grpc/grpc_cpp/demo.grpc.pb.cc

CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/3rd/grpc/grpc_cpp/demo.grpc.pb.cc > CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.i

CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/3rd/grpc/grpc_cpp/demo.grpc.pb.cc -o CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.s

CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.pb.cc.o: CMakeFiles/Main.dir/flags.make
CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.pb.cc.o: ../3rd/grpc/grpc_cpp/demo.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.pb.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.pb.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/3rd/grpc/grpc_cpp/demo.pb.cc

CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.pb.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/3rd/grpc/grpc_cpp/demo.pb.cc > CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.pb.cc.i

CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.pb.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/3rd/grpc/grpc_cpp/demo.pb.cc -o CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.pb.cc.s

# Object files for target Main
Main_OBJECTS = \
"CMakeFiles/Main.dir/main.cc.o" \
"CMakeFiles/Main.dir/src/ClientForDatebase.cc.o" \
"CMakeFiles/Main.dir/src/ClientForLobby.cc.o" \
"CMakeFiles/Main.dir/src/ClientForTestUser.cc.o" \
"CMakeFiles/Main.dir/src/GameProcess.cc.o" \
"CMakeFiles/Main.dir/src/MycondintonCo.cc.o" \
"CMakeFiles/Main.dir/src/Player.cc.o" \
"CMakeFiles/Main.dir/src/Poker.cc.o" \
"CMakeFiles/Main.dir/src/Room.cc.o" \
"CMakeFiles/Main.dir/src/ShuffledPokers.cc.o" \
"CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.o" \
"CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.pb.cc.o"

# External object files for target Main
Main_EXTERNAL_OBJECTS =

Main: CMakeFiles/Main.dir/main.cc.o
Main: CMakeFiles/Main.dir/src/ClientForDatebase.cc.o
Main: CMakeFiles/Main.dir/src/ClientForLobby.cc.o
Main: CMakeFiles/Main.dir/src/ClientForTestUser.cc.o
Main: CMakeFiles/Main.dir/src/GameProcess.cc.o
Main: CMakeFiles/Main.dir/src/MycondintonCo.cc.o
Main: CMakeFiles/Main.dir/src/Player.cc.o
Main: CMakeFiles/Main.dir/src/Poker.cc.o
Main: CMakeFiles/Main.dir/src/Room.cc.o
Main: CMakeFiles/Main.dir/src/ShuffledPokers.cc.o
Main: CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.o
Main: CMakeFiles/Main.dir/3rd/grpc/grpc_cpp/demo.pb.cc.o
Main: CMakeFiles/Main.dir/build.make
Main: /root/.local/lib/libgrpc++_reflection.a
Main: /root/.local/lib/libgrpc++.a
Main: /root/.local/lib64/libprotobuf.a
Main: /root/.local/lib/libgrpc.a
Main: /root/.local/lib/libz.a
Main: /root/.local/lib/libcares.a
Main: /root/.local/lib/libaddress_sorting.a
Main: /root/.local/lib/libre2.a
Main: /root/.local/lib/libabsl_hash.a
Main: /root/.local/lib/libabsl_city.a
Main: /root/.local/lib/libabsl_wyhash.a
Main: /root/.local/lib/libabsl_raw_hash_set.a
Main: /root/.local/lib/libabsl_hashtablez_sampler.a
Main: /root/.local/lib/libabsl_exponential_biased.a
Main: /root/.local/lib/libabsl_statusor.a
Main: /root/.local/lib/libabsl_bad_variant_access.a
Main: /root/.local/lib/libgpr.a
Main: /root/.local/lib/libupb.a
Main: /root/.local/lib/libabsl_status.a
Main: /root/.local/lib/libabsl_cord.a
Main: /root/.local/lib/libabsl_str_format_internal.a
Main: /root/.local/lib/libabsl_synchronization.a
Main: /root/.local/lib/libabsl_stacktrace.a
Main: /root/.local/lib/libabsl_symbolize.a
Main: /root/.local/lib/libabsl_debugging_internal.a
Main: /root/.local/lib/libabsl_demangle_internal.a
Main: /root/.local/lib/libabsl_graphcycles_internal.a
Main: /root/.local/lib/libabsl_malloc_internal.a
Main: /root/.local/lib/libabsl_time.a
Main: /root/.local/lib/libabsl_strings.a
Main: /root/.local/lib/libabsl_throw_delegate.a
Main: /root/.local/lib/libabsl_strings_internal.a
Main: /root/.local/lib/libabsl_base.a
Main: /root/.local/lib/libabsl_spinlock_wait.a
Main: /root/.local/lib/libabsl_int128.a
Main: /root/.local/lib/libabsl_civil_time.a
Main: /root/.local/lib/libabsl_time_zone.a
Main: /root/.local/lib/libabsl_bad_optional_access.a
Main: /root/.local/lib/libabsl_raw_logging_internal.a
Main: /root/.local/lib/libabsl_log_severity.a
Main: /root/.local/lib/libssl.a
Main: /root/.local/lib/libcrypto.a
Main: CMakeFiles/Main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX executable Main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Main.dir/build: Main

.PHONY : CMakeFiles/Main.dir/build

CMakeFiles/Main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Main.dir/clean

CMakeFiles/Main.dir/depend:
	cd /home/tong/UA_BlackJack_Server/combat_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tong/UA_BlackJack_Server/combat_server /home/tong/UA_BlackJack_Server/combat_server /home/tong/UA_BlackJack_Server/combat_server/build /home/tong/UA_BlackJack_Server/combat_server/build /home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles/Main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Main.dir/depend

