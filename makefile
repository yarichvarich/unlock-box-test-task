# Compiler settings
CXX := clang++
CXXFLAGS := -Wall -Wextra -std=c++17
LDFLAGS := 
TARGET := unlock_box

# Build directories
BUILD_DIR := build
DEBUG_DIR := $(BUILD_DIR)/debug
PROD_DIR := $(BUILD_DIR)/prod

# Source files
SRCS := main.cpp
OBJS := $(SRCS:.cpp=.o)
DEBUG_OBJS := $(addprefix $(DEBUG_DIR)/, $(OBJS))
PROD_OBJS := $(addprefix $(PROD_DIR)/, $(OBJS))

# Default build
all: debug prod

# Debug build(debug flags)
debug: CXXFLAGS += -g -O0
debug: $(DEBUG_DIR)/$(TARGET)

# Production build
prod: CXXFLAGS += -O3 -DNDEBUG
prod: $(PROD_DIR)/$(TARGET)

# Build rules
$(DEBUG_DIR)/$(TARGET): $(DEBUG_OBJS) | $(DEBUG_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(PROD_DIR)/$(TARGET): $(PROD_OBJS) | $(PROD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# Object file rules
$(DEBUG_DIR)/%.o: %.cpp | $(DEBUG_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PROD_DIR)/%.o: %.cpp | $(PROD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(DEBUG_DIR): | $(BUILD_DIR)
	mkdir -p $(DEBUG_DIR)

$(PROD_DIR): | $(BUILD_DIR)
	mkdir -p $(PROD_DIR)

# Clean all builds
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all debug prod clean