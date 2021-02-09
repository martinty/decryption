# Name of the executable
EXECUTABLE := program

# Library path
GRAPH_LIB_DIR := /usr/local
GRAPH_LIB_INCLUDE := $(GRAPH_LIB_DIR)/include/Graph_lib
GRAPH_LIB_LIB := $(GRAPH_LIB_DIR)/lib/Graph_lib

# Output directories
DEBUG_OUT := Debug
RELEASE_OUT := Release

# Use clang as compiler and linker
# Use C++17 and suppress false positive warnings
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wno-overloaded-virtual -I$(GRAPH_LIB_INCLUDE)
LDFLAGS = -L$(GRAPH_LIB_LIB) -l$(GRAPH_LIB)
LINK = $(CXX)

# fltk-config returns compiler and linker flags needed successfully
# build fltk programs.
CXXFLAGS += $(shell fltk-config --use-images --cxxflags)
LDFLAGS += $(shell fltk-config --use-images --ldflags)

SOURCES := $(wildcard *.cpp)

ifdef DEBUG
CXXFLAGS += -g -ggdb -O0 -DDEBUG
OUT := $(DEBUG_OUT)
GRAPH_LIB := Graph_libd
else
CXXFLAGS += -O3 -DNDEBUG
OUT := $(RELEASE_OUT)
GRAPH_LIB := Graph_lib
endif

OBJECTS := $(SOURCES:%.cpp=$(OUT)/%.o)

$(OUT):
	mkdir -p $@

$(OUT)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUT)/$(EXECUTABLE): $(OUT) $(OBJECTS)
	$(LINK) $(OBJECTS) -o $@ $(LDFLAGS)

.PHONY: runrelease rundebug
runrelease: release
	./$(RELEASE_OUT)/$(EXECUTABLE)
rundebug: debug
	./$(DEBUG_OUT)/$(EXECUTABLE)

.PHONY: clean cleandebug cleanrelease
clean: cleandebug cleanrelease
cleandebug:
	$(RM) -r $(DEBUG_OUT)
cleanrelease:
	$(RM) -r $(RELEASE_OUT)

# Helpers to build release and debug binaries
.PHONY: debug release
debug:
	DEBUG=1 $(MAKE) $(DEBUG_OUT)/$(EXECUTABLE)
release:
	$(MAKE) $(RELEASE_OUT)/$(EXECUTABLE)


# Helper to clean console
.PHONY: clearscreen
clearscreen:
	clear
