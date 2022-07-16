.SHELLFLAGS = -ec
.DEFAULT_GOAL = compileandtest

PROJECTS = lib compass gps sail nav

compileandtest:
	@for subdir in $(PROJECTS) ; do \
	  echo "\n____________________________________________________" ; \
	  echo Making $$subdir ; \
		$(MAKE) -C $$subdir ; \
	done

clean:
	for subdir in $(PROJECTS) ; do \
		$(MAKE) -C $$subdir clean ; \
	done

.PHONY : clean compileandtest
