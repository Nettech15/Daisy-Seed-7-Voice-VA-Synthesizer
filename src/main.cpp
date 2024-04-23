/*
This code is a modification of OscPocketD/VASynth, created by Staffan Melin, staffan.melin@oscillator.se.
It was later modified by (Steven @moonfriendsynth) to work with the Daisy Pod.
I (Christopher @Nettech15) have modified it further to run on the Daisy Seed without the use of the Daisy Pod board.
Synth parameters are now controlled by a Miditech i2-61 midi keyboard.
Feel free to copy, modify, and improve this code to match your equipment and sound requirements.
*/

#include "daisy_seed.h"
#include "daisysp.h"

#include "vasynth.h"
#include "sequencer.h"
#include "midi_manager.h"

using namespace daisy;
using namespace daisysp;

// globals
DaisySeed      hardware;
MidiUsbHandler midi;

// sound
std::shared_ptr<VASynth> vasynth;

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    float voice_left, voice_right;

    for(size_t n = 0; n < size; n += 2)
    {
        if(vasynth->IsPlaying())
        {
            vasynth->Process(&voice_left, &voice_right);

            out[n]     = voice_left + in[n];
            out[n + 1] = voice_right + in[n + 1];
        }
        else
        {
            out[n]     = 0;
            out[n + 1] = 0;
        }
    }
}

int main(void)
{
    // init hardware
    hardware.Init(true); // true = boost to 480MHz
    hardware.SetAudioBlockSize(1);

    // sysCallbackRate = hardware.AudioCallbackRate();

    vasynth = std::make_shared<VASynth>(hardware.AudioSampleRate());

    // Initialize USB Midi
    MidiUsbHandler::Config midi_cfg;
    midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
    midi.Init(midi_cfg);

    // let everything settle
    System::Delay(100);

    Sequencer   sequencer(vasynth);
    MidiManager midi_manager(vasynth, std::make_unique<Sequencer>(sequencer));

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
