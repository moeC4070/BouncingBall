
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wunused-parameter -O2 $(shell pkg-config --cflags sdl3)

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/BouncingBall

LDFLAGS   := $(shell pkg-config --libs sdl3)
SRCS      := src/main.cpp

all: $(TARGET)

run: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all run clean

