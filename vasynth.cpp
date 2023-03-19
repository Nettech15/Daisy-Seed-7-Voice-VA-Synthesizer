#include "daisy_seed.h"
#include "daisysp.h"

#include "main.h"
#include "vasynth.h"

using namespace daisy;
using namespace daisysp;

// globals
extern DaisySeed hardware;
extern float sysSampleRate;
extern uint8_t gPlay;

extern uint8_t param_;
extern float pitch_bend;
extern float master_tune;

// fx
static DelayLine<float, DELAY_MAX> DSY_SDRAM_BSS delay_;

// presets

uint8_t preset_max = PRESET_MAX;
uint8_t preset_number = 0;

VASynthSetting preset_setting[PRESET_MAX] = 
{
	{//1
		VASynth::WAVE_POLYBLEP_SAW, 0.5f, 0.0f, 0.2f, FILTER_CUTOFF_MAX, 1.0f, 0.5f,
		0.0f, 0.4f, 1.0f, 0.1f,   0.0f, 0.4f, 0.2f, 0.1f,
		VASynth::WAVE_SIN, 0.5f, 0.0f,
		VASynth::WAVE_SIN, 0.2f, 0.2f,
		VASynth::WAVE_POLYBLEP_SAW, 0.1f, 1.0f, 1, MIDI_CHANNEL_ONE, 0.5f
	},
	{//2
		VASynth::WAVE_POLYBLEP_SQUARE, 0.5f, 0.0f, 0.2f, FILTER_CUTOFF_MAX, 1.0f, 0.5f,
		0.0f, 0.4f, 1.0f, 0.1f,   0.0f, 0.4f, 0.2f, 0.1f,
		VASynth::WAVE_SIN, 0.5f, 0.0f,
		VASynth::WAVE_SIN, 0.2f, 0.2f,
		VASynth::WAVE_POLYBLEP_SQUARE, 0.1f, 1.0f, 1, MIDI_CHANNEL_ONE, 0.5f
	},
	{//3
		VASynth::WAVE_POLYBLEP_TRI, 0.5f, 0.0f, 0.2f, FILTER_CUTOFF_MAX, 1.0f, 0.5f,
		0.0f, 0.4f, 1.0f, 0.1f,   0.0f, 0.4f, 0.2f, 0.1f,
		VASynth::WAVE_SIN, 0.5f, 0.0f,
		VASynth::WAVE_SIN, 0.2f, 0.2f,
		VASynth::WAVE_POLYBLEP_TRI, 0.1f, 1.0f, 1, MIDI_CHANNEL_ONE, 0.5f
	},
	{//4
		VASynth::WAVE_SIN, 0.5f, 0.0f, 0.2f, FILTER_CUTOFF_MAX, 1.0f, 0.5f,
		0.0f, 0.4f, 1.0f, 0.1f,   0.0f, 0.4f, 0.2f, 0.1f,
		VASynth::WAVE_SIN, 0.5f, 0.0f,
		VASynth::WAVE_SIN, 0.2f, 0.2f,
		VASynth::WAVE_SIN, 0.1f, 1.0f, 1, MIDI_CHANNEL_ONE, 0.5f
	},
	{//5
		VASynth::WAVE_POLYBLEP_SAW, 0.5f, 0.0f, 0.2f, FILTER_CUTOFF_MAX, 1.0f, 0.5f,
		0.0f, 0.4f, 1.0f, 0.1f,   0.0f, 0.4f, 0.2f, 0.1f,
		VASynth::WAVE_SIN, 0.5f, 0.0f,
		VASynth::WAVE_SIN, 0.2f, 0.2f,
		VASynth::WAVE_POLYBLEP_SAW, 0.1f, 1.0f, 1, MIDI_CHANNEL_ONE, 0.5f
	},
	{//6
		VASynth::WAVE_POLYBLEP_SQUARE, 0.5f, 0.0f, 0.2f, FILTER_CUTOFF_MAX, 1.0f, 0.5f,
		0.0f, 0.4f, 1.0f, 0.1f,   0.0f, 0.4f, 0.2f, 0.1f,
		VASynth::WAVE_SIN, 0.5f, 0.0f,
		VASynth::WAVE_SIN, 0.2f, 0.2f,
		VASynth::WAVE_POLYBLEP_SQUARE, 0.1f, 1.0f, 1, MIDI_CHANNEL_ONE, 0.5f
	},
	{//7
		VASynth::WAVE_POLYBLEP_TRI, 0.5f, 0.0f, 0.2f, FILTER_CUTOFF_MAX, 1.0f, 0.5f,
		0.0f, 0.4f, 1.0f, 0.1f,   0.0f, 0.4f, 0.2f, 0.1f,
		VASynth::WAVE_SIN, 0.5f, 0.0f,
		VASynth::WAVE_SIN, 0.2f, 0.2f,
		VASynth::WAVE_POLYBLEP_TRI, 0.1f, 1.0f, 1, MIDI_CHANNEL_ONE, 0.5f
	},
	{//8
		VASynth::WAVE_SIN, 0.5f, 0.0f, 0.2f, FILTER_CUTOFF_MAX, 1.0f, 0.5f,
		0.0f, 0.4f, 1.0f, 0.1f,   0.0f, 0.4f, 0.2f, 0.1f,
		VASynth::WAVE_SIN, 0.5f, 0.0f,
		VASynth::WAVE_SIN, 0.2f, 0.2f,
		VASynth::WAVE_SIN, 0.1f, 1.0f, 1, MIDI_CHANNEL_ONE, 0.5f
	},
	{//9
		VASynth::WAVE_RAMP, 0.5f, 0.0f, 0.2f, FILTER_CUTOFF_MAX, 1.0f, 0.5f,
		0.0f, 0.4f, 1.0f, 0.1f,   0.0f, 0.4f, 0.2f, 0.1f,
		VASynth::WAVE_SIN, 0.5f, 0.0f,
		VASynth::WAVE_SIN, 0.2f, 0.2f,
		VASynth::WAVE_RAMP, 0.1f, 1.0f, 1, MIDI_CHANNEL_ONE, 0.5f
	},
	{//10
		VASynth::WAVE_RAMP, 0.5f, 0.0f, 0.2f, FILTER_CUTOFF_MAX, 1.0f, 0.5f,
		0.0f, 0.4f, 1.0f, 0.1f,   0.0f, 0.4f, 0.2f, 0.1f,
		VASynth::WAVE_SIN, 0.5f, 0.0f,
		VASynth::WAVE_SIN, 0.2f, 0.2f,
		VASynth::WAVE_RAMP, 0.1f, 1.0f, 1, MIDI_CHANNEL_ONE, 0.5f
	}
};

/*
waveform, osc_mix, detune (unused), filter_res, filter_cutoff, filter envelope amount, osc_pw
eg_a_attack, d, s, r, eg_f_attack, d, s, r,
lfo_waveform, lfo_freq, lfo_amp,
pwmlfo_waveform, pwmlfo_freq, pwmlfo_amp,
osc2_waveform, osc2_detune, osc2_transpose, vel_select, midi_channel, osc2_pw;
*/

void VASynth::Init()
{
	for (uint8_t i = 0; i < VOICES_MAX; i++)
	{
		// oscillator
		osc_[i].Init(sample_rate_);
		osc_[i].SetWaveform(waveform_);
		osc_[i].SetAmp(1.0f); // default
		osc_[i].SetFreq(440.0f); // default
		osc_[i].SetPw(0.5f);

		osc2_[i].Init(sample_rate_);
		osc2_[i].SetWaveform(osc2_waveform_);
		osc2_[i].SetAmp(1.0f); // default
		osc2_[i].SetFreq(440.0f * osc2_detune_); // default
		osc2_[i].SetPw(0.5f);
		
		// EG - amplitude
		eg_a_[i].Init(sample_rate_);
		eg_a_[i].SetTime(ADSR_SEG_ATTACK, eg_a_attack_);
		eg_a_[i].SetTime(ADSR_SEG_DECAY, eg_a_decay_);
		eg_a_[i].SetTime(ADSR_SEG_RELEASE, eg_a_release_);
		eg_a_[i].SetSustainLevel(eg_a_sustain_);

		// EG - filter
		eg_f_[i].Init(sample_rate_);
		eg_f_[i].SetTime(ADSR_SEG_ATTACK, eg_f_attack_);
		eg_f_[i].SetTime(ADSR_SEG_DECAY, eg_f_decay_);
		eg_f_[i].SetTime(ADSR_SEG_RELEASE, eg_f_release_);
		eg_f_[i].SetSustainLevel(eg_f_sustain_);

		// LP - filter
		flt[i].Init(sample_rate_);
		flt[i].SetFreq(filter_cutoff_);
		flt[i].SetRes(filter_res_);
	}

	// lfo
	lfo_.Init(sample_rate_);
	lfo_.SetWaveform(lfo_waveform_);
	lfo_.SetFreq(lfo_freq_);
	lfo_.SetAmp(lfo_amp_);
	
	pwmlfo_.Init(sample_rate_);
	pwmlfo_.SetWaveform(pwmlfo_waveform_);
	pwmlfo_.SetFreq(pwmlfo_freq_);
	pwmlfo_.SetAmp(pwmlfo_amp_);

	// stereo simulator
	delay_.Init();
	delay_.SetDelay(sample_rate_ * 0.01f);

    // init
    osc_next_ = 0;
}

void VASynth::First()
{
	sample_rate_ = sysSampleRate;
	SaveToLive(&preset_setting[0]);
}	

void VASynth::SetWaveform()
{
	for (uint8_t i = 0; i < VOICES_MAX; i++)
	{
		osc_[i].SetWaveform(waveform_);
		osc2_[i].SetWaveform(osc2_waveform_);
	}
}

void VASynth::SetEG()
{
	for (uint8_t i = 0; i < VOICES_MAX; i++)
	{
		// EG - filter
		eg_f_[i].SetTime(ADSR_SEG_ATTACK, eg_f_attack_);
		eg_f_[i].SetTime(ADSR_SEG_DECAY, eg_f_decay_);
		eg_f_[i].SetTime(ADSR_SEG_RELEASE, eg_f_release_);
		eg_f_[i].SetSustainLevel(eg_f_sustain_);

		// EG - amplitude
		eg_a_[i].SetTime(ADSR_SEG_ATTACK, eg_a_attack_);
		eg_a_[i].SetTime(ADSR_SEG_DECAY, eg_a_decay_);
		eg_a_[i].SetTime(ADSR_SEG_RELEASE, eg_a_release_);
		eg_a_[i].SetSustainLevel(eg_a_sustain_);
	}
}

void VASynth::SetFilter()
{
	for (uint8_t i = 0; i < VOICES_MAX; i++)
	{
		flt[i].SetFreq(filter_cutoff_);
		flt[i].SetRes(filter_res_);
	}
}

void VASynth::SetLFO()
{
	lfo_.SetWaveform(lfo_waveform_);
	lfo_.SetFreq(lfo_freq_*10);
	lfo_.SetAmp(lfo_amp_);
}

void VASynth::SetPWMLFO()
{
	pwmlfo_.SetWaveform(pwmlfo_waveform_);
	pwmlfo_.SetFreq(pwmlfo_freq_);
	pwmlfo_.SetAmp(pwmlfo_amp_);
}

void VASynth::Process(float *out_l, float *out_r)
{
	float lfo_out;
	float pwmlfo_out;
	float env_a_out;
	float env_f_out;
	float osc_one;
	float osc_two;
	float osc_out;
	float filter_out;
	float voice_out;
	float delay_out;
	float bender;
	float tuning;
	float pitchmod;
	float detune;

	bool note_on;

	// mod and pwm lfo's
	lfo_out = lfo_.Process();
	pwmlfo_out = pwmlfo_.Process();
	
	filter_out = 0;

	for (uint8_t i = 0; i < VOICES_MAX; i++)
	{
		note_on = (note_midi_[i] != 0);

		// EG - AMP
		env_a_out = eg_a_[i].Process(note_on);

		if(vel_select_ >= 2)
		{
			// Enable velocity control of the VCA on osc
			osc_[i].SetAmp(env_a_out * velocity[i]);
		}
		else
		{
			// Disable velocity control of the VCA on osc
			osc_[i].SetAmp(env_a_out);
		}

		if(vel_select_ >= 2)
		{
			// Enable velocity control of the VCA on osc2
			osc2_[i].SetAmp(env_a_out * velocity[i]);
		}
		else
		{
			// Disable velocity control of the VCA on osc2
			osc2_[i].SetAmp(env_a_out);
		}

		// Calculate keyboard follow for ENV and VCF
		follow[i] = (float (note_midi_[i])/42.0f);

		// Set osc + osc2 frequencies with pitch bend, mod wheel, master tuning, and osc2 detune
		tuning = bender_offset[i] * master_tune;
		bender = bender_offset[i] * pitch_bend;
		pitchmod = bender_offset[i] * lfo_out;
		detune = bender_offset[i] * osc2_detune_;
		osc_[i].SetFreq((note_freq_[i] + detune_ + bender + tuning + pitchmod));
		osc2_[i].SetFreq((((note_freq_[i] * osc2_transpose_) + (bender * osc2_transpose_) + (tuning * osc2_transpose_)) + (pitchmod * osc2_transpose_) + (detune * osc2_transpose_)));

		// Set the pw, process both oscillators, then mix them
		osc_[i].SetPw(osc_pw_ + pwmlfo_out);
		osc2_[i].SetPw(osc2_pw_ + pwmlfo_out);

		osc_one = osc_[i].Process();
		osc_two = osc2_[i].Process();

		osc_out = (osc_one * (osc_mix_)) + ((osc_two * (1.0f - osc_mix_)));

		// filter		
		env_f_out = eg_f_[i].Process(note_on);

		if(vel_select_ == 1 || vel_select_ == 3)
		{
			// Enable velocity control of the VCF 
			flt[i].SetFreq(filter_cutoff_ * (env_f_out * velocity[i] * eg_f_amount_));
		}
		else
		{
			// Disable velocity control of the VCF
			flt[i].SetFreq(filter_cutoff_ * (env_f_out * eg_f_amount_));
		}
		
		filter_out += flt[i].Process(osc_out);
	}

	filter_out /= VOICES_MAX;

	voice_out = filter_out;

	// delay
	delay_out = delay_.Read();
	delay_.Write(voice_out);
	
	// out
	*out_l = voice_out;
	*out_r = delay_out;
}

void VASynth::NoteOn(uint8_t midi_note, uint8_t midi_velocity)
{
	for (uint8_t i = 0; i < VOICES_MAX; i++)
	{
		osc_next_ = (osc_next_ + 1) % VOICES_MAX;

		if(note_midi_[osc_next_] == 0)
		{
			note_midi_[osc_next_] = midi_note;
			note_freq_[osc_next_] = mtof(note_midi_[osc_next_]);
			bender_offset[osc_next_] = ((note_freq_[osc_next_]) - (mtof(note_midi_[osc_next_] + 1)));
			velocity[osc_next_] = ((float)midi_velocity / MIDI_VELOCITY_MAX);
			break;
		}
	}
}	

void VASynth::NoteOff(uint8_t midi_note)
{
	for (uint8_t i = 0; i < VOICES_MAX; i++)
	{
		if (note_midi_[i] == midi_note)
		{
			note_midi_[i] = 0;
		}
	}
}	

void VASynth::PitchBend(int16_t data)
{
	pitch_bend = 1.0f - ((float)data / 4096.0f);
}	

void VASynth::ProgramChange(uint8_t data)
{
	param_= data;
}

// Flash handling - load and save
// 8MB of flash
// 4kB blocks
// assume our settings < 4kB, so put one patch per block
#define FLASH_BLOCK 4096

uint8_t DSY_QSPI_BSS qspi_buffer[FLASH_BLOCK * 16];
 
void VASynth::FlashLoad(uint8_t aSlot)
{
	VASynthSetting vaload;

    size_t size = sizeof(VASynthSetting);
    
	//memcpy(*dest, *src, sizet);
	memcpy(&vaload, &qspi_buffer[aSlot * FLASH_BLOCK], size);

	SaveToLive(&vaload);
}



void VASynth::FlashSave(uint8_t aSlot)
{
	VASynthSetting vasave;

	LiveToSave(&vasave);

	size_t start_address = (size_t)qspi_buffer;

	size_t size = sizeof(VASynthSetting);
    
	size_t slot_address = start_address + (aSlot * FLASH_BLOCK);

    hardware.qspi.Erase(slot_address, slot_address + size);
    hardware.qspi.Write(slot_address, size, (uint8_t*)&vasave);

}	

void VASynth::SaveToLive(VASynthSetting *vas)
{
	gPlay = PLAY_OFF;
	
	waveform_ = vas->waveform;
	osc_mix_ = vas->osc_mix;
	detune_ = vas->detune;

	filter_res_ = vas->filter_res;
	filter_cutoff_ = vas->filter_cutoff;
	eg_f_amount_ = vas->eg_f_amount;
	osc_pw_ = vas->osc_pw;
	
	eg_a_attack_ = vas->eg_a_attack;
	eg_a_decay_ = vas->eg_a_decay;
	eg_a_sustain_ = vas->eg_a_sustain; // level
	eg_a_release_ = vas->eg_a_release;
	eg_f_attack_ = vas->eg_f_attack;
	eg_f_decay_ = vas->eg_f_decay;
	eg_f_sustain_ = vas->eg_f_sustain; // level
	eg_f_release_ = vas->eg_f_release;

	lfo_waveform_ = vas->lfo_waveform;
	lfo_freq_ = vas->lfo_freq;
	lfo_amp_ = vas->lfo_amp;

	pwmlfo_waveform_ = vas->pwmlfo_waveform;
	pwmlfo_freq_ = vas->pwmlfo_freq;
	pwmlfo_amp_ = vas->pwmlfo_amp;

	osc2_waveform_ = vas->osc2_waveform;
	osc2_detune_ = vas->osc2_detune;
	osc2_transpose_ = vas->osc2_transpose;
	vel_select_ = vas->vel_select;
	
	midi_channel_ = vas->midi_channel;
	osc2_pw_ = vas->osc2_pw;

	osc_next_ = 0;
	Init();
	gPlay = PLAY_ON;
}

void VASynth::LiveToSave(VASynthSetting *vas)
{
	vas->waveform = waveform_;
	vas->osc_mix = osc_mix_;
	vas->detune = detune_;

	vas->filter_res = filter_res_;
	vas->filter_cutoff = filter_cutoff_;
	vas->eg_f_amount = eg_f_amount_;
	vas->osc_pw = osc_pw_;
	
	vas->eg_a_attack = eg_a_attack_;
	vas->eg_a_decay = eg_a_decay_;
	vas->eg_a_sustain = eg_a_sustain_;
	vas->eg_a_release = eg_a_release_;
	vas->eg_f_attack = eg_f_attack_;
	vas->eg_f_decay = eg_f_decay_;
	vas->eg_f_sustain = eg_f_sustain_;
	vas->eg_f_release = eg_f_release_;
	
	vas->lfo_waveform = lfo_waveform_;
	vas->lfo_freq = lfo_freq_;
	vas->lfo_amp = lfo_amp_;

	vas->pwmlfo_waveform = pwmlfo_waveform_;
	vas->pwmlfo_freq = pwmlfo_freq_;
	vas->pwmlfo_amp = pwmlfo_amp_;

	vas->osc2_waveform = osc2_waveform_;
	vas->osc2_detune = osc2_detune_;
	vas->osc2_transpose = osc2_transpose_;
	vas->vel_select = vel_select_;
	
	vas->midi_channel = midi_channel_;
	vas->osc2_pw = osc2_pw_;
}
