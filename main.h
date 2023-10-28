#pragma once

#include "daisysp.h"

// Set sequencer memory to SRAM
#define WRITE_READ_SRAM_ADDR 0x20000000

// settings
#define WAVEFORMS_MAX 8
#define VOICES_MAX 8
#define FILTER_CUTOFF_MAX 18000.0f

// play status
#define PLAY_OFF 0
#define PLAY_ON 1

// delay
#define DELAY_MAX static_cast<size_t>(48000 * DELAY_MAX_S)
#define DELAY_MAX_S 1.0f // delay max in seconds

// MIDI
#define MIDI_CHANNEL_ONE 0
#define MIDI_CHANNEL_ALL 17
#define MIDI_VELOCITY_MAX 127
extern uint8_t preset_number;
