# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.3.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.3.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\CLion Project\QtRun"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\CLion Project\QtRun\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/QtRun.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/QtRun.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/QtRun.dir/flags.make

CMakeFiles/QtRun.dir/main.cpp.obj: CMakeFiles/QtRun.dir/flags.make
CMakeFiles/QtRun.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\CLion Project\QtRun\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/QtRun.dir/main.cpp.obj"
	D:\Program_Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\QtRun.dir\main.cpp.obj -c "D:\CLion Project\QtRun\main.cpp"

CMakeFiles/QtRun.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QtRun.dir/main.cpp.i"
	D:\Program_Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\CLion Project\QtRun\main.cpp" > CMakeFiles\QtRun.dir\main.cpp.i

CMakeFiles/QtRun.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QtRun.dir/main.cpp.s"
	D:\Program_Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\CLion Project\QtRun\main.cpp" -o CMakeFiles\QtRun.dir\main.cpp.s

# Object files for target QtRun
QtRun_OBJECTS = \
"CMakeFiles/QtRun.dir/main.cpp.obj"

# External object files for target QtRun
QtRun_EXTERNAL_OBJECTS =

QtRun.exe: CMakeFiles/QtRun.dir/main.cpp.obj
QtRun.exe: CMakeFiles/QtRun.dir/build.make
QtRun.exe: CMakeFiles/QtRun.dir/linklibs.rsp
QtRun.exe: CMakeFiles/QtRun.dir/objects1.rsp
QtRun.exe: CMakeFiles/QtRun.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="D:\CLion Project\QtRun\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable QtRun.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\QtRun.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/QtRun.dir/build: QtRun.exe

.PHONY : CMakeFiles/QtRun.dir/build

CMakeFiles/QtRun.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\QtRun.dir\cmake_clean.cmake
.PHONY : CMakeFiles/QtRun.dir/clean

CMakeFiles/QtRun.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\CLion Project\QtRun" "D:\CLion Project\QtRun" "D:\CLion Project\QtRun\cmake-build-debug" "D:\CLion Project\QtRun\cmake-build-debug" "D:\CLion Project\QtRun\cmake-build-debug\CMakeFiles\QtRun.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/QtRun.dir/depend

