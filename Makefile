# Project Name
TARGET = oscvasynth

USE_DAISYSP_LGPL = 1

# Sources
CPP_SOURCES = \
 src/main.cpp \
 src/vasynth.cpp \
 src/sequencer.cpp \
 src/midi_manager.cpp

# Library Locations
LIBDAISY_DIR = ../../libDaisy
DAISYSP_DIR = ../../DaisySP

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

