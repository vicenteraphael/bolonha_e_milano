CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include
SRC      = $(wildcard src/*.cpp)
# Os testes tem main proprio: usam tudo de src/ exceto o Main do jogo.
SRC_LIB  = $(filter-out src/Main.cpp,$(SRC))
TARGET   = build/bolonhaemilano
TESTES   = build/testes

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(SRC) $(wildcard include/*.h)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

run: all
	./$(TARGET)

$(TESTES): $(SRC_LIB) tests/Testes.cpp tests/Testes.h $(wildcard include/*.h)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -I tests -o $@ $(SRC_LIB) tests/Testes.cpp

test: $(TESTES)
	./$(TESTES)

clean:
	rm -rf build
