#pragma once

#ifdef __cplusplus
#include "daisy_seed.h"
#include <memory>

using namespace daisy;

class VASynth;
class Sequencer;

    
class MidiManager
{
public:
    MidiManager(std::shared_ptr<VASynth> va_synth, std::unique_ptr<Sequencer> sequencer);
    
    void HandleMidiMessage(MidiEvent m);
    
    void setControlParam(uint8_t data) { control_param_= data; }

private:
    std::shared_ptr<VASynth> vasynth_;
    std::unique_ptr<Sequencer> sequencer_;
    uint8_t control_param_;
};

#endif
