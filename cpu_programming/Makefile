PROJECT=dashcam
SOURCES=test.cpp trivium.cpp

CXX=g++
CXX_FLAGS = -Wconversion -std=c++0x

# ------------ MAGIC BEGINS HERE -------------

# Automatic generation of some important lists
OBJECTS=$(SOURCES:.cpp=.o)

test: test.o trivium.o
	@echo Linking...
	@$(CXX) $^ -o $@ $(CXX_FLAGS)

all: $(SOURCES) 

clean:
	rm -f $(OBJECTS)

%.o: %.cpp
	@echo Compiling $<
	@$(CXX) -c -o $@ $< $(CXX_FLAGS)