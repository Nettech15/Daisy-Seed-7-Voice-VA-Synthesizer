#include "midi_manager.h"
#include "vasynth.h"
#include "sequencer.h"

MidiManager::MidiManager(std::shared_ptr<VASynth> va_synth, std::unique_ptr<Sequencer> sequencer) :
    vasynth_{va_synth},
    sequencer_{std::move(sequencer)}
{
}

// midi handler
void MidiManager::HandleMidiMessage(MidiEvent m)
{
    switch(m.type)
    {
        case NoteOn:
        {
            NoteOnEvent p = m.AsNoteOn();
            if ((vasynth_->midi_channel_ == MIDI_CHANNEL_ALL) || (p.channel == vasynth_->midi_channel_))
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
            if ((vasynth_->midi_channel_ == MIDI_CHANNEL_ALL) || (p.channel == vasynth_->midi_channel_))
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
            if ((vasynth_->midi_channel_ == MIDI_CHANNEL_ALL) || (p.channel == vasynth_->midi_channel_))
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
					vasynth_->lfo_amp_ = ((float)p.value / 127.0f);
                    vasynth_->SetLFO();
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
									vasynth_->waveform_ = WAVE_TRI;
									break;
								case 1:
									vasynth_->waveform_ = WAVE_SAW;
									break;
								case 2:
									vasynth_->waveform_ = WAVE_SQUARE;
									break;
								case 3:
									vasynth_->waveform_ = WAVE_POLYBLEP_SAW;
									break;
							}
							vasynth_->SetWaveform();
							break;
						}
						case 2:
						{
							switch(p.value >> 5)
							{
								case 0:
									vasynth_->osc2_waveform_ = WAVE_TRI;
									break;
								case 1:
									vasynth_->osc2_waveform_ = WAVE_SAW;
									break;
								case 2:
									vasynth_->osc2_waveform_ = WAVE_SQUARE;
									break;
								case 3:
									vasynth_->osc2_waveform_ = WAVE_POLYBLEP_SAW;
									break;
							}
							vasynth_->SetWaveform();
							break;
						}
						case 3:
						{
							vasynth_->osc_mix_ = ((float)p.value / 127.0f);
							break;
						}
						case 4:
						{
							vasynth_->osc2_detune_ = ((float)p.value / 255.0f);
							break;
						}
						case 5:
						{
							vasynth_->osc2_transpose_ = (1.0f + ((float)p.value / 127.0f));
							break;
						}
						case 6:
						{
							vasynth_->filter_res_ = ((float)p.value / 127.0f);
                    		vasynth_->SetFilter();
							break;
						}
						case 7:
						{
							vasynth_->osc_pw_ = ((float)p.value / 255.0f);
							break;
						}
						case 8:
						{
							vasynth_->osc2_pw_ = ((float)p.value / 255.0f);
							break;
						}
						case 9:
						{
							vasynth_->eg_f_attack_ = ((float)p.value / 127.0f);
							vasynth_->SetEG();
							break;
						}
						case 10:
						{
							vasynth_->eg_f_decay_ = ((float)p.value / 127.0f);
							vasynth_->SetEG();
							break;
						}
						case 11:
						{
							vasynth_->eg_f_sustain_ = ((float)p.value / 127.0f);
							vasynth_->SetEG();
							break;
						}
						case 12:
						{
							vasynth_->eg_f_release_ = ((float)p.value / 127.0f);
							vasynth_->SetEG();
							break;
						}
						case 13:
						{
							vasynth_->eg_a_attack_ = ((float)p.value / 127.0f);
							vasynth_->SetEG();
							break;
						}
						case 14:
						{
							vasynth_->eg_a_decay_ = ((float)p.value / 127.0f);
							vasynth_->SetEG();
							break;
						}
						case 15:
						{
							vasynth_->eg_a_sustain_ = ((float)p.value / 127.0f);
							vasynth_->SetEG();
							break;
						}
						case 16:
						{
							vasynth_->eg_a_release_ = ((float)p.value / 127.0f);
							vasynth_->SetEG();
							break;
						}
						case 17:
						{
							vasynth_->vcf_kbd_follow_= ((float)p.value / 127.0f);
							break;
						}
						case 18:
						{
							vasynth_->env_kbd_follow_ = ((float)p.value / 127.0f);
							break;
						}
						case 19:
						{
							vasynth_->vel_select_ = p.value >> 5;
							break;
						}
						case 20:
						{
							vasynth_->eg_f_amount_ = ((float)p.value / 127.0f);
							break;
						}
						case 21:
						{
							vasynth_->lfo_freq_ = ((float)p.value / 127.0f);
							vasynth_->SetLFO();
							break;
						}
						case 22:
						{
							vasynth_->pwmlfo_freq_ = ((float)p.value / 127.0f);
							vasynth_->SetPWMLFO();
							break;
						}
						case 23:
						{
							vasynth_->pwmlfo_amp_ = ((float)p.value / 511.0f);
							vasynth_->SetPWMLFO();
							break;
						}
						case 24:
						{
							vasynth_->pwm2lfo_freq_ = ((float)p.value / 127.0f);
							vasynth_->SetPWM2LFO();
							break;
						}
						case 25:
						{
							vasynth_->pwm2lfo_amp_ = ((float)p.value / 511.0f);
							vasynth_->SetPWM2LFO();
							break;
						}
						case 26:
						{
							vasynth_->vcavcflfo_freq_ = ((float)p.value / 127.0f);
							vasynth_->SetVCAVCFLFO();
							break;
						}
						case 27:
						{
							vasynth_->vcavcflfo_amp_ = ((float)p.value / 127.0f);
							vasynth_->SetVCAVCFLFO();
							break;
						}
						case 28:
						{
							switch(p.value >> 5)
							{
								case 0:
									vasynth_->vcavcflfo_waveform_ = WAVE_TRI;
									break;
								case 1:
									vasynth_->vcavcflfo_waveform_ = WAVE_SAW;
									break;
								case 2:
									vasynth_->vcavcflfo_waveform_ = WAVE_SQUARE;
									break;
								case 3:
									vasynth_->vcavcflfo_waveform_ = WAVE_POLYBLEP_SAW;
									break;
							}
							vasynth_->SetVCAVCFLFO();
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
            if ((vasynth_->midi_channel_ == MIDI_CHANNEL_ALL) || (p.channel == vasynth_->midi_channel_))
			{
				if(p.program >= 29)
				{
					switch (p.program)
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
