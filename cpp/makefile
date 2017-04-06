SHELL = /bin/sh

# Compiler options
CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -pedantic
LDFLAGS =
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

# External libraries
export PKG_CONFIG_PATH := ../config:$(PKG_CONFIG_PATH)

# OpenCV
CXXFLAGS += `pkg-config --cflags opencv`
LDFLAGS += `pkg-config --libs opencv`

# dlib
CXXFLAGS += `pkg-config --cflags dlib-1`
LDFLAGS += `pkg-config --libs dlib-1`

# openblas
CXXFLAGS += `pkg-config --cflags openblas`
LDFLAGS += `pkg-config --libs openblas`

# Boost
CXXFLAGS += `pkg-config --cflags boost`
LDFLAGS += `pkg-config --libs boost`

# For compilation with Caffe
use_caffe = false
ifeq ($(use_caffe),true)
	# Caffe
	CXXFLAGS += -DUSE_CAFFE
	CXXFLAGS += `pkg-config --cflags caffe`
	LDFLAGS += `pkg-config --libs caffe`

	# Cuda
	CXXFLAGS += `pkg-config --cflags cuda`
	LDFLAGS += `pkg-config --libs cuda`
endif

# Directories
SOURCEDIR = src
BUILDDIR = build
MAINDIR = examples
DEPDIR = .d

# Input/Output files
SOURCES := $(shell find $(SOURCEDIR) -name "*.cpp")
OBJECTS := $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
DEPFILES := $(patsubst $(SOURCEDIR)/%.cpp,$(DEPDIR)/%.d,$(SOURCES))
TARGET_detect := detectApp.out
TARGET_track := trackApp.out
TARGETS := $(TARGET_detect) $(TARGET_track)

# For running DetectAndTrack.cpp
#detect: OBJECTS := $(filter-out $(BUILDDIR)/$(MAINDIR)/%.o,$(OBJECTS))
#detect: OBJECTS += $(BUILDDIR)/$(MAINDIR)/DetectAndTrack.o
OBJECTS_detect := $(filter-out $(BUILDDIR)/$(MAINDIR)/%.o,$(OBJECTS))
OBJECTS_detect += $(BUILDDIR)/$(MAINDIR)/DetectAndTrack.o

# For running Track.cpp
#track: OBJECTS := $(filter-out $(BUILDDIR)/$(MAINDIR)/%.o,$(OBJECTS))
#track: OBJECTS += $(BUILDDIR)/$(MAINDIR)/Track.o
OBJECTS_track := $(filter-out $(BUILDDIR)/$(MAINDIR)/%.o,$(OBJECTS))
OBJECTS_track += $(BUILDDIR)/$(MAINDIR)/Track.o

# Build instructioncs
MKDIRIFNOTEXIST = @test -d $(@D) || mkdir -p $(@D)
COMPILE = $(CXX) $(DEPFLAGS) $(CXXFLAGS) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

# Targets
.SECONDEXPANSION:

# Instructions
all:
	@echo 'Call "make <target>" to build or "make run-<target>" to build and run'

# Print variable
print-% : ; @echo $* = $($*)

# (Build and) run target on test data
run-% : % ; ./$(TARGET_$*) -f ../data/test.txt

# Compile and link target
detect track: compile $$(TARGET_$$@)

# Link target's objects
%App.out: $$(OBJECTS_%)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile all files
compile: $(OBJECTS)

# Crazy alternative: [...]%.d | $(BUILDDIR)/$$(filter-out ./,$$(dir %)).dir --- %.dir: ; $(MKDIRIFNOTEXIST)
$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(DEPDIR)/%.d
	$(MKDIRIFNOTEXIST)
	$(COMPILE) $< -o $@
	$(POSTCOMPILE)

$(DEPDIR)/%.d:
	$(MKDIRIFNOTEXIST)

# Prevent files from being deleted as intermediate files by make
.PRECIOUS: $(OBJECTS) $(DEPFILES)

# Include dependency files
include $(wildcard $(DEPFILES))

# Rebuild all object files
force_rebuild = false
ifeq ($(force_rebuild),true)
$(shell rm $(OBJECTS))
endif

# Remove all generated files
.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(DEPFILES)
	rm -f $(TARGETS)