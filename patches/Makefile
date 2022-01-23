.DEFAULT_GOAL = patch

include ../lib/detectos

ifeq ($(UNAME), Darwin)
	ARDUINO_CORE_DIR = ${HOME}/Library/Arduino15/packages/arduino/hardware/samd/1.8.6/cores/arduino
else
	ARDUINO_CORE_DIR= ../../../../AppData/Local/Arduino15/packages/arduino/hardware/samd/1.8.12/cores/arduino
endif

PATCH_FILES = $(wildcard *.cpp *.h)
DST_FILES = $(foreach file, $(PATCH_FILES), $(ARDUINO_CORE_DIR)/$(file))
ORIGINAL_FILES = $(foreach file, $(PATCH_FILES), $(ARDUINO_CORE_DIR)/$(file))

$(DST_FILES): $(ARDUINO_CORE_DIR)/%: %
	@echo patching $@
	@cp -rp $< $@

$(ORIGINAL_FILES): $(ARDUINO_CORE_DIR)/%: %
	@echo restoring $@
	@echo from original/$<
#	@cp -rp $< $@

patch: $(DST_FILES)

unpatch: $(ORIGINAL_FILES)
