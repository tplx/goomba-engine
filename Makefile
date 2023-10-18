# Makefile

# Compiler
CXX = g++

# C++ version
CXXFLAGS = -std=c++17 -g

# Source files
SRC = Main.cpp Position.cpp Render.cpp Search.cpp Entry.cpp

# Output executable
OUT = goomba

all: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
