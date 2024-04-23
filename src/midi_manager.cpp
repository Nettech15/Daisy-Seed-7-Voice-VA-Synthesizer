#include "midi_manager.h"
#include "vasynth.h"
#include "sequencer.h"

#define MIDI_CHANNEL_ONE 0
#define MIDI_CHANNEL_ALL 17

MidiManager::MidiManager(std::shared_ptr<VASynth>   va_synth,
                         std::unique_ptr<Sequencer> sequencer)
: vasynth_{va_synth}, sequencer_{std::move(sequencer)}
{
}

float MidiManager::fixed2float(uint8_t value, const float max)
{
    return (float)value / max;
}
