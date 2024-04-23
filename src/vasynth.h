#pragma once
#ifndef DSY_VAVOICE_H
#define DSY_VAVOICE_H

#include "daisysp.h"

// preset
#define PRESET_MAX 2

#define VOICES_MAX 8

using namespace daisysp;

typedef struct
{
    // config
    uint8_t waveform;
    float   osc_mix;
    float   detune;
    float   osc_pw;

    uint8_t osc2_waveform;
    float   osc2_detune;
    float   osc2_transpose;
    float   osc2_pw;

    float eg_a_attack;
    float eg_a_decay;
    float eg_a_sustain; // level
    float eg_a_release;
    float eg_f_attack;
    float eg_f_decay;
    float eg_f_sustain; // level
    float eg_f_release;

    uint8_t lfo_waveform;
    float   lfo_freq;
    float   lfo_amp;

    uint8_t pwmlfo_waveform;
    float   pwmlfo_freq;
    float   pwmlfo_amp;

    uint8_t pwm2lfo_waveform;
    float   pwm2lfo_freq;
    float   pwm2lfo_amp;

    uint8_t vcavcflfo_waveform;
    float   vcavcflfo_freq;
    float   vcavcflfo_amp;

    float vcf_kbd_follow;
    float env_kbd_follow;

    float   filter_res;
    float   filter_cutoff;
    float   eg_f_amount;
    uint8_t vel_select;
    uint8_t midi_channel;
} VASynthSetting;

class VASynth
{
  public:
    enum OSCid
    {
        OSC1,
        OSC2,
        MAX_OSC
    };
    enum Param
    {
        Waveform,
        Frequency,
        Amp
    };
    enum AdsrParam
    {
        ADSR_SEG_ATTACK  = daisysp::ADSR_SEG_ATTACK,
        ADSR_SEG_DECAY   = daisysp::ADSR_SEG_DECAY,
        ADSR_SEG_RELEASE = daisysp::ADSR_SEG_RELEASE,
        ADSR_SEG_SUSTAIN = ADSR_SEG_RELEASE << 1
    };

    enum EnvId
    {
        EnvAmplitude,
        EnvFrequency
    };

    VASynth(float sample_rate, unsigned initial_patch = 0);
    ~VASynth() = default;

    void Init();

    void SetMasterTune(int16_t data);
    void SetWaveform(uint8_t waveform, OSCid osc);
    void SetEG();
    void SetFilterCutoff(float cutoff);
    void SetFilterResonance(float res);

    void SetLFO(float value, Param param);
    void SetPWMLFO(float value, Param param);
    void SetPWM2LFO(float value, Param param);
    void SetVCAVCFLFO(float value, Param param);
    void Process(float *, float *);
    void NoteOn(uint8_t, uint8_t);
    void NoteOff(uint8_t);
    void FlashLoad(uint8_t aSlot);
    void FlashSave(uint8_t aSlot);
    void PitchBend(int16_t data);
    void SaveToLive(VASynthSetting *);
    void LiveToSave(VASynthSetting *);

    void SetOscillatorMix(float osc_mix) { osc_mix_ = osc_mix; }
    void SetOscPw(float value) { osc_pw_ = value; }
    void SetOsc2Pw(float value) { osc2_pw_ = value; }
    void SetOsc2Detune(float value) { osc2_detune_ = value; }
    void SetOsc2Transpose(float value) { osc2_transpose_ = value; }

    void SetEnvelopeGenerator(float value, EnvId id, AdsrParam param);
    void SetVcfKbdFollow(float value) { vcf_kbd_follow_ = value; }
    void SetEnvKbdFollow(float value) { env_kbd_follow_ = value; }
    void SetFreqEnvGenAmount(float value) { eg_f_amount_ = value; }
    void SetVelSelect(uint8_t value) { vel_select_ = value; }

    uint8_t getMidiChannel() { return midi_channel_; }

    bool IsPlaying() { return isPlaying; }

  private:
    bool isPlaying;

    // config
    float sample_rate_;

    uint8_t waveform_;
    float   osc_mix_;
    float   detune_;
    float   osc_pw_;

    uint8_t osc2_waveform_;
    float   osc2_detune_;
    float   osc2_transpose_;
    float   osc2_pw_;

    float eg_a_attack_;
    float eg_a_decay_;
    float eg_a_sustain_; // level
    float eg_a_release_;
    float eg_f_attack_;
    float eg_f_decay_;
    float eg_f_sustain_; // level
    float eg_f_release_;

    uint8_t lfo_waveform_;
    float   lfo_freq_;
    float   lfo_amp_;

    uint8_t pwmlfo_waveform_;
    float   pwmlfo_freq_;
    float   pwmlfo_amp_;

    uint8_t pwm2lfo_waveform_;
    float   pwm2lfo_freq_;
    float   pwm2lfo_amp_;

    uint8_t vcavcflfo_waveform_;
    float   vcavcflfo_freq_;
    float   vcavcflfo_amp_;

    float vcf_kbd_follow_;
    float env_kbd_follow_;

    float   filter_res_;
    float   filter_cutoff_;
    float   eg_f_amount_;
    uint8_t vel_select_;
    uint8_t midi_channel_;

    // runtime
    uint8_t osc_next_;
    uint8_t note_midi_[VOICES_MAX];
    float   note_freq_[VOICES_MAX];

    float follow[VOICES_MAX];
    float velocity[VOICES_MAX];
    float bender_offset[VOICES_MAX];

    Oscillator          osc_[VOICES_MAX];
    Oscillator          osc2_[VOICES_MAX];
    Oscillator          lfo_;
    Oscillator          pwmlfo_;
    Oscillator          pwm2lfo_;
    Oscillator          vcavcflfo_;
    Adsr                eg_a_[VOICES_MAX];
    Adsr                eg_f_[VOICES_MAX];
    daisysp::MoogLadder flt[VOICES_MAX];

    float pitch_bend_;
    float master_tune_;
};

#endif
