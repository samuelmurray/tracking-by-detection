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
caffe: CXXFLAGS += `pkg-config --cflags caffe`
caffe: LDFLAGS += `pkg-config --libs caffe`

# Cuda
caffe: CXXFLAGS += `pkg-config --cflags cuda`
caffe: LDFLAGS += `pkg-config --libs cuda`

# Create dependency files
DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

# Input/Output files
SOURCEDIR = src
BUILDDIR = build
#SOURCES = $(wildcard $(SOURCEDIR)/*.cpp $(SOURCEDIR)/**/*.cpp)
SOURCES := $(shell find $(SOURCEDIR) -name "*.cpp")
OBJECTS = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
TARGET = main.out

# For compilation without Caffe
nocaffe: CAFFE_DEPOBJ = detector/BBDetector.o
nocaffe: OBJECTS := $(filter-out $(addprefix $(BUILDDIR)/,$(CAFFE_DEPOBJ)), $(OBJECTS))

# Compilation options
MKDIRIFNOTEXIST = @test -d $(@D) || mkdir -p $(@D)
COMPILE = $(CXX) $(DEPFLAGS) $(CXXFLAGS) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

caffe nocaffe: $(TARGET)

.SECONDEXPANSION:
.SECONDARY: $(OBJECTS)
%.out: $$(OBJECTS) 
	$(CXX) -o $@ $^ $(LDFLAGS)

# Instead of "| ... " we could call MKDIRIFNOTEXIST always, but I found this neat
$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(SOURCEDIR)/%.h $(DEPDIR)/%.d | $(BUILDDIR)/$$(filter-out ./,$$(dir %)).dir
	$(COMPILE) $< -o $@
	$(POSTCOMPILE)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(DEPDIR)/%.d | $(BUILDDIR)/$$(filter-out ./,$$(dir %)).dir
	$(COMPILE) $< -o $@
	$(POSTCOMPILE)

%.dir:
	$(MKDIRIFNOTEXIST)

$(DEPDIR)/%.d:
	$(MKDIRIFNOTEXIST)
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SOURCES))))

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/* 
	rm -rf $(DEPDIR)/*
	rm -f $(TARGET)