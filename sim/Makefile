CLASSES := Boat Timer ConsoleLogger Rudder Gps Compass Sail Switches WindSensor
INCLUDES := Logger
LIB_CLASSES := Angle Globe Helm Tacker Navigator Captain Utility RotaryPID
LIB_INCLUDES := Position Utility Routes Windrange Waypoint
INC_PARAMS = $(foreach d, $(LIB_INCLUDES), -I../lib/$d) $(foreach e, $(INCLUDES), -I./$e)
CLASS_PARAMS = $(foreach p, $(CLASSES), ./$(p)/$(p).cpp -I./$(p))
LIB_CLASS_PARAMS = $(foreach p, $(LIB_CLASSES), ../lib/$(p)/$(p).cpp -I../lib/$(p))

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
	$(GCC) -std=c++11 -isystem /usr/local/lib/include $(SRCS) $(CLASS_PARAMS) $(LIB_CLASS_PARAMS) $(INC_PARAMS) -include stdint.h -lstdc++ -o $(EXE)

run: $(EXE)
	./$(EXE)

.PHONY: clean run

clean:
	$(RM) $(OBJS)
