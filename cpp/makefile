# Compiler options
CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -pedantic
LDFLAGS =

# Default
default: trackdetections

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
trackcaffe: trackimages

# Caffe
trackcaffe: CXXFLAGS += -DUSE_CAFFE
trackcaffe: CXXFLAGS += `pkg-config --cflags caffe`
trackcaffe: LDFLAGS += `pkg-config --libs caffe`

# Cuda
trackcaffe: CXXFLAGS += `pkg-config --cflags cuda`
trackcaffe: LDFLAGS += `pkg-config --libs cuda`

# Create dependency files
DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

# Input/Output files
SOURCEDIR = src
BUILDDIR = build
MAINDIR = examples

# For compilation of TrackImages.cpp
trackimages: SOURCES := $(shell find $(SOURCEDIR) -name "*.cpp" -not -path *examples/*)
trackimages: SOURCES += $(SOURCEDIR)/$(MAINDIR)/TrackImages.cpp
trackimages: OBJECTS := $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

# For compilation of TrackDetections.cpp
trackdetections: SOURCES := $(shell find $(SOURCEDIR) -name "*.cpp" -not -path *examples/* -not -path *detector/*)
trackdetections: SOURCES += $(SOURCEDIR)/$(MAINDIR)/TrackDetections.cpp
trackdetections: OBJECTS := $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

# Compilation options
MKDIRIFNOTEXIST = @test -d $(@D) || mkdir -p $(@D)
COMPILE = $(CXX) $(DEPFLAGS) $(CXXFLAGS) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

trackimages: trackim.out
trackdetections: trackdet.out

.SECONDEXPANSION:
%.out: $$(OBJECTS) 
	$(CXX) -o $@ $^ $(LDFLAGS)

# I found this neat: | $(BUILDDIR)/$$(filter-out ./,$$(dir %)).dir --- %.dir: ; $(MKDIRIFNOTEXIST)
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

# Print variable
print-% : ; @echo $* = $($*)
# Run target on test data
run-% : ; ./track$*.out -f ../data/test.txt

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/* 
	rm -rf $(DEPDIR)/*
	rm *.out