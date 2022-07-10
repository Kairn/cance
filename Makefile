# -*- MakeFile -*-
# Reference: https://makefiletutorial.com/

CC := gcc

BUILD_DIR := ./build
SRC_DIRS := ./src

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)
