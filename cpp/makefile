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

# For building TrackImages.cpp
detect: OBJECTS := $(filter-out $(BUILDDIR)/$(MAINDIR)/%.o,$(OBJECTS))
detect: OBJECTS += $(BUILDDIR)/$(MAINDIR)/TrackImages.o

# For building TrackDetections.cpp
track: OBJECTS := $(filter-out $(BUILDDIR)/$(MAINDIR)/%.o,$(OBJECTS))
track: OBJECTS += $(BUILDDIR)/$(MAINDIR)/TrackDetections.o

# Build instructioncs
MKDIRIFNOTEXIST = @test -d $(@D) || mkdir -p $(@D)
COMPILE = $(CXX) $(DEPFLAGS) $(CXXFLAGS) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

# Print variable
print-% : ; @echo $* = $($*)

# Run target on test data
run-% : % ; ./$*App.out -f ../data/test.txt

detect: compile detectApp.out
track: compile trackApp.out

.SECONDEXPANSION:
# Link target's objects
%.out: $$(OBJECTS) 
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile all files
compile: $(OBJECTS)

# Crazy alternative: [...]%.d | $(BUILDDIR)/$$(filter-out ./,$$(dir %)).dir --- %.dir: ; $(MKDIRIFNOTEXIST)
$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(SOURCEDIR)/%.h $(DEPDIR)/%.d
	$(MKDIRIFNOTEXIST)
	$(COMPILE) $< -o $@
	$(POSTCOMPILE)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(DEPDIR)/%.d
	$(MKDIRIFNOTEXIST)
	$(COMPILE) $< -o $@
	$(POSTCOMPILE)

$(DEPDIR)/%.d:
	$(MKDIRIFNOTEXIST)

.PRECIOUS: $(BUILDDIR)/%.o $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SOURCES))))

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/* 
	rm -rf $(DEPDIR)/*
	rm -f *.out