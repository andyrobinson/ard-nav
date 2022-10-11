CLASSES := Boat Timer ConsoleLogger Gps Compass Switches WindSensor MServo Arduino
INCLUDES := Logger
LIB_CLASSES := Angle Globe Helm Tacker Navigator Captain Utility RotaryPID Sail Rudder
LIB_INCLUDES := Position Utility Routes Windrange Waypoint IHelm
INC_PARAMS = $(foreach d, $(LIB_INCLUDES), -I../lib/$d) $(foreach e, $(INCLUDES), -I./$e)
CLASS_PARAMS = $(foreach p, $(CLASSES), ./$(p)/$(p).cpp -I./$(p))
LIB_CLASS_PARAMS = $(foreach p, $(LIB_CLASSES), ../lib/$(p)/$(p).cpp -I../lib/$(p))

SRCS := sim.cpp
OBJS = sim.o
EXE := sim.o

.SHELLFLAGS = -ec

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

# for the sub projects
SUB_PROJECTS = Boat Compass Gps Rudder Sail WindSensor

test:
	@for subdir in $(SUB_PROJECTS) ; do \
  	echo "\nMaking lib $$subdir" ; \
		$(MAKE) -C $$subdir ; \
	done

cleanall:
	for subdir in $(SUB_PROJECTS) ; do \
		$(MAKE) -C $$subdir clean ; \
	done
