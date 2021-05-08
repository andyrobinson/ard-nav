CLASSES := ConsoleLogger Boat
LIB_INCLUDES := Logger Position
INC_PARAMS = $(foreach d, $(LIB_INCLUDES), -I../lib/$d)
CLASS_PARAMS = $(foreach p, $(CLASSES), ./$(p)/$(p).cpp -I./$(p))

SRCS := sim.cpp
OBJS = sim.o
EXE := sim.o

.DEFAULT_GOAL = run

include ../lib/detectos

ifeq ($(UNAME), Darwin)
    GCC = g++-9 -std=c++11
else
	GCC = g++
endif

$(EXE): $(SRCS)
	$(GCC) $(SRCS) $(CLASS_PARAMS) $(INC_PARAMS) -lstdc++ -o $(EXE)

run: $(EXE)
	./$(EXE)

.PHONY: clean run

clean:
	$(RM) $(OBJS)

