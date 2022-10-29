CLASSES := Boat Timer ConsoleLogger Gps Compass Switches WindSensor MServo Arduino
INCLUDES := Logger
LIB_CLASSES := Angle Globe Helm Tacker Navigator Captain Utility RotaryPID Sail Rudder
LIB_INCLUDES := Position Utility Routes Windrange Waypoint IHelm
CLASS_FILES := $(foreach p, $(CLASSES), ./$(p)/$(p).cpp) $(foreach p, $(LIB_CLASSES), ../lib/$(p)/$(p).cpp ../lib/$(p)/$(p).h)
INCLUDE_FILES := $(foreach p, $(CLASSES), ./$(p)/$(p).h)
INC_PARAMS = $(foreach d, $(LIB_INCLUDES), -I../lib/$d) $(foreach e, $(INCLUDES), -I./$e)
CLASS_PARAMS = $(foreach p, $(CLASSES), ./$(p)/$(p).cpp -I./$(p))
LIB_CLASS_PARAMS = $(foreach p, $(LIB_CLASSES), ../lib/$(p)/$(p).cpp -I../lib/$(p))

SRCS := sim.cpp $(CLASS_FILES) $(INCLUDE_FILES)
OBJS = sim.o

.SHELLFLAGS = -ec

.DEFAULT_GOAL = run

include ../lib/detectos

ifeq ($(UNAME), Darwin)
    GCC = g++-9 -std=c++11
else
	GCC = g++
endif

$(OBJS): $(SRCS)
	$(GCC) -std=c++11 -isystem /usr/local/lib/include sim.cpp $(CLASS_PARAMS) $(LIB_CLASS_PARAMS) $(INC_PARAMS) -include stdint.h -lstdc++ -o $(OBJS)

run: $(OBJS)
	./$(OBJS)

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
