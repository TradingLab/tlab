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
CMAKE_SOURCE_DIR = /home/jordi/TradingLab/tlab

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jordi/TradingLab/tlab/build

# Include any dependencies generated for this target.
include CMakeFiles/tlab.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tlab.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tlab.dir/flags.make

CMakeFiles/tlab.dir/tlab_automoc.o: CMakeFiles/tlab.dir/flags.make
CMakeFiles/tlab.dir/tlab_automoc.o: tlab_automoc.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jordi/TradingLab/tlab/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/tlab.dir/tlab_automoc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/tlab.dir/tlab_automoc.o -c /home/jordi/TradingLab/tlab/build/tlab_automoc.cpp

CMakeFiles/tlab.dir/tlab_automoc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tlab.dir/tlab_automoc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jordi/TradingLab/tlab/build/tlab_automoc.cpp > CMakeFiles/tlab.dir/tlab_automoc.i

CMakeFiles/tlab.dir/tlab_automoc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tlab.dir/tlab_automoc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jordi/TradingLab/tlab/build/tlab_automoc.cpp -o CMakeFiles/tlab.dir/tlab_automoc.s

CMakeFiles/tlab.dir/tlab_automoc.o.requires:
.PHONY : CMakeFiles/tlab.dir/tlab_automoc.o.requires

CMakeFiles/tlab.dir/tlab_automoc.o.provides: CMakeFiles/tlab.dir/tlab_automoc.o.requires
	$(MAKE) -f CMakeFiles/tlab.dir/build.make CMakeFiles/tlab.dir/tlab_automoc.o.provides.build
.PHONY : CMakeFiles/tlab.dir/tlab_automoc.o.provides

CMakeFiles/tlab.dir/tlab_automoc.o.provides.build: CMakeFiles/tlab.dir/tlab_automoc.o

CMakeFiles/tlab.dir/julian.o: CMakeFiles/tlab.dir/flags.make
CMakeFiles/tlab.dir/julian.o: ../julian.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jordi/TradingLab/tlab/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/tlab.dir/julian.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/tlab.dir/julian.o -c /home/jordi/TradingLab/tlab/julian.cpp

CMakeFiles/tlab.dir/julian.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tlab.dir/julian.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jordi/TradingLab/tlab/julian.cpp > CMakeFiles/tlab.dir/julian.i

CMakeFiles/tlab.dir/julian.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tlab.dir/julian.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jordi/TradingLab/tlab/julian.cpp -o CMakeFiles/tlab.dir/julian.s

CMakeFiles/tlab.dir/julian.o.requires:
.PHONY : CMakeFiles/tlab.dir/julian.o.requires

CMakeFiles/tlab.dir/julian.o.provides: CMakeFiles/tlab.dir/julian.o.requires
	$(MAKE) -f CMakeFiles/tlab.dir/build.make CMakeFiles/tlab.dir/julian.o.provides.build
.PHONY : CMakeFiles/tlab.dir/julian.o.provides

CMakeFiles/tlab.dir/julian.o.provides.build: CMakeFiles/tlab.dir/julian.o

CMakeFiles/tlab.dir/main.o: CMakeFiles/tlab.dir/flags.make
CMakeFiles/tlab.dir/main.o: ../main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jordi/TradingLab/tlab/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/tlab.dir/main.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/tlab.dir/main.o -c /home/jordi/TradingLab/tlab/main.cpp

CMakeFiles/tlab.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tlab.dir/main.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jordi/TradingLab/tlab/main.cpp > CMakeFiles/tlab.dir/main.i

CMakeFiles/tlab.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tlab.dir/main.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jordi/TradingLab/tlab/main.cpp -o CMakeFiles/tlab.dir/main.s

CMakeFiles/tlab.dir/main.o.requires:
.PHONY : CMakeFiles/tlab.dir/main.o.requires

CMakeFiles/tlab.dir/main.o.provides: CMakeFiles/tlab.dir/main.o.requires
	$(MAKE) -f CMakeFiles/tlab.dir/build.make CMakeFiles/tlab.dir/main.o.provides.build
.PHONY : CMakeFiles/tlab.dir/main.o.provides

CMakeFiles/tlab.dir/main.o.provides.build: CMakeFiles/tlab.dir/main.o

# Object files for target tlab
tlab_OBJECTS = \
"CMakeFiles/tlab.dir/tlab_automoc.o" \
"CMakeFiles/tlab.dir/julian.o" \
"CMakeFiles/tlab.dir/main.o"

# External object files for target tlab
tlab_EXTERNAL_OBJECTS =

tlab: CMakeFiles/tlab.dir/tlab_automoc.o
tlab: CMakeFiles/tlab.dir/julian.o
tlab: CMakeFiles/tlab.dir/main.o
tlab: CMakeFiles/tlab.dir/build.make
tlab: /usr/lib64/libpqxx.so
tlab: /usr/lib64/libkdeui.so.5.11.5
tlab: /usr/lib64/libkdecore.so.5.11.5
tlab: /usr/lib64/libQtDBus.so
tlab: /usr/lib64/libQtCore.so
tlab: /usr/lib64/libQtGui.so
tlab: /usr/lib64/libQtSvg.so
tlab: CMakeFiles/tlab.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable tlab"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tlab.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -D_filename=/home/jordi/TradingLab/tlab/build/tlab.shell -D_library_path_variable=LD_LIBRARY_PATH -D_ld_library_path="/home/jordi/TradingLab/tlab/build/lib/./:/usr/local/lib64:/usr/lib64:/usr/lib64" -D_executable=/home/jordi/TradingLab/tlab/build/tlab -P /usr/share/kde4/apps/cmake/modules/kde4_exec_via_sh.cmake

# Rule to build all files generated by this target.
CMakeFiles/tlab.dir/build: tlab
.PHONY : CMakeFiles/tlab.dir/build

CMakeFiles/tlab.dir/requires: CMakeFiles/tlab.dir/tlab_automoc.o.requires
CMakeFiles/tlab.dir/requires: CMakeFiles/tlab.dir/julian.o.requires
CMakeFiles/tlab.dir/requires: CMakeFiles/tlab.dir/main.o.requires
.PHONY : CMakeFiles/tlab.dir/requires

CMakeFiles/tlab.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tlab.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tlab.dir/clean

CMakeFiles/tlab.dir/depend:
	cd /home/jordi/TradingLab/tlab/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jordi/TradingLab/tlab /home/jordi/TradingLab/tlab /home/jordi/TradingLab/tlab/build /home/jordi/TradingLab/tlab/build /home/jordi/TradingLab/tlab/build/CMakeFiles/tlab.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tlab.dir/depend

