CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/opt/homebrew/Cellar/sfml/2.6.0/include
LDFLAGS = -L/opt/homebrew/Cellar/sfml/2.6.0/lib -lsfml-graphics -lsfml-window -lsfml-system

SRCS = $(wildcard src/*.cpp) $(wildcard src/game/*.cpp) $(wildcard src/engine/*.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = build/my_game

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p build
	$(CXX) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
