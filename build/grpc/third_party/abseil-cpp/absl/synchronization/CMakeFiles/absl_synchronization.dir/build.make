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
CMAKE_SOURCE_DIR = /root/ricki/UA_BlackJack_Server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/ricki/UA_BlackJack_Server/build

# Include any dependencies generated for this target.
include grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/depend.make

# Include the progress variables for this target.
include grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/progress.make

# Include the compile flags for this target's objects.
include grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/flags.make

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/barrier.cc.o: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/flags.make
grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/barrier.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/barrier.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/barrier.cc.o"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_synchronization.dir/barrier.cc.o -c /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/barrier.cc

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/barrier.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_synchronization.dir/barrier.cc.i"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/barrier.cc > CMakeFiles/absl_synchronization.dir/barrier.cc.i

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/barrier.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_synchronization.dir/barrier.cc.s"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/barrier.cc -o CMakeFiles/absl_synchronization.dir/barrier.cc.s

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/blocking_counter.cc.o: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/flags.make
grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/blocking_counter.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/blocking_counter.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/blocking_counter.cc.o"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_synchronization.dir/blocking_counter.cc.o -c /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/blocking_counter.cc

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/blocking_counter.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_synchronization.dir/blocking_counter.cc.i"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/blocking_counter.cc > CMakeFiles/absl_synchronization.dir/blocking_counter.cc.i

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/blocking_counter.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_synchronization.dir/blocking_counter.cc.s"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/blocking_counter.cc -o CMakeFiles/absl_synchronization.dir/blocking_counter.cc.s

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/create_thread_identity.cc.o: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/flags.make
grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/create_thread_identity.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/internal/create_thread_identity.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/create_thread_identity.cc.o"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_synchronization.dir/internal/create_thread_identity.cc.o -c /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/internal/create_thread_identity.cc

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/create_thread_identity.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_synchronization.dir/internal/create_thread_identity.cc.i"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/internal/create_thread_identity.cc > CMakeFiles/absl_synchronization.dir/internal/create_thread_identity.cc.i

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/create_thread_identity.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_synchronization.dir/internal/create_thread_identity.cc.s"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/internal/create_thread_identity.cc -o CMakeFiles/absl_synchronization.dir/internal/create_thread_identity.cc.s

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/per_thread_sem.cc.o: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/flags.make
grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/per_thread_sem.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/internal/per_thread_sem.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/per_thread_sem.cc.o"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_synchronization.dir/internal/per_thread_sem.cc.o -c /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/internal/per_thread_sem.cc

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/per_thread_sem.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_synchronization.dir/internal/per_thread_sem.cc.i"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/internal/per_thread_sem.cc > CMakeFiles/absl_synchronization.dir/internal/per_thread_sem.cc.i

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/per_thread_sem.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_synchronization.dir/internal/per_thread_sem.cc.s"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/internal/per_thread_sem.cc -o CMakeFiles/absl_synchronization.dir/internal/per_thread_sem.cc.s

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/waiter.cc.o: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/flags.make
grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/waiter.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/internal/waiter.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/waiter.cc.o"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_synchronization.dir/internal/waiter.cc.o -c /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/internal/waiter.cc

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/waiter.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_synchronization.dir/internal/waiter.cc.i"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/internal/waiter.cc > CMakeFiles/absl_synchronization.dir/internal/waiter.cc.i

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/waiter.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_synchronization.dir/internal/waiter.cc.s"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/internal/waiter.cc -o CMakeFiles/absl_synchronization.dir/internal/waiter.cc.s

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/notification.cc.o: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/flags.make
grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/notification.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/notification.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/notification.cc.o"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_synchronization.dir/notification.cc.o -c /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/notification.cc

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/notification.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_synchronization.dir/notification.cc.i"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/notification.cc > CMakeFiles/absl_synchronization.dir/notification.cc.i

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/notification.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_synchronization.dir/notification.cc.s"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/notification.cc -o CMakeFiles/absl_synchronization.dir/notification.cc.s

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/mutex.cc.o: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/flags.make
grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/mutex.cc.o: ../thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/mutex.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/mutex.cc.o"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_synchronization.dir/mutex.cc.o -c /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/mutex.cc

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/mutex.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_synchronization.dir/mutex.cc.i"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/mutex.cc > CMakeFiles/absl_synchronization.dir/mutex.cc.i

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/mutex.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_synchronization.dir/mutex.cc.s"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization/mutex.cc -o CMakeFiles/absl_synchronization.dir/mutex.cc.s

# Object files for target absl_synchronization
absl_synchronization_OBJECTS = \
"CMakeFiles/absl_synchronization.dir/barrier.cc.o" \
"CMakeFiles/absl_synchronization.dir/blocking_counter.cc.o" \
"CMakeFiles/absl_synchronization.dir/internal/create_thread_identity.cc.o" \
"CMakeFiles/absl_synchronization.dir/internal/per_thread_sem.cc.o" \
"CMakeFiles/absl_synchronization.dir/internal/waiter.cc.o" \
"CMakeFiles/absl_synchronization.dir/notification.cc.o" \
"CMakeFiles/absl_synchronization.dir/mutex.cc.o"

# External object files for target absl_synchronization
absl_synchronization_EXTERNAL_OBJECTS =

grpc/third_party/abseil-cpp/absl/synchronization/libabsl_synchronization.a: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/barrier.cc.o
grpc/third_party/abseil-cpp/absl/synchronization/libabsl_synchronization.a: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/blocking_counter.cc.o
grpc/third_party/abseil-cpp/absl/synchronization/libabsl_synchronization.a: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/create_thread_identity.cc.o
grpc/third_party/abseil-cpp/absl/synchronization/libabsl_synchronization.a: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/per_thread_sem.cc.o
grpc/third_party/abseil-cpp/absl/synchronization/libabsl_synchronization.a: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/internal/waiter.cc.o
grpc/third_party/abseil-cpp/absl/synchronization/libabsl_synchronization.a: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/notification.cc.o
grpc/third_party/abseil-cpp/absl/synchronization/libabsl_synchronization.a: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/mutex.cc.o
grpc/third_party/abseil-cpp/absl/synchronization/libabsl_synchronization.a: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/build.make
grpc/third_party/abseil-cpp/absl/synchronization/libabsl_synchronization.a: grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/ricki/UA_BlackJack_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX static library libabsl_synchronization.a"
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && $(CMAKE_COMMAND) -P CMakeFiles/absl_synchronization.dir/cmake_clean_target.cmake
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/absl_synchronization.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/build: grpc/third_party/abseil-cpp/absl/synchronization/libabsl_synchronization.a

.PHONY : grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/build

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/clean:
	cd /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization && $(CMAKE_COMMAND) -P CMakeFiles/absl_synchronization.dir/cmake_clean.cmake
.PHONY : grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/clean

grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/depend:
	cd /root/ricki/UA_BlackJack_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ricki/UA_BlackJack_Server /root/ricki/UA_BlackJack_Server/thirdparty/grpc/third_party/abseil-cpp/absl/synchronization /root/ricki/UA_BlackJack_Server/build /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization /root/ricki/UA_BlackJack_Server/build/grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grpc/third_party/abseil-cpp/absl/synchronization/CMakeFiles/absl_synchronization.dir/depend

