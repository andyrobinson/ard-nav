include ../detectos

.DEFAULT_GOAL = test

BUILD_DIR = ./build
SRCS := $(shell find . -name "*.cpp")
INCLUDES = ../Utility/
INC_PARAMS=$(foreach d, $(INCLUDES), -I$d)

ifeq ($(UNAME), Darwin)
  GCC = g++-9 -std=c++11
	GTEST_DIR = /usr/local/lib
	GTEST_INCLUDE = /usr/local/include
	TEST_RUNNER = $(BUILD_DIR)/TestRunner
else
	GCC = g++
	GTEST_DIR = /home/Andy/googletest-1.10.x/googletest
	GTEST_INCLUDE = /home/Andy/googletest-1.10.x/googletest
	TEST_RUNNER = $(BUILD_DIR)/TestRunner.exe
endif

$(TEST_RUNNER): $(SRCS)
	$(MKDIR_P) $(dir $@)
	$(GCC) -I${GTEST_INCLUDE}/include $(INC_PARAMS) -pthread $(SRCS) $(GTEST_DIR)/libgtest.a -o $(TEST_RUNNER)

test: $(TEST_RUNNER)
	$(TEST_RUNNER)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

MKDIR_P ?= mkdir -p
