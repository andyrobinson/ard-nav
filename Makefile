.SHELLFLAGS = -ec
.DEFAULT_GOAL = compileandtest

PROJECTS = lib compass gps hwint sail nav

compileandtest:
	for subdir in $(PROJECTS) ; do \
		$(MAKE) -C $$subdir ; \
	done

clean:
	for subdir in $(PROJECTS) ; do \
		$(MAKE) -C $$subdir clean ; \
	done

.PHONY : clean compileandtest
