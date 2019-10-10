.DEFAULT_GOAL = copylibs

LIBS = Windsensor
# For libs that require all files, not just the standard .cpp and .h
LIBS_ALL =
DEST_DIR = ./libraries
LIB_DIR = ../lib

LIB_FILES = $(foreach lib, $(LIBS), $(LIB_DIR)/$(lib)/$(lib).cpp $(LIB_DIR)/$(lib)/$(lib).h)
LIB_ALL_FILES = $(foreach lib, $(LIBS_ALL), $(wildcard $(LIB_DIR)/$(lib)/*))
DST_FILES = $(patsubst $(LIB_DIR)/%, $(DEST_DIR)/%, $(LIB_FILES) $(LIB_ALL_FILES))

$(DST_FILES): $(DEST_DIR)/%: $(LIB_DIR)/%
	mkdir -p $(dir $@)
	@cp -rpv $< $@

copylibs: $(DST_FILES)

clean:
	$(RM) -r $(DEST_DIR)/*

.PHONY : clean
