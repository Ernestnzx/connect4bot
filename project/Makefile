CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17 -Ofast -pthread
OBJDIR = obj
EXECUTABLES = connect4 openingbook generator

# Source files for each executable
SOURCES_connect4 = main.cpp $(filter-out openingbook.cpp generator.cpp, $(wildcard *.cpp))
SOURCES_openingbook = openingbook.cpp $(filter-out main.cpp generator.cpp, $(wildcard *.cpp))
SOURCES_generator= generator.cpp $(filter-out openingbook.cpp main.cpp, $(wildcard *.cpp))

# Object files for each executable
OBJECTS_connect4 = $(SOURCES_connect4:%.cpp=$(OBJDIR)/%.o)
OBJECTS_openingbook = $(SOURCES_openingbook:%.cpp=$(OBJDIR)/%.o)
OBJECTS_generator = $(SOURCES_generator:%.cpp=$(OBJDIR)/%.o)

all: connect4

connect4: $(OBJECTS_connect4)
	$(CXX) $(CXXFLAGS) $^ -o $@

openingbook: $(OBJECTS_openingbook)
	$(CXX) $(CXXFLAGS) $^ -o $@

generator: $(OBJECTS_generator)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(EXECUTABLES)
	rm -rf $(OBJDIR)

# Phony targets
.PHONY: all clean
