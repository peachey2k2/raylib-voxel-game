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
buildDir := ../../bin/mods
modName := WorldEater
sources := $(call rwildcard,./,*.cpp)
objects := $(patsubst ./%, $(buildDir)/%, $(patsubst %.cpp, %.o, $(sources)))
depends := $(patsubst %.o, %.d, $(objects))
# compileFlags := -std=c++20 -I./include -I./src -I./include -g
compileFlags := -std=c++20 -I ../../include -I ./src/misc -I -g -fPIC
warnings := -Wall -Wextra -Wpedantic -Werror -Wno-narrowing -Wno-missing-field-initializers
linkFlags = -shared -L lib/$(platform) -L ../../lib -l raylib -l noise

# Check for Windows
ifeq ($(OS), Windows_NT)
	# Set Windows macros
	platform := Windows
	libName := $(modName).dll
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
		libName := lib$(modName).so
		CXX ?= g++
		linkFlags += -l GL -l m -l pthread -l dl -l rt -l X11
	endif
	ifeq ($(UNAMEOS), Darwin)
		# Set macOS macros
		platform := macOSlinkFlags
		libName := lib$(modName).dylib
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

target := $(buildDir)/$(libName)

# Lists phony targets for Makefile
.PHONY: all clean

all: $(target)

# Link the program and create the executable
$(target): $(objects)
	$(CXX) $(objects) -o $(target) $(linkFlags)

# Add all rules from dependency files
-include $(depends)

# Compile objects to the build directory
$(buildDir)/%.o: ./%.cpp Makefile
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $(warnings) $< -o $@ $(CXXFLAGS)

# Clean the build directory
clean:
	$(RM) $(buildDir)$(PATHSEP)*

