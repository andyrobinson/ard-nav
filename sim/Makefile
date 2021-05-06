SRCS := $(shell find . -name "*.cpp") $(SOURCES)
OBJS =  $(shell find . -name "*.o")

.DEFAULT_GOAL = run

include ../lib/detectos

ifeq ($(UNAME), Darwin)
    GCC = g++-9 -std=c++11
else
	GCC = g++
endif

sim.o: $(SRCS)
	$(GCC) $(SRCS) -lstdc++ -o sim.o

run: sim.o
	./sim.o

.PHONY: clean run

clean:
	$(RM) $(OBJS)

