# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.27

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\ricky\Documents\Projects\vulkrayphy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\ricky\Documents\Projects\vulkrayphy\build-windows

# Include any dependencies generated for this target.
include CMakeFiles/GameTime.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/GameTime.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/GameTime.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GameTime.dir/flags.make

CMakeFiles/GameTime.dir/src/main.cpp.obj: CMakeFiles/GameTime.dir/flags.make
CMakeFiles/GameTime.dir/src/main.cpp.obj: CMakeFiles/GameTime.dir/includes_CXX.rsp
CMakeFiles/GameTime.dir/src/main.cpp.obj: C:/Users/ricky/Documents/Projects/vulkrayphy/src/main.cpp
CMakeFiles/GameTime.dir/src/main.cpp.obj: CMakeFiles/GameTime.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\ricky\Documents\Projects\vulkrayphy\build-windows\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/GameTime.dir/src/main.cpp.obj"
	C:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GameTime.dir/src/main.cpp.obj -MF CMakeFiles\GameTime.dir\src\main.cpp.obj.d -o CMakeFiles\GameTime.dir\src\main.cpp.obj -c C:\Users\ricky\Documents\Projects\vulkrayphy\src\main.cpp

CMakeFiles/GameTime.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GameTime.dir/src/main.cpp.i"
	C:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\ricky\Documents\Projects\vulkrayphy\src\main.cpp > CMakeFiles\GameTime.dir\src\main.cpp.i

CMakeFiles/GameTime.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GameTime.dir/src/main.cpp.s"
	C:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\ricky\Documents\Projects\vulkrayphy\src\main.cpp -o CMakeFiles\GameTime.dir\src\main.cpp.s

# Object files for target GameTime
GameTime_OBJECTS = \
"CMakeFiles/GameTime.dir/src/main.cpp.obj"

# External object files for target GameTime
GameTime_EXTERNAL_OBJECTS =

GameTime.exe: CMakeFiles/GameTime.dir/src/main.cpp.obj
GameTime.exe: CMakeFiles/GameTime.dir/build.make
GameTime.exe: lib/glfw-3.4/src/libglfw3.a
GameTime.exe: C:/VulkanSDK/1.3.296.0/Lib/vulkan-1.lib
GameTime.exe: CMakeFiles/GameTime.dir/linkLibs.rsp
GameTime.exe: CMakeFiles/GameTime.dir/objects1.rsp
GameTime.exe: CMakeFiles/GameTime.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\ricky\Documents\Projects\vulkrayphy\build-windows\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable GameTime.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\GameTime.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GameTime.dir/build: GameTime.exe
.PHONY : CMakeFiles/GameTime.dir/build

CMakeFiles/GameTime.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\GameTime.dir\cmake_clean.cmake
.PHONY : CMakeFiles/GameTime.dir/clean

CMakeFiles/GameTime.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\ricky\Documents\Projects\vulkrayphy C:\Users\ricky\Documents\Projects\vulkrayphy C:\Users\ricky\Documents\Projects\vulkrayphy\build-windows C:\Users\ricky\Documents\Projects\vulkrayphy\build-windows C:\Users\ricky\Documents\Projects\vulkrayphy\build-windows\CMakeFiles\GameTime.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/GameTime.dir/depend
