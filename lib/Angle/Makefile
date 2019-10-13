include ../detectos

.DEFAULT_GOAL = test

BUILD_DIR = ./build
TEST_RUNNER = $(BUILD_DIR)/TestRunner.exe

ifeq ($(UNAME), Darwin)
  GCC = g++-4.9
	GTEST_DIR = /usr/local/lib
	GTEST_INCLUDE = /usr/local/include
else
	GCC = g++
	GTEST_DIR = /home/Andy/googletest-1.10.x/googletest
	GTEST_INCLUDE = /home/Andy/googletest-1.10.x/googletest
endif

SRCS := $(shell find . -name "*.cpp")

$(TEST_RUNNER): $(SRCS)
	$(MKDIR_P) $(dir $@)
	g++ -isystem ${GTEST_INCLUDE}/include -pthread $(SRCS) $(GTEST_DIR)/libgtest.a -o $(TEST_RUNNER)

test: $(TEST_RUNNER)
	$(TEST_RUNNER)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

MKDIR_P ?= mkdir -p
