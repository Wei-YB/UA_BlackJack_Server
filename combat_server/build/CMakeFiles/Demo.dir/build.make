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
include CMakeFiles/Demo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Demo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Demo.dir/flags.make

CMakeFiles/Demo.dir/main.cc.o: CMakeFiles/Demo.dir/flags.make
CMakeFiles/Demo.dir/main.cc.o: ../main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Demo.dir/main.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Demo.dir/main.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/main.cc

CMakeFiles/Demo.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Demo.dir/main.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/main.cc > CMakeFiles/Demo.dir/main.cc.i

CMakeFiles/Demo.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Demo.dir/main.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/main.cc -o CMakeFiles/Demo.dir/main.cc.s

CMakeFiles/Demo.dir/src/Player.cc.o: CMakeFiles/Demo.dir/flags.make
CMakeFiles/Demo.dir/src/Player.cc.o: ../src/Player.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Demo.dir/src/Player.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Demo.dir/src/Player.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/Player.cc

CMakeFiles/Demo.dir/src/Player.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Demo.dir/src/Player.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/Player.cc > CMakeFiles/Demo.dir/src/Player.cc.i

CMakeFiles/Demo.dir/src/Player.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Demo.dir/src/Player.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/Player.cc -o CMakeFiles/Demo.dir/src/Player.cc.s

CMakeFiles/Demo.dir/src/Poker.cc.o: CMakeFiles/Demo.dir/flags.make
CMakeFiles/Demo.dir/src/Poker.cc.o: ../src/Poker.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Demo.dir/src/Poker.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Demo.dir/src/Poker.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/Poker.cc

CMakeFiles/Demo.dir/src/Poker.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Demo.dir/src/Poker.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/Poker.cc > CMakeFiles/Demo.dir/src/Poker.cc.i

CMakeFiles/Demo.dir/src/Poker.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Demo.dir/src/Poker.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/Poker.cc -o CMakeFiles/Demo.dir/src/Poker.cc.s

CMakeFiles/Demo.dir/src/Room.cc.o: CMakeFiles/Demo.dir/flags.make
CMakeFiles/Demo.dir/src/Room.cc.o: ../src/Room.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Demo.dir/src/Room.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Demo.dir/src/Room.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/Room.cc

CMakeFiles/Demo.dir/src/Room.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Demo.dir/src/Room.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/Room.cc > CMakeFiles/Demo.dir/src/Room.cc.i

CMakeFiles/Demo.dir/src/Room.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Demo.dir/src/Room.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/Room.cc -o CMakeFiles/Demo.dir/src/Room.cc.s

CMakeFiles/Demo.dir/src/ShuffledPokers.cc.o: CMakeFiles/Demo.dir/flags.make
CMakeFiles/Demo.dir/src/ShuffledPokers.cc.o: ../src/ShuffledPokers.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Demo.dir/src/ShuffledPokers.cc.o"
	/usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Demo.dir/src/ShuffledPokers.cc.o -c /home/tong/UA_BlackJack_Server/combat_server/src/ShuffledPokers.cc

CMakeFiles/Demo.dir/src/ShuffledPokers.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Demo.dir/src/ShuffledPokers.cc.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tong/UA_BlackJack_Server/combat_server/src/ShuffledPokers.cc > CMakeFiles/Demo.dir/src/ShuffledPokers.cc.i

CMakeFiles/Demo.dir/src/ShuffledPokers.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Demo.dir/src/ShuffledPokers.cc.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tong/UA_BlackJack_Server/combat_server/src/ShuffledPokers.cc -o CMakeFiles/Demo.dir/src/ShuffledPokers.cc.s

# Object files for target Demo
Demo_OBJECTS = \
"CMakeFiles/Demo.dir/main.cc.o" \
"CMakeFiles/Demo.dir/src/Player.cc.o" \
"CMakeFiles/Demo.dir/src/Poker.cc.o" \
"CMakeFiles/Demo.dir/src/Room.cc.o" \
"CMakeFiles/Demo.dir/src/ShuffledPokers.cc.o"

# External object files for target Demo
Demo_EXTERNAL_OBJECTS =

Demo: CMakeFiles/Demo.dir/main.cc.o
Demo: CMakeFiles/Demo.dir/src/Player.cc.o
Demo: CMakeFiles/Demo.dir/src/Poker.cc.o
Demo: CMakeFiles/Demo.dir/src/Room.cc.o
Demo: CMakeFiles/Demo.dir/src/ShuffledPokers.cc.o
Demo: CMakeFiles/Demo.dir/build.make
Demo: CMakeFiles/Demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable Demo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Demo.dir/build: Demo

.PHONY : CMakeFiles/Demo.dir/build

CMakeFiles/Demo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Demo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Demo.dir/clean

CMakeFiles/Demo.dir/depend:
	cd /home/tong/UA_BlackJack_Server/combat_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tong/UA_BlackJack_Server/combat_server /home/tong/UA_BlackJack_Server/combat_server /home/tong/UA_BlackJack_Server/combat_server/build /home/tong/UA_BlackJack_Server/combat_server/build /home/tong/UA_BlackJack_Server/combat_server/build/CMakeFiles/Demo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Demo.dir/depend

