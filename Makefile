# Project Name
TARGET = oscvasynth

USE_DAISYSP_LGPL = 1

MIDI_TECH_I2_61 = 1

# Sources
CPP_SOURCES = \
 src/main.cpp \
 src/vasynth.cpp \
 src/sequencer.cpp \
 src/midi_manager.cpp

ifdef MIDI_TECH_I2_61
 C_DEFS = -DMIDI_TECH_I2_61
 CPP_SOURCES += src/miditech_i2_61.cpp
endif

# Library Locations
LIBDAISY_DIR = ../../libDaisy
DAISYSP_DIR = ../../DaisySP

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
