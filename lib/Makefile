.SHELLFLAGS = -ec
.DEFAULT_GOAL = test

PROJECTS = Angle Globe Helm Rudder Sail Utility Tacker Navigator

test:
	for subdir in $(PROJECTS) ; do \
		$(MAKE) -C $$subdir ; \
	done

clean:
	for subdir in $(PROJECTS) ; do \
		$(MAKE) -C $$subdir clean ; \
	done

.PHONY : clean test
