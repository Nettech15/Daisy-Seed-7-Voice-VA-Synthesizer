#include "miditech_i2_61.h"
#include "vasynth.h"
#include "sequencer.h"

MidiTech_i2_61::MidiTech_i2_61(std::shared_ptr<VASynth>   va_synth,
                               std::unique_ptr<Sequencer> sequencer)
: MidiManager(va_synth, std::move(sequencer))
{
}

// midi handler
void MidiTech_i2_61::HandleMidiMessage(MidiEvent m)
{
    uint8_t midi_channel = vasynth_->getMidiChannel();

    switch(m.type)
    {
        case NoteOn:
        {
            NoteOnEvent p = m.AsNoteOn();
            if((midi_channel == MIDI_CHANNEL_ALL)
               || (p.channel == midi_channel))
            {
                if(sequencer_->get_mode() == Sequencer::Mode::RecordEnable)
                {
                    sequencer_->record((p.note | 0x80), p.velocity);
                }
                vasynth_->NoteOn(p.note, p.velocity);
                // hardware.SetLed(true);
            }
            break;
        }
        case NoteOff:
        {
            NoteOnEvent p = m.AsNoteOn();
            if((midi_channel == MIDI_CHANNEL_ALL)
               || (p.channel == midi_channel))
            {
                if(sequencer_->get_mode() == Sequencer::Mode::RecordEnable)
                {
                    sequencer_->record(p.note, 0);
                }
                vasynth_->NoteOff(p.note);
                // hardware.SetLed(false);
            }
            break;
        }
        case PitchBend:
        {
            PitchBendEvent p = m.AsPitchBend();
            if((midi_channel == MIDI_CHANNEL_ALL)
               || (p.channel == midi_channel))
            {
                vasynth_->PitchBend(p.value);
            }
            break;
        }
        case ControlChange:
        {
            ControlChangeEvent p = m.AsControlChange();
            switch(p.control_number)
            {
                case 1: // Modulation Wheel
                    vasynth_->SetLFO(fixed2float(p.value), VASynth::Param::Amp);
                    break;
                case 7: // Data Slider Default (Volume)
                    switch(control_param_)
                    {
                        case 0: // This is set as the default parameter
                        {
                            vasynth_->SetMasterTune(p.value);
                            break;
                        }
                        case 1:
                        {
                            switch(p.value >> 5)
                            {
                                case 0:
                                    vasynth_->SetWaveform(
                                        daisysp::Oscillator::WAVE_TRI,
                                        VASynth::OSCid::OSC1);
                                    break;
                                case 1:
                                    vasynth_->SetWaveform(
                                        daisysp::Oscillator::WAVE_SAW,
                                        VASynth::OSCid::OSC1);
                                    break;
                                case 2:
                                    vasynth_->SetWaveform(
                                        daisysp::Oscillator::WAVE_SQUARE,
                                        VASynth::OSCid::OSC1);
                                    break;
                                case 3:
                                    vasynth_->SetWaveform(
                                        daisysp::Oscillator::WAVE_POLYBLEP_SAW,
                                        VASynth::OSCid::OSC1);
                                    break;
                            }
                            break;
                        }
                        case 2:
                        {
                            switch(p.value >> 5)
                            {
                                case 0:
                                    vasynth_->SetWaveform(
                                        daisysp::Oscillator::WAVE_TRI,
                                        VASynth::OSCid::OSC2);
                                    break;
                                case 1:
                                    vasynth_->SetWaveform(
                                        daisysp::Oscillator::WAVE_SAW,
                                        VASynth::OSCid::OSC2);
                                    break;
                                case 2:
                                    vasynth_->SetWaveform(
                                        daisysp::Oscillator::WAVE_SQUARE,
                                        VASynth::OSCid::OSC2);
                                    break;
                                case 3:
                                    vasynth_->SetWaveform(
                                        daisysp::Oscillator::WAVE_POLYBLEP_SAW,
                                        VASynth::OSCid::OSC2);
                                    break;
                            }
                            break;
                        }
                        case 3:
                        {
                            vasynth_->SetOscillatorMix(fixed2float(p.value));
                            break;
                        }
                        case 4:
                        {
                            vasynth_->SetOsc2Detune(
                                fixed2float(p.value, 255.0f));
                            break;
                        }
                        case 5:
                        {
                            vasynth_->SetOsc2Transpose(1.0f
                                                       + fixed2float(p.value));
                            break;
                        }
                        case 6:
                        {
                            vasynth_->SetFilterResonance(fixed2float(p.value));
                            break;
                        }
                        case 7:
                        {
                            vasynth_->SetOscPw(fixed2float(p.value, 255.0f));
                            break;
                        }
                        case 8:
                        {
                            vasynth_->SetOsc2Pw(fixed2float(p.value, 255.0f));
                            break;
                        }
                        case 9:
                        {
                            vasynth_->SetEnvelopeGenerator(
                                fixed2float(p.value),
                                VASynth::EnvId::EnvFrequency,
                                VASynth::AdsrParam::ADSR_SEG_ATTACK);
                            break;
                        }
                        case 10:
                        {
                            vasynth_->SetEnvelopeGenerator(
                                fixed2float(p.value),
                                VASynth::EnvId::EnvFrequency,
                                VASynth::AdsrParam::ADSR_SEG_DECAY);
                            break;
                        }
                        case 11:
                        {
                            vasynth_->SetEnvelopeGenerator(
                                fixed2float(p.value),
                                VASynth::EnvId::EnvFrequency,
                                VASynth::AdsrParam::ADSR_SEG_SUSTAIN);
                            break;
                        }
                        case 12:
                        {
                            vasynth_->SetEnvelopeGenerator(
                                fixed2float(p.value),
                                VASynth::EnvId::EnvFrequency,
                                VASynth::AdsrParam::ADSR_SEG_RELEASE);
                            break;
                        }
                        case 13:
                        {
                            vasynth_->SetEnvelopeGenerator(
                                fixed2float(p.value),
                                VASynth::EnvId::EnvAmplitude,
                                VASynth::AdsrParam::ADSR_SEG_ATTACK);
                            break;
                        }
                        case 14:
                        {
                            vasynth_->SetEnvelopeGenerator(
                                fixed2float(p.value),
                                VASynth::EnvId::EnvAmplitude,
                                VASynth::AdsrParam::ADSR_SEG_DECAY);
                            break;
                        }
                        case 15:
                        {
                            vasynth_->SetEnvelopeGenerator(
                                fixed2float(p.value),
                                VASynth::EnvId::EnvAmplitude,
                                VASynth::AdsrParam::ADSR_SEG_SUSTAIN);
                            break;
                        }
                        case 16:
                        {
                            vasynth_->SetEnvelopeGenerator(
                                fixed2float(p.value),
                                VASynth::EnvId::EnvAmplitude,
                                VASynth::AdsrParam::ADSR_SEG_RELEASE);

                            break;
                        }
                        case 17:
                        {
                            vasynth_->SetVcfKbdFollow(fixed2float(p.value));
                            break;
                        }
                        case 18:
                        {
                            vasynth_->SetEnvKbdFollow(fixed2float(p.value));
                            break;
                        }
                        case 19:
                        {
                            vasynth_->SetVelSelect(p.value >> 5);
                            break;
                        }
                        case 20:
                        {
                            vasynth_->SetFreqEnvGenAmount(fixed2float(p.value));
                            break;
                        }
                        case 21:
                        {
                            vasynth_->SetLFO(fixed2float(p.value),
                                             VASynth::Param::Frequency);
                            break;
                        }
                        case 22:
                        {
                            vasynth_->SetPWMLFO(fixed2float(p.value),
                                                VASynth::Param::Frequency);
                            break;
                        }
                        case 23:
                        {
                            vasynth_->SetPWMLFO(fixed2float(p.value, 511.0f),
                                                VASynth::Param::Amp);
                            break;
                        }
                        case 24:
                        {
                            vasynth_->SetPWM2LFO(fixed2float(p.value),
                                                 VASynth::Param::Frequency);
                            break;
                        }
                        case 25:
                        {
                            vasynth_->SetPWM2LFO(fixed2float(p.value, 511.0f),
                                                 VASynth::Param::Amp);
                            break;
                        }
                        case 26:
                        {
                            vasynth_->SetVCAVCFLFO(fixed2float(p.value),
                                                   VASynth::Param::Frequency);
                            break;
                        }
                        case 27:
                        {
                            vasynth_->SetVCAVCFLFO(fixed2float(p.value),
                                                   VASynth::Param::Amp);
                            break;
                        }
                        case 28:
                        {
                            switch(p.value >> 5)
                            {
                                case 0:
                                    vasynth_->SetVCAVCFLFO(
                                        daisysp::Oscillator::WAVE_TRI,
                                        VASynth::Param::Waveform);
                                    break;
                                case 1:
                                    vasynth_->SetVCAVCFLFO(
                                        daisysp::Oscillator::WAVE_SAW,
                                        VASynth::Param::Waveform);
                                    break;
                                case 2:
                                    vasynth_->SetVCAVCFLFO(
                                        daisysp::Oscillator::WAVE_SQUARE,
                                        VASynth::Param::Waveform);
                                    break;
                                case 3:
                                    vasynth_->SetVCAVCFLFO(
                                        daisysp::Oscillator::WAVE_POLYBLEP_SAW,
                                        VASynth::Param::Waveform);
                                    break;
                            }
                            break;
                        }
                    }
                    break;
                default: break;
            }
            break;
        }
        case ProgramChange:
        {
            ProgramChangeEvent p = m.AsProgramChange();
            if((midi_channel == MIDI_CHANNEL_ALL)
               || (p.channel == midi_channel))
            {
                if(p.program >= 29)
                {
                    switch(p.program)
                    {
                        case 35:
                        {
                            // Sequencer Mode Record
                            sequencer_->record_mode(true);
                            break;
                        }
                        case 36:
                        {
                            // Sequencer Mode Record End
                            sequencer_->record_mode(false);
                            break;
                        }
                        case 37:
                        {
                            // Sequencer Mode Stop
                            sequencer_->stop_mode();
                            break;
                        }
                        case 38:
                        {
                            sequencer_->play_mode();
                            break;
                        }
                        case 39:
                        {
                            vasynth_->FlashLoad(0);
                            break;
                        }
                        case 40:
                        {
                            vasynth_->FlashLoad(1);
                            break;
                        }
                        case 41:
                        {
                            vasynth_->FlashLoad(2);
                            break;
                        }
                        case 42:
                        {
                            vasynth_->FlashLoad(3);
                            break;
                        }
                        case 43:
                        {
                            vasynth_->FlashLoad(4);
                            break;
                        }
                        case 44:
                        {
                            vasynth_->FlashLoad(5);
                            break;
                        }
                        case 45:
                        {
                            vasynth_->FlashLoad(6);
                            break;
                        }
                        case 46:
                        {
                            vasynth_->FlashLoad(7);
                            break;
                        }
                        case 47:
                        {
                            vasynth_->FlashLoad(8);
                            break;
                        }
                        case 48:
                        {
                            vasynth_->FlashLoad(9);
                            break;
                        }
                        case 49:
                        {
                            vasynth_->FlashSave(0);
                            break;
                        }
                        case 50:
                        {
                            vasynth_->FlashSave(1);
                            break;
                        }
                        case 51:
                        {
                            vasynth_->FlashSave(2);
                            break;
                        }
                        case 52:
                        {
                            vasynth_->FlashSave(3);
                            break;
                        }
                        case 53:
                        {
                            vasynth_->FlashSave(4);
                            break;
                        }
                        case 54:
                        {
                            vasynth_->FlashSave(5);
                            break;
                        }
                        case 55:
                        {
                            vasynth_->FlashSave(6);
                            break;
                        }
                        case 56:
                        {
                            vasynth_->FlashSave(7);
                            break;
                        }
                        case 57:
                        {
                            vasynth_->FlashSave(8);
                            break;
                        }
                        case 58:
                        {
                            vasynth_->FlashSave(9);
                            break;
                        }
                    }
                }
                else
                {
                    setControlParam(p.program);
                    break;
                }
            }
        }
        default: break;
    }
}
