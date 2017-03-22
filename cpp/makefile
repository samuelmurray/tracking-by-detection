# Compiler options
CC = g++
CCFLAGS = -std=c++11 -g -Wall -pedantic
LDFLAGS =

SOURCEDIR = src
BUILDDIR = build

#VPATH = ./munkres

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

# Eigen
CCFLAGS += `pkg-config --cflags eigen`

# Caffe
CCFLAGS += `pkg-config --cflags caffe`
LDFLAGS += `pkg-config --libs caffe`

# Cuda
CCFLAGS += `pkg-config --cflags cuda`
LDFLAGS += `pkg-config --libs cuda`

# Create dependency files
DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

# Input/Output files
SOURCES = $(wildcard $(SOURCEDIR)/*.cpp)
#OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
TARGET = main.out

# For compilation without Caffe
NCSOURCES = $(filter-out $(SOURCEDIR)/BBDetector.cpp, $(SOURCES))
#NCOBJECTS = $(NCSOURCES:.cpp=.o)
NCOBJECTS = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(NCSOURCES))
NCTARGET = ncmain.out
NCLDFLAGS = `pkg-config --libs opencv`
NCLDFLAGS += `pkg-config --libs dlib-1`
NCLDFLAGS += `pkg-config --libs openblas`


COMPILE = $(CC) $(DEPFLAGS) $(CCFLAGS) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

nocaffe: $(NCTARGET)

$(NCTARGET): $(NCOBJECTS)
	$(CC) -o $@ $^ $(NCLDFLAGS)


$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(SOURCEDIR)/%.h
$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(SOURCEDIR)/%.h $(DEPDIR)/%.d
	$(COMPILE) $< -o $@
	$(POSTCOMPILE)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(DEPDIR)/%.d
	$(COMPILE) $< -o $@
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SOURCES))))

clean:
	rm -f $(OBJECTS) $(NCOBJECTS) $(TARGET)
	rm -rf $(TARGET).dSYM/