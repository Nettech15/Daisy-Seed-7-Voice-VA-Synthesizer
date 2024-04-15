#pragma once
#ifndef DSY_VAVOICE_H
#define DSY_VAVOICE_H

#include "daisysp.h"

#include "main.h"

// preset
#define PRESET_MAX 2

// waveforms
#define WAVE_TRI 1
#define WAVE_SAW 2
#define WAVE_SQUARE 4
#define WAVE_POLYBLEP_SAW 6 

using namespace daisy;
using namespace daisysp;

typedef struct
{
	// config
	uint8_t waveform;
	float osc_mix;
	float detune;
	float osc_pw;

	uint8_t osc2_waveform;
	float osc2_detune;
	float osc2_transpose;
	float osc2_pw;
	
	float eg_a_attack;
	float eg_a_decay;
	float eg_a_sustain; // level
	float eg_a_release;
	float eg_f_attack;
	float eg_f_decay;
	float eg_f_sustain; // level
	float eg_f_release;
	
	uint8_t lfo_waveform;
	float lfo_freq;
	float lfo_amp;

	uint8_t pwmlfo_waveform;
	float pwmlfo_freq;
	float pwmlfo_amp;

	uint8_t pwm2lfo_waveform;
	float pwm2lfo_freq;
	float pwm2lfo_amp;
	
	uint8_t vcavcflfo_waveform;
	float vcavcflfo_freq;
	float vcavcflfo_amp;

	float vcf_kbd_follow;
	float env_kbd_follow;

	float filter_res;
	float filter_cutoff;
	float eg_f_amount;
	uint8_t vel_select;
	uint8_t midi_channel;
} VASynthSetting;

class VASynth
{
	public:

    VASynth() {}
    ~VASynth() {}

	void Init();
	void First(uint8_t);
	void SetWaveform();
	void SetEG();
	void SetFilter();
	void SetLFO();
	void SetPWMLFO();
	void SetPWM2LFO();
	void SetVCAVCFLFO();
	void Process(float *, float *);
	void NoteOn(uint8_t, uint8_t);
	void NoteOff(uint8_t);
	void FlashLoad(uint8_t aSlot);
	void FlashSave(uint8_t aSlot);
	void ProgramChange(uint8_t data);
	void PitchBend(int16_t data);
	void SaveToLive(VASynthSetting *);
	void LiveToSave(VASynthSetting *);

//private:

	// config
	float sample_rate_;

	uint8_t waveform_;
	float osc_mix_;
	float detune_;
	float osc_pw_;

	uint8_t osc2_waveform_;
	float osc2_detune_;
	float osc2_transpose_;
	float osc2_pw_;

	float eg_a_attack_;
	float eg_a_decay_;
	float eg_a_sustain_; // level
	float eg_a_release_;
	float eg_f_attack_;
	float eg_f_decay_;
	float eg_f_sustain_; // level
	float eg_f_release_;

	uint8_t lfo_waveform_;
	float lfo_freq_;
	float lfo_amp_;

	uint8_t pwmlfo_waveform_;
	float pwmlfo_freq_;
	float pwmlfo_amp_;

	uint8_t pwm2lfo_waveform_;
	float pwm2lfo_freq_;
	float pwm2lfo_amp_;

	uint8_t vcavcflfo_waveform_;
	float vcavcflfo_freq_;
	float vcavcflfo_amp_;
	
	float vcf_kbd_follow_;
	float env_kbd_follow_;

	float filter_res_;
	float filter_cutoff_;
	float eg_f_amount_;
	uint8_t vel_select_;
	uint8_t midi_channel_;
	
	// runtime
	uint8_t osc_next_;
	uint8_t note_midi_[VOICES_MAX];
	float note_freq_[VOICES_MAX];

	float follow[VOICES_MAX];
	float velocity[VOICES_MAX];
	float bender_offset[VOICES_MAX];

	Oscillator osc_[VOICES_MAX];
	Oscillator osc2_[VOICES_MAX];
	Oscillator lfo_;
	Oscillator pwmlfo_;
	Oscillator pwm2lfo_;
	Oscillator vcavcflfo_;
	Adsr eg_a_[VOICES_MAX];
	Adsr eg_f_[VOICES_MAX];
	LadderFilter flt[VOICES_MAX];
};

#endif
