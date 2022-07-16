.SHELLFLAGS = -ec
.DEFAULT_GOAL = test

PROJECTS = Angle Battery Captain Compass Helm Globe Navigator RotaryPID Rudder Sail Tacker Utility WindSensor

test:
	@for subdir in $(PROJECTS) ; do \
  	echo "\nMaking lib $$subdir" ; \
		$(MAKE) -C $$subdir ; \
	done

clean:
	for subdir in $(PROJECTS) ; do \
		$(MAKE) -C $$subdir clean ; \
	done

.PHONY : clean test
