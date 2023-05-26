Description of the project:

This code is a modification of OscPocketD/VASynth, created by Staffan Melin, staffan.melin@oscillator.se.
It was later modified by (Steven @moonfriendsynth) to work with the Daisy Pod.
I (Christopher @Nettech15) have modified it further to run on the Daisy Seed without the use of the Daisy Pod board.
Synth parameters are now controlled by a Miditech i2-61 midi keyboard.
Multiple Daisy Seeds will appear as USB-MIDI devices with the name "Daisy Seed Built in" and the device number.
Audio output/input is thru the built-in audio codec.

Feel free to copy, modify, and improve this code to match your equipment and sound requirements.
In the meantime, I will be actively working on implementing more features and fixing existing problems.

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
