.DEFAULT_GOAL = test

BUILD_DIR = ./build
TEST_RUNNER = $(BUILD_DIR)/TestRunner.exe
GTEST_DIR = /home/Andy/googletest-1.10.x/googletest
SRCS := $(shell find . -name "*.cpp")

$(TEST_RUNNER): $(SRCS)
	$(MKDIR_P) $(dir $@)
	g++ -isystem ${GTEST_DIR}/include -pthread $(SRCS) $(GTEST_DIR)/libgtest.a -o $(TEST_RUNNER)

test: $(TEST_RUNNER)
	$(TEST_RUNNER)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

MKDIR_P ?= mkdir -p
