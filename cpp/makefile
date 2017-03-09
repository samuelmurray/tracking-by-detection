# Compiler options
CC = g++
CCFLAGS = -std=c++11 -g -Wall -pedantic
LDFLAGS =

# OpenCV - set PKG_CONFIG_PATH to folder containing opencv.pc
CCFLAGS += `pkg-config --cflags opencv`
LDFLAGS += `pkg-config --libs opencv`

# Create dependency files
DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

# Input/Output files
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main.out

COMPILE = $(CC) $(DEPFLAGS) $(CCFLAGS) $(INCLUDE) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS) 

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