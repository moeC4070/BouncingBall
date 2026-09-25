
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wunused-parameter -O2 -I./include -I./include/SDL3

# Target Binary Name (Inside the build folder)
BUILD_DIR := build
TARGET    := $(BUILD_DIR)/BouncingBall.exe

# 🚨 CORE LAYER ONLY: Removed libSDL3_image.a since SDL3 handles PNGs natively!
LDFLAGS   := ./lib/libSDL3.a

# --- WINDOWS HARDWARE SUB-DRIVERS ---
LDFLAGS   += -lwinmm -lgdi32 -limm32 -lole32 -loleaut32 -lversion -luuid -lsetupapi

# Project Source tracking
SRCS      := src/main.cpp

# Default Automation Target
all: $(TARGET)

# Run Target (Compiles first if needed, then executes)
run: $(TARGET)
	@echo Running $(TARGET)...
	@$(TARGET)

# Link/Compile Phase: Compiles src/main.cpp directly into the output binary
$(TARGET): $(SRCS)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

# Cleanup Workspace Artifacts safely
clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)

.PHONY: all run clean

