CC = g++
CCFLAGS = -std=c++11 -g -Wall -pedantic
LDFLAGS =

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main.out

COMPILE = $(CC) $(DEPFLAGS) $(CCFLAGS) -c
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
	rm -f *.o *.out
	rm *.out.dSYM/*
	rmdir .out.dSYM