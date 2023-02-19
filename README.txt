Description of the project:

This code is a modification of OscPocketD/VASynth, created by Staffan Melin, staffan.melin@oscillator.se.
It was later modified by (Steven @moonfriendsynth) to work with the Daisy Pod.
I (Christopher @Nettech15) have modified it further to run on the Daisy Seed without the use of the Daisy Pod board.
Synth parameters are now controlled by a Miditech i2-61 midi keyboard.
Multiple Daisy Seeds will appear as USB-MIDI devices with the name "Daisy Seed Built in" and the device number.
Audio output/input is thru the built-in audio codec.

+ Upgraded DaisySP with more efficient MoogLadder Filter code.
+ Added synthesized PW square waves using the VASynth::RAMP wave.
+ Added Param Switch and Data Entry Slider. 
+ Added QSPI storage for ten user patches. Made ten selectable presets.
+ Added Audio Input PassThru and MIDI indicator. 
+ Added USB-MIDI input.
+ Added Stereo Simulator. 
+ Added Moogladder filter. 
+ Added Dynamic Voice Allocation. 
+ Added keyboard velocity control to VCA and VCF.

- Removed Circular Voice Allocation.
- Removed Portamento.
- Removed SVF.
- Removed Reverb.
- Removed Serial MIDI input.
- Removed all Daisy Pod related code (Knobs, Switches, and Encoder).

Feel free to copy, modify, and improve this code to match your equipment and sound requirements.
In the meantime, I will be actively working on implementing more features and fixing existing problems.

How to use the Daisy Seed 8-Voice VA Synthesizer:

- Copy the files from the "DaisySP Filter Upgrade" directory into your DaisySP/Source/Filters directory.
- Run "Build DaisySP" task to update the library. 
- Connect audio outputs of the Daisy Seed board to an amp, mixer, or headphones.
- Plug the Daisy Seed board into your computers USB port.
- Run "Build and Program" to load the Daisy Seed board with the software.
- Connect a MIDI Keyboard to your computers USB port. I am using a MidiTech i2-61 MIDI controller currently.
- Run VMPK or any MIDI Sequencer software. I use FL Studio currently.
- Select "Miditechi2-61" as your MIDI Input device.
- Select "Daisy Seed Built In" as your MIDI Output device.
- If more than one Daisy Seed board is connected, it will appear as device "Daisy Seed Built In #X". Select its board number.
- The default patch loaded on startup is a PolySynth sound.
- Follow the instructions in "Daisy Seed Synth Parameters.txt" to select presets, modify parameters, and store new patches.


Specifications:

- 8 voice polyphonic.
- Dual oscillator per voice.
- VCA per voice. 
- VCF (4-pole Resonant low-pass) per voice.
- Separate ADSR's for VCA and VCF
- MIDI Editable parameters. 
- Ten Preset Patches.
- Ten User-Programmable Patches using QSPI Flash memory storage.
- Waveform select for both oscillators; Sine/Triangle/Pulse-Width-Square/Saw/Square/Polyblep-Tri/Polyblep-Saw/Polyblep-Square
- Oscillator mix. 
- Oscillator #2 De-tune. 
- Scale for Oscillator #2. 
- LFO for Pitch Modulation. 
- Keyboard velocity routable to VCA and/or VCF. 
- VCF envelope level. 
- Stereo simulation effect.


Editable Parameters:

(Parameters marked with a "-" have not been implemented yet, but are planned for the near future.)

Select Program Change 1 thru 29, then adjust the parameters value by moving the Data Slider.

 -(1): // Master Tuning
 -(2): // VCF Envelope to VCA Switch
 (3): // Osc Select 1
 (4): // Osc Select 2
 (5): // OSC Mix
 (6): // De-Tune
 (7): // Scale
 (8): // Resonance
 (9): // Pulse Width Value
 (10): // VCF Attack
 (11): // VCF Decay
 (12): // VCF Sustain
 (13): // VCF Release
 (14): // VCA Attack
 (15): // VCA Decay
 (16): // VCA Sustain
 (17): // VCA Release
 -(18): // VCF Follow level
 -(19): // ENV Follow level
 (20): // Velocity Select
 (21): // VCF Envelope level
 (22): // Mod LFO rate
 -(23): // PWM LFO rate
 -(24): // PWM LFO Mod level
 -(25): // PWM2 LFO rate
 -(26): // PWM2 LFO Mod level
 -(27): // VCF/VCA LFO rate
 -(28): // VCF LFO Mod level
 -(29): // VCA LFO Mod level

Select Program Change 30 thru 39 to choose a preset patch.

 (30): // Preset Select #1
 (31): // Preset Select #2
 (32): // Preset Select #3
 (33): // Preset Select #4
 (34): // Preset Select #5
 (35): // Preset Select #6
 (36): // Preset Select #7
 (37): // Preset Select #8
 (38): // Preset Select #9
 (39): // Preset Select #10
  
Select Program Change 40 thru 49 to load a user patch.

 (40): // User Patch Select #1
 (41): // User Patch Select #2
 (42): // User Patch Select #3
 (43): // User Patch Select #4
 (44): // User Patch Select #5
 (45): // User Patch Select #6
 (46): // User Patch Select #7
 (47): // User Patch Select #8
 (48): // User Patch Select #9
 (49): // User Patch Select #10

Select Program Change 50 thru 59 to store a user patch.

 (50): // User Patch Store #1
 (51): // User Patch Store #2
 (52): // User Patch Store #3
 (53): // User Patch Store #4
 (54): // User Patch Store #5
 (55): // User Patch Store #6
 (56): // User Patch Store #7
 (57): // User Patch Store #8
 (58): // User Patch Store #9
 (59): // User Patch Store #10
