# -*- MakeFile -*-
# Reference: https://makefiletutorial.com/
# This is a study project where each source C file is independently compilable

SHELL=/bin/bash
CC := gcc

BUILD_DIR := ./build
SRC_DIRS := ./src
SRCS := $(shell find $(SRC_DIRS) -name '*.c')
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Define an executable target for each source C file
EXES := $(SRCS:$(SRC_DIRS)/%.c=$(BUILD_DIR)/%)

.PHONY: all
all: $(EXES)

# Build executable file in the build directory for the source C file
$(BUILD_DIR)/%: $(SRC_DIRS)/%.c
	@mkdir -p $(dir $@)
	$(CC) -ansi $(INC_FLAGS) $(CFLAGS) $< -o $@

.PHONY: format
format:
	@clang-format --style=file -i --verbose $(SRC_DIRS)/*.c

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)
