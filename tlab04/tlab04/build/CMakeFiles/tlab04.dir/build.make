# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jordi/TradingLab/tlab/tlab04/tlab04

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jordi/TradingLab/tlab/tlab04/tlab04/build

# Include any dependencies generated for this target.
include CMakeFiles/tlab04.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tlab04.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tlab04.dir/flags.make

CMakeFiles/tlab04.dir/main.o: CMakeFiles/tlab04.dir/flags.make
CMakeFiles/tlab04.dir/main.o: ../main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jordi/TradingLab/tlab/tlab04/tlab04/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/tlab04.dir/main.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/tlab04.dir/main.o -c /home/jordi/TradingLab/tlab/tlab04/tlab04/main.cpp

CMakeFiles/tlab04.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tlab04.dir/main.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jordi/TradingLab/tlab/tlab04/tlab04/main.cpp > CMakeFiles/tlab04.dir/main.i

CMakeFiles/tlab04.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tlab04.dir/main.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jordi/TradingLab/tlab/tlab04/tlab04/main.cpp -o CMakeFiles/tlab04.dir/main.s

CMakeFiles/tlab04.dir/main.o.requires:
.PHONY : CMakeFiles/tlab04.dir/main.o.requires

CMakeFiles/tlab04.dir/main.o.provides: CMakeFiles/tlab04.dir/main.o.requires
	$(MAKE) -f CMakeFiles/tlab04.dir/build.make CMakeFiles/tlab04.dir/main.o.provides.build
.PHONY : CMakeFiles/tlab04.dir/main.o.provides

CMakeFiles/tlab04.dir/main.o.provides.build: CMakeFiles/tlab04.dir/main.o

# Object files for target tlab04
tlab04_OBJECTS = \
"CMakeFiles/tlab04.dir/main.o"

# External object files for target tlab04
tlab04_EXTERNAL_OBJECTS =

tlab04: CMakeFiles/tlab04.dir/main.o
tlab04: CMakeFiles/tlab04.dir/build.make
tlab04: CMakeFiles/tlab04.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable tlab04"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tlab04.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tlab04.dir/build: tlab04
.PHONY : CMakeFiles/tlab04.dir/build

CMakeFiles/tlab04.dir/requires: CMakeFiles/tlab04.dir/main.o.requires
.PHONY : CMakeFiles/tlab04.dir/requires

CMakeFiles/tlab04.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tlab04.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tlab04.dir/clean

CMakeFiles/tlab04.dir/depend:
	cd /home/jordi/TradingLab/tlab/tlab04/tlab04/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jordi/TradingLab/tlab/tlab04/tlab04 /home/jordi/TradingLab/tlab/tlab04/tlab04 /home/jordi/TradingLab/tlab/tlab04/tlab04/build /home/jordi/TradingLab/tlab/tlab04/tlab04/build /home/jordi/TradingLab/tlab/tlab04/tlab04/build/CMakeFiles/tlab04.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tlab04.dir/depend

