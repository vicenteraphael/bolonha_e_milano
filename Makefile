CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include
SRC      = $(wildcard src/*.cpp)
TARGET   = build/bolonhaemilano

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC) $(wildcard include/*.h)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

run: all
	./$(TARGET)

clean:
	rm -rf build
