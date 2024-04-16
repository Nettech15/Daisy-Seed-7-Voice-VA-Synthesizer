# Project Name
TARGET = oscvasynth

# Sources
CPP_SOURCES = main.cpp moogladder.cpp vasynth.cpp sequencer.cpp midi_manager.cpp

# Library Locations
LIBDAISY_DIR = ../../libDaisy
DAISYSP_DIR = ../../DaisySP

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

