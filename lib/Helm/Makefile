.DEFAULT_GOAL = test

include ../detectos

BUILD_DIR = ./build
TEST_RUNNER = $(BUILD_DIR)/TestRunner.exe
SRCS := $(shell find . -name "*.cpp") ../Angle/Angle.cpp
INCLUDES = ../Angle/ ../Utility/ ./stubs/
INC_PARAMS=$(foreach d, $(INCLUDES), -I$d)

ifeq ($(UNAME), Darwin)
  GCC = g++-9 -std=c++11
	GTEST_DIR = /usr/local/lib
	GTEST_INCLUDE = /usr/local/include
else
	GCC = g++
	GTEST_DIR = /home/Andy/googletest-1.10.x/googletest
	GTEST_INCLUDE = /home/Andy/googletest-1.10.x/googletest
endif

$(TEST_RUNNER): $(SRCS)
	$(MKDIR_P) $(dir $@)
	$(GCC) -isystem ${GTEST_DIR}/include $(INC_PARAMS) -pthread $(SRCS) $(GTEST_DIR)/libgtest.a -o $(TEST_RUNNER)

test: $(TEST_RUNNER)
	$(TEST_RUNNER)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

MKDIR_P ?= mkdir -p
