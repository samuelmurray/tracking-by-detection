# Compiler options
CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -pedantic
LDFLAGS =

# Print variable
print-%  : ; @echo $* = $($*)

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

# For compilation with Caffe
# Caffe
trackimages: CXXFLAGS += `pkg-config --cflags caffe`
trackimages: LDFLAGS += `pkg-config --libs caffe`

# Cuda
trackimages: CXXFLAGS += `pkg-config --cflags cuda`
trackimages: LDFLAGS += `pkg-config --libs cuda`

# Create dependency files
DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

# Input/Output files
SOURCEDIR = src
BUILDDIR = build
MAINDIR = examples
TARGET = main.out

# For compilation of TrackImages.cpp
trackimages: SOURCES := $(shell find $(SOURCEDIR) -name "*.cpp" -not -path *examples/*)
trackimages: OBJECTS := $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
trackimages: OBJECTS += $(BUILDDIR)/$(MAINDIR)/TrackImages.o

# For compilation of TrackDetections.cpp
trackdetections: SOURCES := $(shell find $(SOURCEDIR) -name "*.cpp" -not -path *examples/* -not -path *detector/*)
trackdetections: OBJECTS := $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
trackdetections: OBJECTS += $(BUILDDIR)/$(MAINDIR)/TrackDetections.o

# Compilation options
MKDIRIFNOTEXIST = @test -d $(@D) || mkdir -p $(@D)
COMPILE = $(CXX) $(DEPFLAGS) $(CXXFLAGS) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

default: trackdetections
trackimages: $(TARGET)
trackdetections: $(TARGET)

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

.PHONY: run clean
run:
	./$(TARGET) -f ../data/train.txt
clean:
	rm -rf $(BUILDDIR)/* 
	rm -rf $(DEPDIR)/*
	rm -f $(TARGET)