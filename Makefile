# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

# Define custom functions
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
platformpth = $(subst /,$(PATHSEP),$1)

# Set global macros
buildDir := bin
executable := app

target := $(buildDir)/$(executable)
modsDir := $(buildDir)/mods

sources := $(call rwildcard,src/,*.cpp)
objects := $(patsubst src/%, $(buildDir)/%, $(patsubst %.cpp, %.o, $(sources)))
depends := $(patsubst %.o, %.d, $(objects))
compileFlags := -std=c++20 -I./include -I./src -g
warnings := -Wall -Wextra -Wpedantic -Werror -Wno-narrowing -Wno-missing-field-initializers
linkFlags = -L ./lib/$(platform) -l raylib -l noise -l GLEW

# Check for Windows
ifeq ($(OS), Windows_NT)
	# Set Windows macros
	platform := Windows
	CXX ?= g++
	linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
	THEN := &&
	PATHSEP := \$(BLANK)
	MKDIR := -mkdir -p
	RM := -del /q
	COPY = -robocopy "$(call platformpth,$1)" "$(call platformpth,$2)" $3
else
	# Check for MacOS/Linux
	UNAMEOS := $(shell uname)
	ifeq ($(UNAMEOS), Linux)
		# Set Linux macros
		platform := Linux
		CXX ?= g++
		linkFlags += -l GL -l m -l pthread -l dl -l rt -l X11
	endif
	ifeq ($(UNAMEOS), Darwin)
		# Set macOS macros
		platform := macOSlinkFlags
		CXX ?= clang++
		linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
	endif

	# Set UNIX macros
	THEN := ;
	PATHSEP := /
	MKDIR := mkdir -p
	RM := rm -rf
	COPY = cp $1$(PATHSEP)$3 $2
endif

outputDir := ../out

# Lists phony targets for Makefile
.PHONY: all setup submodules execute clean

# Default target to build the program
all: $(coreModDir) $(target)

core-mod: clean-mods $(coreModDir)

# Compile then execute the program
run: $(target) execute

# Sets up the project for compiling, generates includes and libs
setup: include lib

# Pull and update the the build submodules
submodules:
	git submodule update --init --recursive --depth 1

# Copy the relevant header files into includes
include: submodules
	$(MKDIR) $(call platformpth, ./include)
	$(call COPY,vendor/raylib/src,./include/raylib,raylib.h)
	$(call COPY,vendor/raylib/src,./include/raylib,raymath.h)
	$(call COPY,vendor/raylib/src,./include/raylib,rlgl.h)

# Build the raylib static library file and copy it into lib
lib: submodules
	cd vendor/raylib/src $(THEN) "$(MAKE)" PLATFORM=PLATFORM_DESKTOP GRAPHICS=GRAPHICS_API_OPENGL_43
	$(MKDIR) $(call platformpth, lib/$(platform))
	$(call COPY,vendor/raylib/src,lib/$(platform),libraylib.a)

# Link the program and create the executable
$(target): $(objects)
	$(MKDIR) $(outputDir) 
	$(CXX) $(objects) -o $(outputDir)/$(executable) $(linkFlags)
	rm -rf $(outputDir)/res
	rm -rf $(outputDir)/shaders
	cp -r ./res $(outputDir)
	cp -r ./shaders $(outputDir)

# Add all rules from dependency files
-include $(depends)

# Compile objects to the build directory
$(buildDir)/%.o: src/%.cpp Makefile
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $(warnings) $< -o $@ $(CXXFLAGS)

# Run the executable
execute:
	$(target) $(ARGS)

# Clean up all relevant files
clean:
	$(RM) $(call platformpth, $(buildDir)/*)

clean-mods:
	$(RM) $(call platformpth, $(modsDir)/*)
