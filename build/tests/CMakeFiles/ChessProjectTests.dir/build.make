# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.24.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.24.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/arveanlabib/Documents/Mystery-Mate

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/arveanlabib/Documents/Mystery-Mate/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/ChessProjectTests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/ChessProjectTests.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/ChessProjectTests.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/ChessProjectTests.dir/flags.make

tests/CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/flags.make
tests/CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.o: /Users/arveanlabib/Documents/Mystery-Mate/tests/test_bishop.cpp
tests/CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/arveanlabib/Documents/Mystery-Mate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.o"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.o -MF CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.o.d -o CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.o -c /Users/arveanlabib/Documents/Mystery-Mate/tests/test_bishop.cpp

tests/CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.i"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/arveanlabib/Documents/Mystery-Mate/tests/test_bishop.cpp > CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.i

tests/CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.s"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/arveanlabib/Documents/Mystery-Mate/tests/test_bishop.cpp -o CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.s

tests/CMakeFiles/ChessProjectTests.dir/test_board.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/flags.make
tests/CMakeFiles/ChessProjectTests.dir/test_board.cpp.o: /Users/arveanlabib/Documents/Mystery-Mate/tests/test_board.cpp
tests/CMakeFiles/ChessProjectTests.dir/test_board.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/arveanlabib/Documents/Mystery-Mate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/CMakeFiles/ChessProjectTests.dir/test_board.cpp.o"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/ChessProjectTests.dir/test_board.cpp.o -MF CMakeFiles/ChessProjectTests.dir/test_board.cpp.o.d -o CMakeFiles/ChessProjectTests.dir/test_board.cpp.o -c /Users/arveanlabib/Documents/Mystery-Mate/tests/test_board.cpp

tests/CMakeFiles/ChessProjectTests.dir/test_board.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ChessProjectTests.dir/test_board.cpp.i"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/arveanlabib/Documents/Mystery-Mate/tests/test_board.cpp > CMakeFiles/ChessProjectTests.dir/test_board.cpp.i

tests/CMakeFiles/ChessProjectTests.dir/test_board.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ChessProjectTests.dir/test_board.cpp.s"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/arveanlabib/Documents/Mystery-Mate/tests/test_board.cpp -o CMakeFiles/ChessProjectTests.dir/test_board.cpp.s

tests/CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/flags.make
tests/CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.o: /Users/arveanlabib/Documents/Mystery-Mate/tests/test_board_rules.cpp
tests/CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/arveanlabib/Documents/Mystery-Mate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object tests/CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.o"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.o -MF CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.o.d -o CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.o -c /Users/arveanlabib/Documents/Mystery-Mate/tests/test_board_rules.cpp

tests/CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.i"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/arveanlabib/Documents/Mystery-Mate/tests/test_board_rules.cpp > CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.i

tests/CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.s"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/arveanlabib/Documents/Mystery-Mate/tests/test_board_rules.cpp -o CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.s

tests/CMakeFiles/ChessProjectTests.dir/test_king.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/flags.make
tests/CMakeFiles/ChessProjectTests.dir/test_king.cpp.o: /Users/arveanlabib/Documents/Mystery-Mate/tests/test_king.cpp
tests/CMakeFiles/ChessProjectTests.dir/test_king.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/arveanlabib/Documents/Mystery-Mate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object tests/CMakeFiles/ChessProjectTests.dir/test_king.cpp.o"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/ChessProjectTests.dir/test_king.cpp.o -MF CMakeFiles/ChessProjectTests.dir/test_king.cpp.o.d -o CMakeFiles/ChessProjectTests.dir/test_king.cpp.o -c /Users/arveanlabib/Documents/Mystery-Mate/tests/test_king.cpp

tests/CMakeFiles/ChessProjectTests.dir/test_king.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ChessProjectTests.dir/test_king.cpp.i"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/arveanlabib/Documents/Mystery-Mate/tests/test_king.cpp > CMakeFiles/ChessProjectTests.dir/test_king.cpp.i

tests/CMakeFiles/ChessProjectTests.dir/test_king.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ChessProjectTests.dir/test_king.cpp.s"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/arveanlabib/Documents/Mystery-Mate/tests/test_king.cpp -o CMakeFiles/ChessProjectTests.dir/test_king.cpp.s

tests/CMakeFiles/ChessProjectTests.dir/test_knight.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/flags.make
tests/CMakeFiles/ChessProjectTests.dir/test_knight.cpp.o: /Users/arveanlabib/Documents/Mystery-Mate/tests/test_knight.cpp
tests/CMakeFiles/ChessProjectTests.dir/test_knight.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/arveanlabib/Documents/Mystery-Mate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object tests/CMakeFiles/ChessProjectTests.dir/test_knight.cpp.o"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/ChessProjectTests.dir/test_knight.cpp.o -MF CMakeFiles/ChessProjectTests.dir/test_knight.cpp.o.d -o CMakeFiles/ChessProjectTests.dir/test_knight.cpp.o -c /Users/arveanlabib/Documents/Mystery-Mate/tests/test_knight.cpp

tests/CMakeFiles/ChessProjectTests.dir/test_knight.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ChessProjectTests.dir/test_knight.cpp.i"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/arveanlabib/Documents/Mystery-Mate/tests/test_knight.cpp > CMakeFiles/ChessProjectTests.dir/test_knight.cpp.i

tests/CMakeFiles/ChessProjectTests.dir/test_knight.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ChessProjectTests.dir/test_knight.cpp.s"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/arveanlabib/Documents/Mystery-Mate/tests/test_knight.cpp -o CMakeFiles/ChessProjectTests.dir/test_knight.cpp.s

tests/CMakeFiles/ChessProjectTests.dir/test_move.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/flags.make
tests/CMakeFiles/ChessProjectTests.dir/test_move.cpp.o: /Users/arveanlabib/Documents/Mystery-Mate/tests/test_move.cpp
tests/CMakeFiles/ChessProjectTests.dir/test_move.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/arveanlabib/Documents/Mystery-Mate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object tests/CMakeFiles/ChessProjectTests.dir/test_move.cpp.o"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/ChessProjectTests.dir/test_move.cpp.o -MF CMakeFiles/ChessProjectTests.dir/test_move.cpp.o.d -o CMakeFiles/ChessProjectTests.dir/test_move.cpp.o -c /Users/arveanlabib/Documents/Mystery-Mate/tests/test_move.cpp

tests/CMakeFiles/ChessProjectTests.dir/test_move.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ChessProjectTests.dir/test_move.cpp.i"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/arveanlabib/Documents/Mystery-Mate/tests/test_move.cpp > CMakeFiles/ChessProjectTests.dir/test_move.cpp.i

tests/CMakeFiles/ChessProjectTests.dir/test_move.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ChessProjectTests.dir/test_move.cpp.s"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/arveanlabib/Documents/Mystery-Mate/tests/test_move.cpp -o CMakeFiles/ChessProjectTests.dir/test_move.cpp.s

tests/CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/flags.make
tests/CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.o: /Users/arveanlabib/Documents/Mystery-Mate/tests/test_pawn.cpp
tests/CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/arveanlabib/Documents/Mystery-Mate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object tests/CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.o"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.o -MF CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.o.d -o CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.o -c /Users/arveanlabib/Documents/Mystery-Mate/tests/test_pawn.cpp

tests/CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.i"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/arveanlabib/Documents/Mystery-Mate/tests/test_pawn.cpp > CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.i

tests/CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.s"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/arveanlabib/Documents/Mystery-Mate/tests/test_pawn.cpp -o CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.s

tests/CMakeFiles/ChessProjectTests.dir/test_position.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/flags.make
tests/CMakeFiles/ChessProjectTests.dir/test_position.cpp.o: /Users/arveanlabib/Documents/Mystery-Mate/tests/test_position.cpp
tests/CMakeFiles/ChessProjectTests.dir/test_position.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/arveanlabib/Documents/Mystery-Mate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object tests/CMakeFiles/ChessProjectTests.dir/test_position.cpp.o"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/ChessProjectTests.dir/test_position.cpp.o -MF CMakeFiles/ChessProjectTests.dir/test_position.cpp.o.d -o CMakeFiles/ChessProjectTests.dir/test_position.cpp.o -c /Users/arveanlabib/Documents/Mystery-Mate/tests/test_position.cpp

tests/CMakeFiles/ChessProjectTests.dir/test_position.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ChessProjectTests.dir/test_position.cpp.i"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/arveanlabib/Documents/Mystery-Mate/tests/test_position.cpp > CMakeFiles/ChessProjectTests.dir/test_position.cpp.i

tests/CMakeFiles/ChessProjectTests.dir/test_position.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ChessProjectTests.dir/test_position.cpp.s"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/arveanlabib/Documents/Mystery-Mate/tests/test_position.cpp -o CMakeFiles/ChessProjectTests.dir/test_position.cpp.s

tests/CMakeFiles/ChessProjectTests.dir/test_queen.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/flags.make
tests/CMakeFiles/ChessProjectTests.dir/test_queen.cpp.o: /Users/arveanlabib/Documents/Mystery-Mate/tests/test_queen.cpp
tests/CMakeFiles/ChessProjectTests.dir/test_queen.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/arveanlabib/Documents/Mystery-Mate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object tests/CMakeFiles/ChessProjectTests.dir/test_queen.cpp.o"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/ChessProjectTests.dir/test_queen.cpp.o -MF CMakeFiles/ChessProjectTests.dir/test_queen.cpp.o.d -o CMakeFiles/ChessProjectTests.dir/test_queen.cpp.o -c /Users/arveanlabib/Documents/Mystery-Mate/tests/test_queen.cpp

tests/CMakeFiles/ChessProjectTests.dir/test_queen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ChessProjectTests.dir/test_queen.cpp.i"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/arveanlabib/Documents/Mystery-Mate/tests/test_queen.cpp > CMakeFiles/ChessProjectTests.dir/test_queen.cpp.i

tests/CMakeFiles/ChessProjectTests.dir/test_queen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ChessProjectTests.dir/test_queen.cpp.s"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/arveanlabib/Documents/Mystery-Mate/tests/test_queen.cpp -o CMakeFiles/ChessProjectTests.dir/test_queen.cpp.s

tests/CMakeFiles/ChessProjectTests.dir/test_rook.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/flags.make
tests/CMakeFiles/ChessProjectTests.dir/test_rook.cpp.o: /Users/arveanlabib/Documents/Mystery-Mate/tests/test_rook.cpp
tests/CMakeFiles/ChessProjectTests.dir/test_rook.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/arveanlabib/Documents/Mystery-Mate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object tests/CMakeFiles/ChessProjectTests.dir/test_rook.cpp.o"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/ChessProjectTests.dir/test_rook.cpp.o -MF CMakeFiles/ChessProjectTests.dir/test_rook.cpp.o.d -o CMakeFiles/ChessProjectTests.dir/test_rook.cpp.o -c /Users/arveanlabib/Documents/Mystery-Mate/tests/test_rook.cpp

tests/CMakeFiles/ChessProjectTests.dir/test_rook.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ChessProjectTests.dir/test_rook.cpp.i"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/arveanlabib/Documents/Mystery-Mate/tests/test_rook.cpp > CMakeFiles/ChessProjectTests.dir/test_rook.cpp.i

tests/CMakeFiles/ChessProjectTests.dir/test_rook.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ChessProjectTests.dir/test_rook.cpp.s"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/arveanlabib/Documents/Mystery-Mate/tests/test_rook.cpp -o CMakeFiles/ChessProjectTests.dir/test_rook.cpp.s

tests/CMakeFiles/ChessProjectTests.dir/test_square.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/flags.make
tests/CMakeFiles/ChessProjectTests.dir/test_square.cpp.o: /Users/arveanlabib/Documents/Mystery-Mate/tests/test_square.cpp
tests/CMakeFiles/ChessProjectTests.dir/test_square.cpp.o: tests/CMakeFiles/ChessProjectTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/arveanlabib/Documents/Mystery-Mate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object tests/CMakeFiles/ChessProjectTests.dir/test_square.cpp.o"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/ChessProjectTests.dir/test_square.cpp.o -MF CMakeFiles/ChessProjectTests.dir/test_square.cpp.o.d -o CMakeFiles/ChessProjectTests.dir/test_square.cpp.o -c /Users/arveanlabib/Documents/Mystery-Mate/tests/test_square.cpp

tests/CMakeFiles/ChessProjectTests.dir/test_square.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ChessProjectTests.dir/test_square.cpp.i"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/arveanlabib/Documents/Mystery-Mate/tests/test_square.cpp > CMakeFiles/ChessProjectTests.dir/test_square.cpp.i

tests/CMakeFiles/ChessProjectTests.dir/test_square.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ChessProjectTests.dir/test_square.cpp.s"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/arveanlabib/Documents/Mystery-Mate/tests/test_square.cpp -o CMakeFiles/ChessProjectTests.dir/test_square.cpp.s

# Object files for target ChessProjectTests
ChessProjectTests_OBJECTS = \
"CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.o" \
"CMakeFiles/ChessProjectTests.dir/test_board.cpp.o" \
"CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.o" \
"CMakeFiles/ChessProjectTests.dir/test_king.cpp.o" \
"CMakeFiles/ChessProjectTests.dir/test_knight.cpp.o" \
"CMakeFiles/ChessProjectTests.dir/test_move.cpp.o" \
"CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.o" \
"CMakeFiles/ChessProjectTests.dir/test_position.cpp.o" \
"CMakeFiles/ChessProjectTests.dir/test_queen.cpp.o" \
"CMakeFiles/ChessProjectTests.dir/test_rook.cpp.o" \
"CMakeFiles/ChessProjectTests.dir/test_square.cpp.o"

# External object files for target ChessProjectTests
ChessProjectTests_EXTERNAL_OBJECTS =

tests/ChessProjectTests: tests/CMakeFiles/ChessProjectTests.dir/test_bishop.cpp.o
tests/ChessProjectTests: tests/CMakeFiles/ChessProjectTests.dir/test_board.cpp.o
tests/ChessProjectTests: tests/CMakeFiles/ChessProjectTests.dir/test_board_rules.cpp.o
tests/ChessProjectTests: tests/CMakeFiles/ChessProjectTests.dir/test_king.cpp.o
tests/ChessProjectTests: tests/CMakeFiles/ChessProjectTests.dir/test_knight.cpp.o
tests/ChessProjectTests: tests/CMakeFiles/ChessProjectTests.dir/test_move.cpp.o
tests/ChessProjectTests: tests/CMakeFiles/ChessProjectTests.dir/test_pawn.cpp.o
tests/ChessProjectTests: tests/CMakeFiles/ChessProjectTests.dir/test_position.cpp.o
tests/ChessProjectTests: tests/CMakeFiles/ChessProjectTests.dir/test_queen.cpp.o
tests/ChessProjectTests: tests/CMakeFiles/ChessProjectTests.dir/test_rook.cpp.o
tests/ChessProjectTests: tests/CMakeFiles/ChessProjectTests.dir/test_square.cpp.o
tests/ChessProjectTests: tests/CMakeFiles/ChessProjectTests.dir/build.make
tests/ChessProjectTests: src/libchess_srcs.a
tests/ChessProjectTests: /usr/local/lib/libgtest_main.a
tests/ChessProjectTests: /usr/local/lib/libgtest.a
tests/ChessProjectTests: tests/CMakeFiles/ChessProjectTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/arveanlabib/Documents/Mystery-Mate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable ChessProjectTests"
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ChessProjectTests.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && /usr/local/Cellar/cmake/3.24.2/bin/cmake -D TEST_TARGET=ChessProjectTests -D TEST_EXECUTABLE=/Users/arveanlabib/Documents/Mystery-Mate/build/tests/ChessProjectTests -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/Users/arveanlabib/Documents/Mystery-Mate/build/tests -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=ChessProjectTests_TESTS -D CTEST_FILE=/Users/arveanlabib/Documents/Mystery-Mate/build/tests/ChessProjectTests[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -D TEST_XML_OUTPUT_DIR= -P /usr/local/Cellar/cmake/3.24.2/share/cmake/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
tests/CMakeFiles/ChessProjectTests.dir/build: tests/ChessProjectTests
.PHONY : tests/CMakeFiles/ChessProjectTests.dir/build

tests/CMakeFiles/ChessProjectTests.dir/clean:
	cd /Users/arveanlabib/Documents/Mystery-Mate/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/ChessProjectTests.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/ChessProjectTests.dir/clean

tests/CMakeFiles/ChessProjectTests.dir/depend:
	cd /Users/arveanlabib/Documents/Mystery-Mate/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/arveanlabib/Documents/Mystery-Mate /Users/arveanlabib/Documents/Mystery-Mate/tests /Users/arveanlabib/Documents/Mystery-Mate/build /Users/arveanlabib/Documents/Mystery-Mate/build/tests /Users/arveanlabib/Documents/Mystery-Mate/build/tests/CMakeFiles/ChessProjectTests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/ChessProjectTests.dir/depend

