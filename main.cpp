/*
This code is a modification of OscPocketD/VASynth, created by Staffan Melin, staffan.melin@oscillator.se.
It was later modified by (Steven @moonfriendsynth) to work with the Daisy Pod.
I (Christopher @Nettech15) have modified it further to run on the Daisy Seed without the use of the Daisy Pod board.
Synth parameters are now controlled by a Miditech i2-61 midi keyboard.
Feel free to copy, modify, and improve this code to match your equipment and sound requirements.
*/

#include "daisy_seed.h"
#include "daisysp.h"

#include "main.h"
#include "vasynth.h"
#include "sequencer.h"
#include "midi_manager.h"

using namespace daisy;
using namespace daisysp;

// globals
DaisySeed hardware;
MidiUsbHandler midi;
float sysSampleRate;
float sysCallbackRate;
extern uint8_t preset_max;
extern VASynthSetting preset_setting[PRESET_MAX];

float master_tune = 0.0f;

// sound
VASynth vasynth;
uint8_t gPlay = PLAY_ON;

// fx
DelayLine<float, DELAY_MAX> DSY_SDRAM_BSS delay_;

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
			
			out[n] = voice_left + in[n];
			out[n + 1] = voice_right + in[n + 1];
		} 
		else 
		{
			out[n] = 0;
			out[n + 1] = 0;		
		}		
	}
}

int main(void)
{
	// init hardware
	hardware.Init(true); // true = boost to 480MHz
	hardware.SetAudioBlockSize(1);

	sysSampleRate = hardware.AudioSampleRate();
	sysCallbackRate = hardware.AudioCallbackRate();

	// setup vasynth initial values
	vasynth.Init();

	// load the default patch
	vasynth.First(0);

	// Initialize USB Midi 
    MidiUsbHandler::Config midi_cfg;
    midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
	midi.Init(midi_cfg);

	// let everything settle
	System::Delay(100);
	
	// Stereo simulator
	delay_.Init();
	delay_.SetDelay(sysSampleRate * 0.01f);
    
    auto pSynth = std::make_shared<VASynth>(vasynth);
    
    Sequencer sequencer(pSynth);
    MidiManager midi_manager(pSynth, std::make_unique<Sequencer>(sequencer));

	// Start calling the audio callback
	hardware.StartAudio(AudioCallback);

	// Loop forever
	for(;;)
	{
        // handle MIDI Events
        midi.Listen();
        while(midi.HasEvents())
        {
            midi_manager.HandleMidiMessage(midi.PopEvent());
        }	
	}
}
