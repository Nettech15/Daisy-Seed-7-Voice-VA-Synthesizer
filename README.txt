How to use the Daisy Seed 8-Voice VA Synthesizer:

Connect audio outputs of the Daisy Seed board to an amp, mixer, or headphones.
Plug the Daisy Seed board into your computers USB port.
Make and Program the Daisy Seed board with the software.
Connect a MIDI Keyboard to your computers USB port. I am using a MidiTech i2-61 MIDI controller currently.
Run VMPK or any MIDI Sequencer software. I use FL Studio currently.
Select "Miditechi2-61" as your MIDI Input device.
Select "Daisy Seed Built In" as your MIDI Output device.
If more than one Daisy Seed board is connected, it will appear as device "Daisy Seed Built In #X". Select its board number.
The default patch loaded on startup is a PolySynth sound.
Follow the instructions in "Params.txt" to select presets, modify parameters, and store new patches.


Specifications:

8 voice polyphonic.
Dual oscillator per voice.
VCA per voice. 
VCF (4-pole Resonant low-pass) per voice.
Separate ADSR's for VCA and VCF
MIDI Editable parameters. 
Eight Preset Patches.
Eight User-Programmable Patches using QSPI Flash memory storage.
Waveform select for both oscillators; Sine/Triangle/Pulse-Width-Square/Saw/Square/Polyblep-Tri/Polyblep-Saw/Polyblep-Square
Oscillator mix. 
Oscillator #2 De-tune. 
Scale for Oscillator #2. 
LFO for Pitch Modulation. 
Keyboard velocity routable to VCA and/or VCF. 
VCF envelope level. 
Stereo simulation effect.


>>>> Daisy Seed 8-Voice VA Synthesizer with USB-MIDI Interface - Editable Parameters <<<<
(Parameters marked with a "-" have not been implemented yet)

Select Program Change 1 thru 29, then adjust the parameters value by moving the Data Slider.

 -(1): // Pitch Bend
 (2): // Modulation
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
 -(24): // PWM2 LFO rate
 -(25): // VCF LFO rate
 -(26): // VCF LFO Mod level
 -(27): // PWM LFO Mod level
 -(28): // PWM2 LFO Mod level
 -(29): // Unassigned

Select Program Change 30 thru 37 to choose a preset patch.

 (30): // Preset Select #1
 (31): // Preset Select #2
 (32): // Preset Select #3
 (33): // Preset Select #4
 (34): // Preset Select #5
 (35): // Preset Select #6
 (36): // Preset Select #7
 (37): // Preset Select #8
  
Select Program Change 40 thru 47 to load a user patch.
 (40): // User Patch Select #1
 (41): // User Patch Select #2
 (42): // User Patch Select #3
 (43): // User Patch Select #4
 (44): // User Patch Select #5
 (45): // User Patch Select #6
 (46): // User Patch Select #7
 (47): // User Patch Select #8

Select Program Change 50 thru 57 to store a user patch.
 (50): // User Patch Store #1
 (51): // User Patch Store #2
 (52): // User Patch Store #3
 (53): // User Patch Store #4
 (54): // User Patch Store #5
 (55): // User Patch Store #6
 (56): // User Patch Store #7
 (57): // User Patch Store #8



