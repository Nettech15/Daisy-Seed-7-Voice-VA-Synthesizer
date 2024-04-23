#pragma once 

#ifdef __cplusplus

#include <memory>

class VASynth;

class Sequencer
{
public:
    Sequencer(std::shared_ptr<VASynth> va_synth);
    
    void play();
    void record(uint8_t, uint8_t);
    
    void record_mode(bool enable);
    void stop_mode();
    void play_mode();
    
    enum Mode {
        Stop,
        RecordEnable,
        RecordDisable,
        Play      
    };
    
    Mode get_mode() { return mode_; }

private:
    void reset();
    
    uint16_t clock_ = 0;
    uint16_t time_ = 0;
    Mode mode_;
    uint16_t seqmsg;
    uint16_t seqnote;
    uint16_t seqvelocity;
    uint32_t memory_;
    const uint32_t memory_origin_ = 0x00010000;
    
    std::shared_ptr<VASynth> vasynth_;
};
#endif
