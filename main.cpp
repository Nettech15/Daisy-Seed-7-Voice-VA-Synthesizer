/*
This code is a modification of OscPocketD/VASynth, created by Staffan Melin, staffan.melin@oscillator.se.
It was later modified by (Steven @moonfriendsynth) to work with the Daisy Pod.
I (Christopher @Nettech15) have modified it further to run on the Daisy Seed without the use of the Daisy Pod board.
Synth parameters are now controlled by a Miditech i2-61 midi keyboard.
Multiple Daisy Seeds will appear as USB-MIDI devices with the name "Daisy Seed Built in" and the device number.
Audio output/input is thru the built-in audio codec.

Specifications:

- 8 voice polyphonic.
- Dual oscillator per voice.
- VCA per voice. 
- VCF (4-pole Resonant low-pass) per voice.
- Separate ADSR's for VCA and VCF
- MIDI Editable parameters. 
- Ten Preset Patches.
- Ten User-Programmable Patches using QSPI Flash memory storage.
- Waveform select for both oscillators; Sine/Triangle/Saw/Square/Ramp/Polyblep-Tri/Polyblep-Saw/Polyblep-Square
- Oscillator (1/2) mix. 
- Oscillator 2 De-tune. 
- Scale for Oscillator 2. 
- LFO for Pitch Modulation Wheel.
- LFO for Pulse Width Modulation.
- LFO for VCF Modulation.
- LFO for VCA Modulation.
- Keyboard velocity routable to VCA and/or VCF. 
- VCF envelope level. 
- Stereo simulation effect.

Feel free to copy, modify, and improve this code to match your equipment and sound requirements.
In the meantime, I will be actively working on implementing more features and fixing existing problems.
*/

#include "daisy_seed.h"
#include "daisysp.h"

#include "main.h"
#include "vasynth.h"

using namespace daisy;
using namespace daisysp;

// globals
DaisySeed hardware;
MidiUsbHandler midi;
float sysSampleRate;
float sysCallbackRate;
extern uint8_t preset_max;
extern VASynthSetting preset_setting[PRESET_MAX];

uint8_t param_;
float pitch_bend = 1.0f;
float master_tune = 0.0f;

// sound
VASynth vasynth;
uint8_t gPlay = PLAY_ON;

// audio callback

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{	
	float voice_left, voice_right;
	
	for (size_t n = 0; n < size; n += 2)
	{	
		if (gPlay == PLAY_ON)
		{			
			vasynth.Process(&voice_left, &voice_right);
			
			out[n] = voice_left + in[n];;
			out[n + 1] = voice_right + in[n + 1];;	
		} 
		else 
		{
			out[n] = 0;
			out[n + 1] = 0;		
		}		
	}
}

// midi handler
void HandleMidiMessage(MidiEvent m)
{
    switch(m.type)
    {
        case NoteOn:
        {
            NoteOnEvent p = m.AsNoteOn();
            if ((vasynth.midi_channel_ == MIDI_CHANNEL_ALL) || (p.channel == vasynth.midi_channel_))
			{
				vasynth.NoteOn(p.note, p.velocity);
				hardware.SetLed(true);
			}
	        break;
        }
        case NoteOff:
        {
            NoteOnEvent p = m.AsNoteOn();
            if ((vasynth.midi_channel_ == MIDI_CHANNEL_ALL) || (p.channel == vasynth.midi_channel_))
			{
				vasynth.NoteOff(p.note);
				hardware.SetLed(false);
			}			
	        break;
        } 
		case PitchBend:
        {
            PitchBendEvent p = m.AsPitchBend();
            if ((vasynth.midi_channel_ == MIDI_CHANNEL_ALL) || (p.channel == vasynth.midi_channel_))
			{
				vasynth.PitchBend(p.value);
			}			
	        break;
        } 
        case ControlChange:
        {
            ControlChangeEvent p = m.AsControlChange();
            switch(p.control_number)
            {
				case 1: // Modulation Wheel
					vasynth.lfo_amp_ = ((float)p.value / 127.0f);
                    vasynth.SetLFO();
                    break;
				case 7: // Data Slider Default (Volume)
					switch(param_)
					{
						case 0: // This is set as the default parameter
						{
							master_tune = 1.0f - ((float)p.value / 64.0f);
							break;
						}
						case 1:
						{
							vasynth.waveform_ = p.value >> 4;
							vasynth.SetWaveform();
							break;
						}
						case 2:
						{
							vasynth.osc2_waveform_ = p.value >> 4;
							vasynth.SetWaveform();
							break;
						}
						case 3:
						{
							vasynth.osc_mix_ = ((float)p.value / 127.0f);
							break;
						}
						case 4:
						{
							vasynth.osc2_detune_ = ((float)p.value / 255.0f);
							break;
						}
						case 5:
						{
							vasynth.osc2_transpose_ = (1.0f + ((float)p.value / 127.0f));
							break;
						}
						case 6:
						{
							vasynth.filter_res_ = ((float)p.value / 127.0f);
                    		vasynth.SetFilter();
							break;
						}
						case 7:
						{
							vasynth.osc_pw_ = ((float)p.value / 255.0f);
							break;
						}
						case 8:
						{
							vasynth.osc2_pw_ = ((float)p.value / 255.0f);
							break;
						}
						case 9:
						{
							vasynth.eg_f_attack_ = ((float)p.value / 127.0f);
							vasynth.SetEG();
							break;
						}
						case 10:
						{
							vasynth.eg_f_decay_ = ((float)p.value / 127.0f);
							vasynth.SetEG();
							break;
						}
						case 11:
						{
							vasynth.eg_f_sustain_ = ((float)p.value / 127.0f);
							vasynth.SetEG();
							break;
						}
						case 12:
						{
							vasynth.eg_f_release_ = ((float)p.value / 127.0f);
							vasynth.SetEG();
							break;
						}
						case 13:
						{
							vasynth.eg_a_attack_ = ((float)p.value / 127.0f);
							vasynth.SetEG();
							break;
						}
						case 14:
						{
							vasynth.eg_a_decay_ = ((float)p.value / 127.0f);
							vasynth.SetEG();
							break;
						}
						case 15:
						{
							vasynth.eg_a_sustain_ = ((float)p.value / 127.0f);
							vasynth.SetEG();
							break;
						}
						case 16:
						{
							vasynth.eg_a_release_ = ((float)p.value / 127.0f);
							vasynth.SetEG();
							break;
						}
						case 17:
						{
							break;
						}
						case 18:
						{
							break;
						}
						case 19:
						{
							vasynth.vel_select_ = p.value >> 5;
							break;
						}
						case 20:
						{
							vasynth.eg_f_amount_ = ((float)p.value / 127.0f);
							break;
						}
						case 21:
						{
							vasynth.lfo_freq_ = ((float)p.value / 127.0f);
							vasynth.SetLFO();
							break;
						}
						case 22:
						{
							vasynth.pwmlfo_freq_ = ((float)p.value / 127.0f);
							vasynth.SetPWMLFO();
							break;
						}
						case 23:
						{
							vasynth.pwmlfo_amp_ = ((float)p.value / 255.0f);
							vasynth.SetPWMLFO();
							break;
						}
						case 24:
						{
							vasynth.vcalfo_freq_ = ((float)p.value / 127.0f);
							vasynth.SetVCALFO();
							break;
						}
						case 25:
						{
							vasynth.vcalfo_amp_ = ((float)p.value / 127.0f);
							vasynth.SetVCALFO();
							break;
						}
						case 26:
						{
							vasynth.vcflfo_freq_ = ((float)p.value / 127.0f);
							vasynth.SetVCFLFO();
							break;
						}
						case 27:
						{
							vasynth.vcflfo_amp_ = ((float)p.value / 127.0f);
							vasynth.SetVCFLFO();
							break;
						}
						case 28:
						{
							vasynth.vcalfo_waveform_ = p.value >> 4;
							vasynth.vcflfo_waveform_ = p.value >> 4;
							vasynth.SetVCALFO();
							vasynth.SetVCFLFO();
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
            if ((vasynth.midi_channel_ == MIDI_CHANNEL_ALL) || (p.channel == vasynth.midi_channel_))
			{
				if(p.program >= 29)
				{
					switch (p.program)
					{
						case 29:
						{
							vasynth.SaveToLive(&preset_setting[0]);
							break;
						}
						case 30:
						{
							vasynth.SaveToLive(&preset_setting[1]);
							break;
						}
						case 31:
						{
							vasynth.SaveToLive(&preset_setting[2]);
							break;
						}
						case 32:
						{
							vasynth.SaveToLive(&preset_setting[3]);
							break;
						}
						case 33:
						{
							vasynth.SaveToLive(&preset_setting[4]);
							break;
						}
						case 34:
						{
							vasynth.SaveToLive(&preset_setting[5]);
							break;
						}
						case 35:
						{
							vasynth.SaveToLive(&preset_setting[6]);
							break;
						}
						case 36:
						{
							vasynth.SaveToLive(&preset_setting[7]);
							break;
						}
						case 37:
						{
							vasynth.SaveToLive(&preset_setting[8]);
							break;
						}
						case 38:
						{
							vasynth.SaveToLive(&preset_setting[9]);
							break;
						}
						case 39:
						{
							vasynth.FlashLoad(0);
							break;
						}
						case 40:
						{
							vasynth.FlashLoad(1);
							break;
						}
						case 41:
						{
							vasynth.FlashLoad(2);
							break;
						}
						case 42:
						{
							vasynth.FlashLoad(3);
							break;
						}
						case 43:
						{
							vasynth.FlashLoad(4);
							break;
						}
						case 44:
						{
							vasynth.FlashLoad(5);
							break;
						}
						case 45:
						{
							vasynth.FlashLoad(6);
							break;
						}
						case 46:
						{
							vasynth.FlashLoad(7);
							break;
						}
						case 47:
						{
							vasynth.FlashLoad(8);
							break;
						}
						case 48:
						{
							vasynth.FlashLoad(9);
							break;
						}
						case 49:
						{
							vasynth.FlashSave(0);
							break;
						}
						case 50:
						{
							vasynth.FlashSave(1);
							break;
						}
						case 51:
						{
							vasynth.FlashSave(2);
							break;
						}
						case 52:
						{
							vasynth.FlashSave(3);
							break;
						}
						case 53:
						{
							vasynth.FlashSave(4);
							break;
						}
						case 54:
						{
							vasynth.FlashSave(5);
							break;
						}
						case 55:
						{
							vasynth.FlashSave(6);
							break;
						}
						case 56:
						{
							vasynth.FlashSave(7);
							break;
						}
						case 57:
						{
							vasynth.FlashSave(8);
							break;
						}
						case 58:
						{
							vasynth.FlashSave(9);
							break;
						}
					}
				}	
				else
				{
					vasynth.ProgramChange(p.program);
					break;				
				}
			}    
        }
        default: break;
    }
}

int main(void)
{
	// init hardware
	hardware.Init(true); // true = boost to 480MHz

	sysSampleRate = hardware.AudioSampleRate();
	sysCallbackRate = hardware.AudioCallbackRate();

	// init qspi flash for saving and loading patches
	QSPIHandle::Config qspi_config;
	qspi_config.device = QSPIHandle::Config::Device::IS25LP064A;
	qspi_config.mode   = QSPIHandle::Config::Mode::MEMORY_MAPPED;
	qspi_config.pin_config.io0 = {DSY_GPIOF, 8};
	qspi_config.pin_config.io1 = {DSY_GPIOF, 9};
	qspi_config.pin_config.io2 = {DSY_GPIOF, 7};
	qspi_config.pin_config.io3 = {DSY_GPIOF, 6};
	qspi_config.pin_config.clk = {DSY_GPIOF, 10};
	qspi_config.pin_config.ncs = {DSY_GPIOG, 6};
	hardware.qspi.Init(qspi_config);

	// setup incl default values
	vasynth.First();

	// Initialize USB Midi 
    MidiUsbHandler::Config midi_cfg;
    midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
	midi.Init(midi_cfg);

	// let everything settle
	System::Delay(100);
	
	// Start calling the audio callback
	hardware.StartAudio(AudioCallback);

	// Loop forever
	for(;;)
	{
        // handle MIDI Events
        midi.Listen();
        while(midi.HasEvents())
        {
            HandleMidiMessage(midi.PopEvent());
        }	
	}
}
