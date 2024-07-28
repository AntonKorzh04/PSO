CXX = g++
CXXFLAGS = -std=c++14 -Iinclude
LDFLAGS = -Llib -lsfml-graphics -lsfml-window -lsfml-system

SRCDIR = src
BINDIR = bin
TARGET = $(BINDIR)/my_program

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BINDIR)
