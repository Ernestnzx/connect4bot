CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17 -Ofast
OBJDIR = obj
EXECUTABLES = connect4 openingbook

# Source files for each executable
SOURCES_connect4 = main.cpp $(filter-out openingbook.cpp, $(wildcard *.cpp))
SOURCES_openingbook = openingbook.cpp $(filter-out main.cpp, $(wildcard *.cpp))

# Object files for each executable
OBJECTS_connect4 = $(SOURCES_connect4:%.cpp=$(OBJDIR)/%.o)
OBJECTS_openingbook = $(SOURCES_openingbook:%.cpp=$(OBJDIR)/%.o)

all: connect4

connect4: $(OBJECTS_connect4)
	$(CXX) $(CXXFLAGS) $^ -o $@

openingbook: $(OBJECTS_openingbook)
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
