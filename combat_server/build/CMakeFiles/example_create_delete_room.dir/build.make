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
include CMakeFiles/example_create_delete_room.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/example_create_delete_room.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/example_create_delete_room.dir/flags.make

CMakeFiles/example_create_delete_room.dir/example/example_create_delete_room.cc.o: CMakeFiles/example_create_delete_room.dir/flags.make
CMakeFiles/example_create_delete_room.dir/example/example_create_delete_room.cc.o: ../example/example_create_delete_room.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/example_create_delete_room.dir/example/example_create_delete_room.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_create_delete_room.dir/example/example_create_delete_room.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/example/example_create_delete_room.cc

CMakeFiles/example_create_delete_room.dir/example/example_create_delete_room.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_create_delete_room.dir/example/example_create_delete_room.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/example/example_create_delete_room.cc > CMakeFiles/example_create_delete_room.dir/example/example_create_delete_room.cc.i

CMakeFiles/example_create_delete_room.dir/example/example_create_delete_room.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_create_delete_room.dir/example/example_create_delete_room.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/example/example_create_delete_room.cc -o CMakeFiles/example_create_delete_room.dir/example/example_create_delete_room.cc.s

CMakeFiles/example_create_delete_room.dir/src/ClientForDatebase.cc.o: CMakeFiles/example_create_delete_room.dir/flags.make
CMakeFiles/example_create_delete_room.dir/src/ClientForDatebase.cc.o: ../src/ClientForDatebase.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/example_create_delete_room.dir/src/ClientForDatebase.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_create_delete_room.dir/src/ClientForDatebase.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/ClientForDatebase.cc

CMakeFiles/example_create_delete_room.dir/src/ClientForDatebase.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_create_delete_room.dir/src/ClientForDatebase.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/ClientForDatebase.cc > CMakeFiles/example_create_delete_room.dir/src/ClientForDatebase.cc.i

CMakeFiles/example_create_delete_room.dir/src/ClientForDatebase.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_create_delete_room.dir/src/ClientForDatebase.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/ClientForDatebase.cc -o CMakeFiles/example_create_delete_room.dir/src/ClientForDatebase.cc.s

CMakeFiles/example_create_delete_room.dir/src/ClientForLobby.cc.o: CMakeFiles/example_create_delete_room.dir/flags.make
CMakeFiles/example_create_delete_room.dir/src/ClientForLobby.cc.o: ../src/ClientForLobby.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/example_create_delete_room.dir/src/ClientForLobby.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_create_delete_room.dir/src/ClientForLobby.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/ClientForLobby.cc

CMakeFiles/example_create_delete_room.dir/src/ClientForLobby.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_create_delete_room.dir/src/ClientForLobby.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/ClientForLobby.cc > CMakeFiles/example_create_delete_room.dir/src/ClientForLobby.cc.i

CMakeFiles/example_create_delete_room.dir/src/ClientForLobby.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_create_delete_room.dir/src/ClientForLobby.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/ClientForLobby.cc -o CMakeFiles/example_create_delete_room.dir/src/ClientForLobby.cc.s

CMakeFiles/example_create_delete_room.dir/src/ClientForTestUser.cc.o: CMakeFiles/example_create_delete_room.dir/flags.make
CMakeFiles/example_create_delete_room.dir/src/ClientForTestUser.cc.o: ../src/ClientForTestUser.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/example_create_delete_room.dir/src/ClientForTestUser.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_create_delete_room.dir/src/ClientForTestUser.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/ClientForTestUser.cc

CMakeFiles/example_create_delete_room.dir/src/ClientForTestUser.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_create_delete_room.dir/src/ClientForTestUser.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/ClientForTestUser.cc > CMakeFiles/example_create_delete_room.dir/src/ClientForTestUser.cc.i

CMakeFiles/example_create_delete_room.dir/src/ClientForTestUser.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_create_delete_room.dir/src/ClientForTestUser.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/ClientForTestUser.cc -o CMakeFiles/example_create_delete_room.dir/src/ClientForTestUser.cc.s

CMakeFiles/example_create_delete_room.dir/src/GameProcess.cc.o: CMakeFiles/example_create_delete_room.dir/flags.make
CMakeFiles/example_create_delete_room.dir/src/GameProcess.cc.o: ../src/GameProcess.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/example_create_delete_room.dir/src/GameProcess.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_create_delete_room.dir/src/GameProcess.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/GameProcess.cc

CMakeFiles/example_create_delete_room.dir/src/GameProcess.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_create_delete_room.dir/src/GameProcess.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/GameProcess.cc > CMakeFiles/example_create_delete_room.dir/src/GameProcess.cc.i

CMakeFiles/example_create_delete_room.dir/src/GameProcess.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_create_delete_room.dir/src/GameProcess.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/GameProcess.cc -o CMakeFiles/example_create_delete_room.dir/src/GameProcess.cc.s

CMakeFiles/example_create_delete_room.dir/src/MycondintonCo.cc.o: CMakeFiles/example_create_delete_room.dir/flags.make
CMakeFiles/example_create_delete_room.dir/src/MycondintonCo.cc.o: ../src/MycondintonCo.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/example_create_delete_room.dir/src/MycondintonCo.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_create_delete_room.dir/src/MycondintonCo.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/MycondintonCo.cc

CMakeFiles/example_create_delete_room.dir/src/MycondintonCo.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_create_delete_room.dir/src/MycondintonCo.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/MycondintonCo.cc > CMakeFiles/example_create_delete_room.dir/src/MycondintonCo.cc.i

CMakeFiles/example_create_delete_room.dir/src/MycondintonCo.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_create_delete_room.dir/src/MycondintonCo.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/MycondintonCo.cc -o CMakeFiles/example_create_delete_room.dir/src/MycondintonCo.cc.s

CMakeFiles/example_create_delete_room.dir/src/Player.cc.o: CMakeFiles/example_create_delete_room.dir/flags.make
CMakeFiles/example_create_delete_room.dir/src/Player.cc.o: ../src/Player.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/example_create_delete_room.dir/src/Player.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_create_delete_room.dir/src/Player.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/Player.cc

CMakeFiles/example_create_delete_room.dir/src/Player.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_create_delete_room.dir/src/Player.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/Player.cc > CMakeFiles/example_create_delete_room.dir/src/Player.cc.i

CMakeFiles/example_create_delete_room.dir/src/Player.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_create_delete_room.dir/src/Player.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/Player.cc -o CMakeFiles/example_create_delete_room.dir/src/Player.cc.s

CMakeFiles/example_create_delete_room.dir/src/Poker.cc.o: CMakeFiles/example_create_delete_room.dir/flags.make
CMakeFiles/example_create_delete_room.dir/src/Poker.cc.o: ../src/Poker.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/example_create_delete_room.dir/src/Poker.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_create_delete_room.dir/src/Poker.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/Poker.cc

CMakeFiles/example_create_delete_room.dir/src/Poker.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_create_delete_room.dir/src/Poker.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/Poker.cc > CMakeFiles/example_create_delete_room.dir/src/Poker.cc.i

CMakeFiles/example_create_delete_room.dir/src/Poker.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_create_delete_room.dir/src/Poker.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/Poker.cc -o CMakeFiles/example_create_delete_room.dir/src/Poker.cc.s

CMakeFiles/example_create_delete_room.dir/src/Room.cc.o: CMakeFiles/example_create_delete_room.dir/flags.make
CMakeFiles/example_create_delete_room.dir/src/Room.cc.o: ../src/Room.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/example_create_delete_room.dir/src/Room.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_create_delete_room.dir/src/Room.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/Room.cc

CMakeFiles/example_create_delete_room.dir/src/Room.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_create_delete_room.dir/src/Room.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/Room.cc > CMakeFiles/example_create_delete_room.dir/src/Room.cc.i

CMakeFiles/example_create_delete_room.dir/src/Room.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_create_delete_room.dir/src/Room.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/Room.cc -o CMakeFiles/example_create_delete_room.dir/src/Room.cc.s

CMakeFiles/example_create_delete_room.dir/src/ShuffledPokers.cc.o: CMakeFiles/example_create_delete_room.dir/flags.make
CMakeFiles/example_create_delete_room.dir/src/ShuffledPokers.cc.o: ../src/ShuffledPokers.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/example_create_delete_room.dir/src/ShuffledPokers.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_create_delete_room.dir/src/ShuffledPokers.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/ShuffledPokers.cc

CMakeFiles/example_create_delete_room.dir/src/ShuffledPokers.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_create_delete_room.dir/src/ShuffledPokers.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/ShuffledPokers.cc > CMakeFiles/example_create_delete_room.dir/src/ShuffledPokers.cc.i

CMakeFiles/example_create_delete_room.dir/src/ShuffledPokers.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_create_delete_room.dir/src/ShuffledPokers.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/ShuffledPokers.cc -o CMakeFiles/example_create_delete_room.dir/src/ShuffledPokers.cc.s

CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.o: CMakeFiles/example_create_delete_room.dir/flags.make
CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.o: ../3rd/grpc/grpc_cpp/demo.grpc.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/3rd/grpc/grpc_cpp/demo.grpc.pb.cc

CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/3rd/grpc/grpc_cpp/demo.grpc.pb.cc > CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.i

CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/3rd/grpc/grpc_cpp/demo.grpc.pb.cc -o CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.s

CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.pb.cc.o: CMakeFiles/example_create_delete_room.dir/flags.make
CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.pb.cc.o: ../3rd/grpc/grpc_cpp/demo.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.pb.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.pb.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/3rd/grpc/grpc_cpp/demo.pb.cc

CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.pb.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/3rd/grpc/grpc_cpp/demo.pb.cc > CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.pb.cc.i

CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.pb.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/3rd/grpc/grpc_cpp/demo.pb.cc -o CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.pb.cc.s

# Object files for target example_create_delete_room
example_create_delete_room_OBJECTS = \
"CMakeFiles/example_create_delete_room.dir/example/example_create_delete_room.cc.o" \
"CMakeFiles/example_create_delete_room.dir/src/ClientForDatebase.cc.o" \
"CMakeFiles/example_create_delete_room.dir/src/ClientForLobby.cc.o" \
"CMakeFiles/example_create_delete_room.dir/src/ClientForTestUser.cc.o" \
"CMakeFiles/example_create_delete_room.dir/src/GameProcess.cc.o" \
"CMakeFiles/example_create_delete_room.dir/src/MycondintonCo.cc.o" \
"CMakeFiles/example_create_delete_room.dir/src/Player.cc.o" \
"CMakeFiles/example_create_delete_room.dir/src/Poker.cc.o" \
"CMakeFiles/example_create_delete_room.dir/src/Room.cc.o" \
"CMakeFiles/example_create_delete_room.dir/src/ShuffledPokers.cc.o" \
"CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.o" \
"CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.pb.cc.o"

# External object files for target example_create_delete_room
example_create_delete_room_EXTERNAL_OBJECTS =

example_create_delete_room: CMakeFiles/example_create_delete_room.dir/example/example_create_delete_room.cc.o
example_create_delete_room: CMakeFiles/example_create_delete_room.dir/src/ClientForDatebase.cc.o
example_create_delete_room: CMakeFiles/example_create_delete_room.dir/src/ClientForLobby.cc.o
example_create_delete_room: CMakeFiles/example_create_delete_room.dir/src/ClientForTestUser.cc.o
example_create_delete_room: CMakeFiles/example_create_delete_room.dir/src/GameProcess.cc.o
example_create_delete_room: CMakeFiles/example_create_delete_room.dir/src/MycondintonCo.cc.o
example_create_delete_room: CMakeFiles/example_create_delete_room.dir/src/Player.cc.o
example_create_delete_room: CMakeFiles/example_create_delete_room.dir/src/Poker.cc.o
example_create_delete_room: CMakeFiles/example_create_delete_room.dir/src/Room.cc.o
example_create_delete_room: CMakeFiles/example_create_delete_room.dir/src/ShuffledPokers.cc.o
example_create_delete_room: CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.grpc.pb.cc.o
example_create_delete_room: CMakeFiles/example_create_delete_room.dir/3rd/grpc/grpc_cpp/demo.pb.cc.o
example_create_delete_room: CMakeFiles/example_create_delete_room.dir/build.make
example_create_delete_room: /root/.local/lib/libgrpc++_reflection.a
example_create_delete_room: /root/.local/lib/libgrpc++.a
example_create_delete_room: /root/.local/lib64/libprotobuf.a
example_create_delete_room: /root/.local/lib/libgrpc.a
example_create_delete_room: /root/.local/lib/libz.a
example_create_delete_room: /root/.local/lib/libcares.a
example_create_delete_room: /root/.local/lib/libaddress_sorting.a
example_create_delete_room: /root/.local/lib/libre2.a
example_create_delete_room: /root/.local/lib/libabsl_hash.a
example_create_delete_room: /root/.local/lib/libabsl_city.a
example_create_delete_room: /root/.local/lib/libabsl_wyhash.a
example_create_delete_room: /root/.local/lib/libabsl_raw_hash_set.a
example_create_delete_room: /root/.local/lib/libabsl_hashtablez_sampler.a
example_create_delete_room: /root/.local/lib/libabsl_exponential_biased.a
example_create_delete_room: /root/.local/lib/libabsl_statusor.a
example_create_delete_room: /root/.local/lib/libabsl_bad_variant_access.a
example_create_delete_room: /root/.local/lib/libgpr.a
example_create_delete_room: /root/.local/lib/libupb.a
example_create_delete_room: /root/.local/lib/libabsl_status.a
example_create_delete_room: /root/.local/lib/libabsl_cord.a
example_create_delete_room: /root/.local/lib/libabsl_str_format_internal.a
example_create_delete_room: /root/.local/lib/libabsl_synchronization.a
example_create_delete_room: /root/.local/lib/libabsl_stacktrace.a
example_create_delete_room: /root/.local/lib/libabsl_symbolize.a
example_create_delete_room: /root/.local/lib/libabsl_debugging_internal.a
example_create_delete_room: /root/.local/lib/libabsl_demangle_internal.a
example_create_delete_room: /root/.local/lib/libabsl_graphcycles_internal.a
example_create_delete_room: /root/.local/lib/libabsl_malloc_internal.a
example_create_delete_room: /root/.local/lib/libabsl_time.a
example_create_delete_room: /root/.local/lib/libabsl_strings.a
example_create_delete_room: /root/.local/lib/libabsl_throw_delegate.a
example_create_delete_room: /root/.local/lib/libabsl_strings_internal.a
example_create_delete_room: /root/.local/lib/libabsl_base.a
example_create_delete_room: /root/.local/lib/libabsl_spinlock_wait.a
example_create_delete_room: /root/.local/lib/libabsl_int128.a
example_create_delete_room: /root/.local/lib/libabsl_civil_time.a
example_create_delete_room: /root/.local/lib/libabsl_time_zone.a
example_create_delete_room: /root/.local/lib/libabsl_bad_optional_access.a
example_create_delete_room: /root/.local/lib/libabsl_raw_logging_internal.a
example_create_delete_room: /root/.local/lib/libabsl_log_severity.a
example_create_delete_room: /root/.local/lib/libssl.a
example_create_delete_room: /root/.local/lib/libcrypto.a
example_create_delete_room: CMakeFiles/example_create_delete_room.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX executable example_create_delete_room"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/example_create_delete_room.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/example_create_delete_room.dir/build: example_create_delete_room

.PHONY : CMakeFiles/example_create_delete_room.dir/build

CMakeFiles/example_create_delete_room.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/example_create_delete_room.dir/cmake_clean.cmake
.PHONY : CMakeFiles/example_create_delete_room.dir/clean

CMakeFiles/example_create_delete_room.dir/depend:
	cd /home/tong/UA_BlackJack_Server/combat_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tong/UA_BlackJack_Server/combat_server /home/tong/UA_BlackJack_Server/combat_server /home/tong/UA_BlackJack_Server/combat_server/build /home/tong/UA_BlackJack_Server/combat_server/build /home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles/example_create_delete_room.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/example_create_delete_room.dir/depend

