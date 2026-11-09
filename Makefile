CXX ?= g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS := $(shell pkg-config --cflags --libs glfw3 gl) -lm

SRCS := $(wildcard src/*.c*)
HEADERS := $(wildcard include/*.h*)
SHADERS := $(wildcard shader/*)
TARGET := 3d-pong

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS) $(SHADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)