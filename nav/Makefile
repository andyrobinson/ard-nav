.DEFAULT_GOAL = compile

include ../lib/detectos

LIBS = Angle Captain Compass DisplayLogger Globe Gps Helm Navigator Rudder Sail SelfTest Tacker Timer Utility WindSensor
# For libs that require all files, not just the standard .cpp and .h
LIBS_ALL = Adafruit_GPS_Library Logger Position
DEST_DIR = ./libraries
LIB_DIR = ../lib

ifeq ($(UNAME), Darwin)
	ARDUINO_LIB_DIR = ~/Documents/Arduino/libraries
else
	ARDUINO_LIB_DIR = ../../../Arduino/libraries
endif

LIB_FILES = $(foreach lib, $(LIBS), $(LIB_DIR)/$(lib)/$(lib).cpp $(LIB_DIR)/$(lib)/$(lib).h)
LIB_ALL_FILES = $(foreach lib, $(LIBS_ALL), $(wildcard $(LIB_DIR)/$(lib)/*))
DST_FILES = $(patsubst $(LIB_DIR)/%, $(DEST_DIR)/%, $(LIB_FILES) $(LIB_ALL_FILES))

$(DST_FILES): $(DEST_DIR)/%: $(LIB_DIR)/%
	mkdir -p $(dir $@)
	@cp -rpv $< $@

copylibs: $(DST_FILES)

clean:
	$(RM) -r $(DEST_DIR)/*
	$(RM) -r *.bin
	$(RM) -r *.elf

compile: $(notdir $(CURDIR)).ino $(DST_FILES)
	cp -rpv $(DEST_DIR)/* $(ARDUINO_LIB_DIR)
	arduino-cli compile --fqbn arduino:samd:arduino_zero_edbg ../$(notdir $(CURDIR))

$(notdir $(CURDIR)).*.bin: $(notdir $(CURDIR)).ino $(DST_FILES)
	$(MAKE) compile

upload: $(notdir $(CURDIR)).*.bin
	 arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:samd:arduino_zero_edbg ../$(notdir $(CURDIR))

.PHONY : clean
