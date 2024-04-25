#pragma once

#ifdef __cplusplus
#include "midi_manager.h"

class MidiTech_i2_61 : public MidiManager
{
  public:
    MidiTech_i2_61(std::shared_ptr<VASynth>   va_synth,
                   std::unique_ptr<Sequencer> sequencer);

    void HandleMidiMessage(MidiEvent m) override;

    void setControlParam(uint8_t data) { control_param_ = data; }

  private:
    uint8_t control_param_;
};
#endif
