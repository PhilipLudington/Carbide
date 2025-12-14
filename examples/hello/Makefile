# Hello Example Project Makefile
# Demonstrates Carbide build system patterns

PROJECT_NAME := hello
VERSION := 1.0.0

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
TEST_DIR := tests

# Output
TARGET := $(BUILD_DIR)/$(PROJECT_NAME)
TEST_TARGET := $(BUILD_DIR)/test_$(PROJECT_NAME)

# Platform Detection
ifeq ($(OS),Windows_NT)
    PLATFORM := windows
    TARGET := $(BUILD_DIR)/$(PROJECT_NAME).exe
    TEST_TARGET := $(BUILD_DIR)/test_$(PROJECT_NAME).exe
    RM := del /Q
    RMDIR := rmdir /S /Q
    MKDIR := mkdir
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        PLATFORM := macos
    else
        PLATFORM := linux
    endif
    RM := rm -f
    RMDIR := rm -rf
    MKDIR := mkdir -p
endif

# Compiler Detection
ifeq ($(origin CC),default)
    ifneq ($(shell which clang 2>/dev/null),)
        CC := clang
    else
        CC := gcc
    endif
endif

# Compiler Flags
CSTD := -std=c11
INCLUDES := -I$(INCLUDE_DIR)
WARNINGS := -Wall -Wextra -Wpedantic -Wshadow -Wconversion
WARNINGS += -Wdouble-promotion -Wformat=2 -Wundef -Wno-unused-parameter

ifeq ($(DEBUG),1)
    OPT := -O0 -g
    DEFINES := -DDEBUG
else
    OPT := -O2
    DEFINES := -DNDEBUG
endif

# Sanitizers
ifeq ($(SANITIZE),address)
    SANITIZE_FLAGS := -fsanitize=address -fno-omit-frame-pointer
else ifeq ($(SANITIZE),undefined)
    SANITIZE_FLAGS := -fsanitize=undefined
else ifeq ($(SANITIZE),all)
    SANITIZE_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer
endif

CFLAGS := $(CSTD) $(INCLUDES) $(WARNINGS) $(OPT) $(DEFINES) $(SANITIZE_FLAGS)
LDFLAGS := $(SANITIZE_FLAGS)

# Source Files
SRCS := $(wildcard $(SRC_DIR)/*.c)
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_OBJS := $(TEST_SRCS:$(TEST_DIR)/%.c=$(BUILD_DIR)/test_%.o)

MAIN_OBJ := $(BUILD_DIR)/main.o
LIB_OBJS := $(filter-out $(MAIN_OBJ),$(OBJS))

# Targets
.PHONY: all build check safety test format format-check clean info run dirs

all: build

# Create build directory
dirs:
	@$(MKDIR) $(BUILD_DIR)

build: dirs $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Static Analysis
check:
	@echo "Running clang-tidy..."
	@clang-tidy $(SRCS) -- $(CSTD) $(INCLUDES) $(DEFINES) 2>&1 || true
	@echo "Static analysis complete."

safety:
	@echo "Running security checks..."
	@clang-tidy $(SRCS) \
		-checks='-*,clang-analyzer-security.*,bugprone-*,cert-*' \
		-- $(CSTD) $(INCLUDES) $(DEFINES) 2>&1 || true
	@echo "Security checks complete."

# Formatting
format:
	@echo "Formatting source files..."
	@clang-format -i $(SRCS) $(wildcard $(INCLUDE_DIR)/$(PROJECT_NAME)/*.h) 2>/dev/null || true
	@echo "Formatting complete."

format-check:
	@echo "Checking format..."
	@clang-format --dry-run --Werror $(SRCS) $(wildcard $(INCLUDE_DIR)/$(PROJECT_NAME)/*.h) 2>/dev/null || true

# Testing
test: SANITIZE_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer
test: CFLAGS := $(CSTD) $(INCLUDES) $(WARNINGS) $(OPT) $(DEFINES) $(SANITIZE_FLAGS)
test: LDFLAGS := $(SANITIZE_FLAGS)
test: dirs $(TEST_TARGET)
	@echo "Running tests..."
	@./$(TEST_TARGET)

$(TEST_TARGET): $(LIB_OBJS) $(TEST_OBJS)
	$(CC) $(LIB_OBJS) $(TEST_OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run
run: build
	@./$(TARGET)

# Clean
clean:
ifeq ($(PLATFORM),windows)
	@if exist $(BUILD_DIR) $(RMDIR) $(BUILD_DIR)
else
	$(RMDIR) $(BUILD_DIR)
endif
	@echo "Clean complete."

# Info
info:
	@echo "Project: $(PROJECT_NAME) v$(VERSION)"
	@echo "Platform: $(PLATFORM)"
	@echo "Compiler: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "Sources: $(SRCS)"
	@echo "Target: $(TARGET)"
