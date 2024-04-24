Description of the project:

This code is a modification of OscPocketD/VASynth, created by Staffan Melin, staffan.melin@oscillator.se.
It was later modified by (Steven @moonfriendsynth) to work with the Daisy Pod.
I (Christopher @Nettech15) have modified it further to run on the Daisy Seed without the use of the Daisy Pod board.
Synth parameters are now controlled by a Miditech i2-61 midi keyboard.
Feel free to copy, modify, and improve this code to match your equipment and sound requirements.

Specifications:

- 7 voice polyphonic.
- Dual oscillator per voice.
- VCA per voice.
- VCF (4-pole Resonant low-pass) per voice.
- Separate ADSR's for VCA and VCF
- MIDI Editable parameters.
- Ten User-Programmable Patches using QSPI Flash memory storage.
- Waveform select for both oscillators; Triangle / Saw / Square(PWM) / Polyblep-Saw.
- Oscillator Mix.
- Oscillator #2 De-tune.
- Scale for Oscillator #2.
- LFO for Pitch Modulation Wheel.
- LFO for Pulse Width Modulation #1.
- LFO for Pulse Width Modulation #2.
- LFO for VCA/VCF Modulation.
- Waveform select for VCF/VCA LFO.
- Keyboard follow for VCF and ENV.
- Keyboard velocity routable to VCA and/or VCF.
- VCF envelope level.
- Stereo simulation effect.
- Single-Track ScratchPad Sequencer.


How to use the Daisy Seed 7-Voice VA Synthesizer:

- Connect audio outputs of the Daisy Seed board to an amp, mixer, or headphones.
- Plug the Daisy Seed board into your computers USB port.
- Run "build_and_program" (or "build_and_program_dfu") task to load the Daisy Seed board with the software.
- Connect a MIDI Keyboard to your computers USB port. I am using a MidiTech i2-61 MIDI controller currently.
- Run VMPK or any MIDI Sequencer software.
- Select "Miditechi2-61" as your MIDI Input device.
- Select "Daisy Seed Built In" as your MIDI Output device.
- If more than one Daisy Seed board is connected, it will appear as device "Daisy Seed Built In #X". Select its board number.
- The default patch loaded on startup is a PolySynth sound.
- Follow the instructions in "Daisy Seed Synth Parameters.txt" to modify parameters, and store user patches.
