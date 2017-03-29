# Compiler options
CC = g++
CCFLAGS = -std=c++11 -g -Wall -pedantic
LDFLAGS =

# Print variable
print-%  : ; @echo $* = $($*)

# OpenCV - set PKG_CONFIG_PATH to folder containing opencv.pc
CCFLAGS += `pkg-config --cflags opencv`
LDFLAGS += `pkg-config --libs opencv`

# dlib
CCFLAGS += `pkg-config --cflags dlib-1`
LDFLAGS += `pkg-config --libs dlib-1`

# openblas
CCFLAGS += `pkg-config --cflags openblas`
LDFLAGS += `pkg-config --libs openblas`

export PKG_CONFIG_PATH := ../config:$(PKG_CONFIG_PATH)

# FOR USE WITH CAFFE

# Caffe
caffe: CCFLAGS += `pkg-config --cflags caffe`
caffe: LDFLAGS += `pkg-config --libs caffe`

# Cuda
caffe: CCFLAGS += `pkg-config --cflags cuda`
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
CAFFE_DEPOBJ = detector/BBDetector.o
NOCAFFE_OBJECTS = $(filter-out $(addprefix $(BUILDDIR)/,$(CAFFE_DEPOBJ)), $(OBJECTS))
NOCAFFE_TARGET = nocaffe.out

# Compilation options
COMPILE = $(CC) $(DEPFLAGS) $(CCFLAGS) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

# With Caffe
caffe: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Without Caffe
nocaffe: $(NOCAFFE_TARGET)

$(NOCAFFE_TARGET): $(NOCAFFE_OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)


$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(SOURCEDIR)/%.h
$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(SOURCEDIR)/%.h $(DEPDIR)/%.d
	@mkdir -p "$(@D)"
	$(COMPILE) $< -o $@
	$(POSTCOMPILE)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(DEPDIR)/%.d
	@mkdir -p "$(@D)"
	$(COMPILE) $< -o $@
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
	@mkdir -p "$(@D)"
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SOURCES))))

clean:
	rm -rf $(BUILDDIR)/* 
	rm -rf $(DEPDIR)/*
	rm -f $(TARGET) $(NOCAFFE_TARGET)