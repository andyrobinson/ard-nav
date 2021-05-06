SRCS := $(shell find . -name "*.cpp") $(SOURCES)
OBJS =  $(shell find . -name "*.o")
EXE := sim.o

.DEFAULT_GOAL = run

include ../lib/detectos

ifeq ($(UNAME), Darwin)
    GCC = g++-9 -std=c++11
else
	GCC = g++
endif

$(EXE): $(SRCS)
	$(GCC) $(SRCS) -lstdc++ -o $(EXE)

run: $(EXE)
	./$(EXE)

.PHONY: clean run

clean:
	$(RM) $(OBJS)

