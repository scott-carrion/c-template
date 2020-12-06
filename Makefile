# Cross-platform Makefile for CLI apps by Scott Carlos Carrion
#
# This program is copyrighted and may not be duplicated or distributed
# without express permission by Scott Carlos Carrion
#
# Copyright (c) Scott Carlos Carrion, 2020
#
# scott.carrion@tamu.edu
#

# Compiler Selection
CC = gcc  # Default to GNU compiler
CXX = g++  # Default to GNU compiler

# Build directory selection (do not include / character)
BUILD_DIR = build

# Executable output
EXE = $(BUILD_DIR)/template_app
#EXE_2 = $(BUILD_DIR)/template_app_2  # Add more than one executable like this

# Source Paths (EXE)
SOURCES = src/main.cpp  # Main
# Do "SOURCES += path/to/my/other/files" to add other files for compilation of EXE

# Source Paths (EXE_2)
#SOURCES_2 = src/main_2.cpp
# Do "SOURCES_2 += path/to/my/other/files" to add other files for compilation of EXE_2

# Object name generation (EXE)
OBJS = $(addsuffix .o, $(basename $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES)))))

# Object name generation (EXE_2)
#OBJS_2 = $(addsuffix .o, $(basename $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES_2))))) 

# Extra fun stuff
.PHONY: all clean  # Phony targets
KERNEL_NAME := $(shell uname -s)
KERNEL_VERSION := $(shell uname -r)
HARDWARE_NAME := $(shell uname -m)

OS_NAME = "Other OS"

# Compiler flags and libraries to link go here
CXXFLAGS = -g -std=c++11
LIBS =

####################################################################################

# Build-specific parameters
#
# You can also put in if clauses for kernel version and hardware name similarly if
# you need! Most of the time though, difference in kernel is all that's needed

ifeq ($(KERNEL_NAME), Linux)  # If kernel is Linux, platform is GNU/Linux
        OS_NAME  = "GNU/Linux"
	# Other stuff goes here...
endif

ifeq ($(KERNEL_NAME), Darwin)  # If platform is Darwin, platform is probably Mac OS X
        OS_NAME = "MacOS"
	CC = clang  # Apple devs use their own build of LLVM/clang
        CXX = clang++  # Apple devs use their own build of LLVM/clang++
        # Other stuff goes here...
endif
####################################################################################

# General build rules

$(BUILD_DIR)/%.o:src/%.cpp  # This assumes bulk of source files are in src/ directory
	mkdir -p $(@D)
	@printf "Compiling .cpp file\n"
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# $(BUILD_DIR)/%.o:my/source/folder/%.cpp  # This is a template for adding other sources
# 	@printf Compiling .cpp file in this cool directory
# 	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE) # $(EXE_2)
	@printf "\e[92mBuild complete for $(OS_NAME) ($(KERNEL_NAME) v$(KERNEL_VERSION)/$(HARDWARE_NAME))\e[39m\n"

$(EXE): $(OBJS)
	@printf "Linking object files ($(EXE))\n"
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

#$(EXE_2): $(OBJS_2)
#	@printf "Linking object files ($(EXE_2))\n"
#	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean: 
	rm -rf $(BUILD_DIR)
