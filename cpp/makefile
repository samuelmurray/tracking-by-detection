# Compiler options
CC = g++
CCFLAGS = -std=c++11 -g -Wall -pedantic
LDFLAGS =

# OpenCV - set PKG_CONFIG_PATH to folder containing opencv.pc
CCFLAGS += `pkg-config --cflags opencv`
LDFLAGS += `pkg-config --libs opencv`

export PKG_CONFIG_PATH := ../config:$(PKG_CONFIG_PATH)

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
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main.out

# For compilation without Caffe
NCSOURCES = $(filter-out BBDetector.cpp, $(wildcard *.cpp))
NCOBJECTS = $(NCSOURCES:.cpp=.o)
NCTARGET = ncmain.out
NCLDFLAGS = `pkg-config --libs opencv`


COMPILE = $(CC) $(DEPFLAGS) $(CCFLAGS) $(INCLUDE) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

nocaffe: $(NCTARGET)

$(NCTARGET): $(NCOBJECTS)
	$(CC) -o $@ $^ $(NCLDFLAGS)


%.o: %.cpp %.h
%.o: %.cpp %.h $(DEPDIR)/%.d
	$(COMPILE) $<
	$(POSTCOMPILE)

%.o: %.cpp
%.o: %.cpp $(DEPDIR)/%.d
	$(COMPILE) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SOURCES))))

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -rf $(TARGET).dSYM/