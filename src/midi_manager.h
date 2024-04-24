#pragma once

#ifdef __cplusplus
#include "daisy_seed.h"
#include <memory>

#define MIDI_CHANNEL_ONE 0
#define MIDI_CHANNEL_ALL 17

using namespace daisy;

class VASynth;
class Sequencer;


class MidiManager
{
  public:
    MidiManager(std::shared_ptr<VASynth>   va_synth,
                std::unique_ptr<Sequencer> sequencer);

    ~MidiManager() = default;

    virtual void HandleMidiMessage(MidiEvent m) = 0;

  protected:
    std::shared_ptr<VASynth>   vasynth_;
    std::unique_ptr<Sequencer> sequencer_;

    float fixed2float(uint8_t value, const float max = 127.0f);
};

#endif
