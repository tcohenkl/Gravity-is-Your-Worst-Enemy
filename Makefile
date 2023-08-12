# compiler
CXX = g++

# compiler flags
CXXFLAGS = -std=c++17 -Wall -I/opt/homebrew/Cellar/sfml/2.6.0/include

# linker flags
LDFLAGS = -L/opt/homebrew/Cellar/sfml/2.6.0/lib -lsfml-graphics -lsfml-window -lsfml-system

# sources and objects
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

# target executable
TARGET = build/rocket_game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
