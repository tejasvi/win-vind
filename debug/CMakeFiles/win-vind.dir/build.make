# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\CMake\bin\cmake.exe

# The command to remove a file.
RM = C:\CMake\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\pit-r\GoogleDrive\GitHub\win-vind

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\pit-r\GoogleDrive\GitHub\win-vind\debug

# Include any dependencies generated for this target.
include CMakeFiles/win-vind.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/win-vind.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/win-vind.dir/flags.make

resource.rc.obj:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\pit-r\GoogleDrive\GitHub\win-vind\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating resource.rc.obj"
	C:\MinGW\bin\windres.exe -D GCC_WINDRES -I C:/Users/pit-r/GoogleDrive/GitHub/win-vind -I C:/Users/pit-r/GoogleDrive/GitHub/win-vind/debug -o C:/Users/pit-r/GoogleDrive/GitHub/win-vind/debug/resource.rc.obj -i C:/Users/pit-r/GoogleDrive/GitHub/win-vind/resources/resource.rc

CMakeFiles/win-vind.dir/src/main.cpp.obj: CMakeFiles/win-vind.dir/flags.make
CMakeFiles/win-vind.dir/src/main.cpp.obj: CMakeFiles/win-vind.dir/includes_CXX.rsp
CMakeFiles/win-vind.dir/src/main.cpp.obj: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\pit-r\GoogleDrive\GitHub\win-vind\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/win-vind.dir/src/main.cpp.obj"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\win-vind.dir\src\main.cpp.obj -c C:\Users\pit-r\GoogleDrive\GitHub\win-vind\src\main.cpp

CMakeFiles/win-vind.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/win-vind.dir/src/main.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\pit-r\GoogleDrive\GitHub\win-vind\src\main.cpp > CMakeFiles\win-vind.dir\src\main.cpp.i

CMakeFiles/win-vind.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/win-vind.dir/src/main.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\pit-r\GoogleDrive\GitHub\win-vind\src\main.cpp -o CMakeFiles\win-vind.dir\src\main.cpp.s

# Object files for target win-vind
win__vind_OBJECTS = \
"CMakeFiles/win-vind.dir/src/main.cpp.obj"

# External object files for target win-vind
win__vind_EXTERNAL_OBJECTS = \
"C:/Users/pit-r/GoogleDrive/GitHub/win-vind/debug/resource.rc.obj"

win-vind.exe: CMakeFiles/win-vind.dir/src/main.cpp.obj
win-vind.exe: resource.rc.obj
win-vind.exe: CMakeFiles/win-vind.dir/build.make
win-vind.exe: src/libMain.a
win-vind.exe: src/libKeybinds.a
win-vind.exe: src/libEditorKeybinds.a
win-vind.exe: src/libOptions.a
win-vind.exe: src/libCommon.a
win-vind.exe: CMakeFiles/win-vind.dir/linklibs.rsp
win-vind.exe: CMakeFiles/win-vind.dir/objects1.rsp
win-vind.exe: CMakeFiles/win-vind.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\pit-r\GoogleDrive\GitHub\win-vind\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable win-vind.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\win-vind.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/win-vind.dir/build: win-vind.exe

.PHONY : CMakeFiles/win-vind.dir/build

CMakeFiles/win-vind.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\win-vind.dir\cmake_clean.cmake
.PHONY : CMakeFiles/win-vind.dir/clean

CMakeFiles/win-vind.dir/depend: resource.rc.obj
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\pit-r\GoogleDrive\GitHub\win-vind C:\Users\pit-r\GoogleDrive\GitHub\win-vind C:\Users\pit-r\GoogleDrive\GitHub\win-vind\debug C:\Users\pit-r\GoogleDrive\GitHub\win-vind\debug C:\Users\pit-r\GoogleDrive\GitHub\win-vind\debug\CMakeFiles\win-vind.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/win-vind.dir/depend
