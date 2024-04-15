#pragma once 

#ifdef __cplusplus

#include <memory>

class VASynth;

class Sequencer
{
public:
    Sequencer(std::shared_ptr<VASynth> va_synth);
    
    void play(uint16_t);
    void record(uint8_t, uint8_t);
    
    void record_mode(bool enable);
    void stop_mode();
    void play_mode();
    
    uint16_t get_mode() { return seqmode; }

private:
    uint16_t seqclock = 0;
    uint16_t seqtime = 0;
    uint16_t seqmode = 0;
    uint16_t seqmsg;
    uint16_t seqnote, seqvelocity;
    uint32_t seqmem = 0x00010000;
    
    std::shared_ptr<VASynth> vasynth_;
};
#endif
