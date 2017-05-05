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
TARGET_detectAndTrack := detectAndTrackApp.out
TARGETS := $(TARGET_detect) $(TARGET_track) $(TARGET_detectAndTrack) 

# For running Detect.cpp
OBJECTS_detect := $(filter-out $(BUILDDIR)/$(MAINDIR)/%.o,$(OBJECTS))
OBJECTS_detect += $(BUILDDIR)/$(MAINDIR)/Detect.o

# For running Track.cpp
OBJECTS_track := $(filter-out $(BUILDDIR)/$(MAINDIR)/%.o,$(OBJECTS))
OBJECTS_track += $(BUILDDIR)/$(MAINDIR)/Track.o

# For running DetectAndTrack.cpp
OBJECTS_detectAndTrack := $(filter-out $(BUILDDIR)/$(MAINDIR)/%.o,$(OBJECTS))
OBJECTS_detectAndTrack += $(BUILDDIR)/$(MAINDIR)/DetectAndTrack.o

# Build instructioncs
MKDIRIFNOTEXIST = @test -d $(@D) || mkdir -p $(@D)
COMPILE = $(CXX) $(DEPFLAGS) $(CXXFLAGS) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

# Targets
.SECONDEXPANSION:

# Instructions
all:
	@echo 'Call "make compile" to compile everything, or "make <target>" to compile and link a specific target'

# Print variable
print-% : ; @echo $* = $($*)

# Compile and link target
detect track detectAndTrack: compile $$(TARGET_$$@)

# Link target's objects
%App.out: $$(OBJECTS_%)
	@echo '(link) CXX $@'
	@$(CXX) -o $@ $^ $(LDFLAGS)

# Compile all files
compile: $(OBJECTS)

# Crazy alternative: [...]%.d | $(BUILDDIR)/$$(filter-out ./,$$(dir %)).dir --- %.dir: ; $(MKDIRIFNOTEXIST)
$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(DEPDIR)/%.d
	@echo '(compile) CXX $<'
	@$(MKDIRIFNOTEXIST)
	@$(COMPILE) $< -o $@
	@$(POSTCOMPILE)

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